#include <Python.h>
#include <iterator>
#include <algorithm>

#include "PluginPython3.hpp"
#include "ModuleDocument.hpp"
#include "PluginState.hpp"
#include "Interpreter.hpp"
#include <ifcdb/IFCDBInterfaceVersion.hpp>
#include "PluginInterfaceVersion.hpp"
#include "ComponentRegistry.hpp"
#include "PythonPluginManifest.hpp"
#include <stdutils/executable.hpp>

using namespace sdm::plugin;

IMPLEMENT_PLUGIN(PluginPython3);

PluginPython3::PluginPython3()
{
    PluginState pluginState;
    IfcDBState dummy;
    Interpreter interpreter(pluginState, dummy);

    try
    {
        interpreter.run();
    }
    catch (const std::exception& e)
    {
        m_errorMessages.emplace_back(e.what());
    }

    pluginState = interpreter.getPluginState();

    for (const auto& pluginFeatures : pluginState.pluginFeatures)
    {
        std::string popup("Python");

        if (!pluginFeatures.popup.empty())
        {
          popup += "|" + pluginFeatures.popup;
        }

        m_features.emplace_back(std::make_unique<PythonAction>(pluginFeatures.name, popup));
    }

    m_documentObserver.attach([&](IfcDB::Populationi* db)
    {
        IfcDB::assignGlobalStates(db);
        for (const auto& feature : m_features)
        {
            feature->setDb(db);
        }

        for (const auto& action : m_manifestActions)
        {
            action->setDb(db);
        }
    });

    m_documentObserver.attach([&](IfcDB::utils::PopulationSubject* pStates)
    {
        IfcDB::utils::PopulationStates* pPopStates = dynamic_cast<IfcDB::utils::PopulationStates*>(pStates);

        for (const auto& feature : m_features)
        {
            feature->setStates(dynamic_cast<IfcDB::utils::PopulationStates*>(pStates));
        }

        for (const auto& action : m_manifestActions)
        {
            action->setStates(pPopStates);
        }
    });

    m_liveLogObserver.attach([&](sdm::plugin::LiveLogInterface* logInterface)
    {
        if (!m_errorMessages.empty())
        {
            for (const auto& message : m_errorMessages)
            {
                sdm::plugin::MessageLevel level(sdm::plugin::MessageLevel::Verbose);

                if (message.find("Error]") != std::string::npos)
                {
                  level = sdm::plugin::MessageLevel::Error;
                }

                logInterface->log(level, "PythonPluginInitialization", message);
            }
        }

        for (const auto& pythonAction : m_features)
        {
            pythonAction->setLogInterface(logInterface);
            pythonAction->setAppFeature(&m_appFeature);
        }

        for (const auto& action : m_manifestActions)
        {
            action->setLogInterface(logInterface);
            action->setAppFeature(&m_appFeature);
        }
    });

    try
    {
        auto pluginDir = stdutils::findDLLFileName().parent_path().parent_path();

        for (const auto& manifest : loadManifests(pluginDir))
        {
            m_manifestActions.emplace_back(std::make_unique<ManifestAction>(manifest));
        }
    }
    catch (const std::exception& e)
    {
        m_errorMessages.emplace_back(e.what());
    }
}

PluginPython3::~PluginPython3()
{
}

sdm::plugin::Version PluginPython3::getInterfaceVersion() const
{
    return sdm::plugin::Version();
}

sdm::plugin::PluginInfo PluginPython3::getInfo() const
{
  sdm::plugin::PluginInfo info;
  info.name         = "Python 3";
  info.description  = "Python API to use own python scripts with the KIT-ModelViewer Population";
  info.version      = { 0, 5 };

  return info;
}

std::vector<sdm::plugin::Feature*> PluginPython3::getFeatures() const
{
    std::vector<sdm::plugin::Feature*> features;
    features.emplace_back(const_cast<VenvAction*>(&m_venvFeature));
    features.emplace_back(const_cast<SeparatorAction*>(&m_separatorAction));
    features.emplace_back(const_cast<sdm::plugin::DocumentObserverImpl*>(&m_documentObserver));
    features.emplace_back(const_cast<sdm::plugin::LiveLogObserver*>(&m_liveLogObserver));
    features.emplace_back(const_cast<sdm::plugin::AppFeatureHelper*>(&m_appFeature));

    std::transform(m_features.begin(), m_features.end(), std::back_inserter(features), [](const std::unique_ptr<PythonAction>& ptr)
    {
        return ptr.get();
    });

    std::transform(m_manifestActions.begin(), m_manifestActions.end(), std::back_inserter(features), [](const std::unique_ptr<ManifestAction>& ptr)
    {
        return ptr.get();
    });

    return features;
}

void PluginPython3::addFeature(std::string name)
{
    m_features.emplace_back(std::make_unique<PythonAction>(name, "Python"));
}

ComponentInfo PluginPython3::getComponentInfo(const RequiredComponent& requiredComponent) const
{
  ComponentRegistry availableComponents;
  availableComponents.addAvailable(IFCDB_INTERFACE_COMPONENT_NAME, IFCDB_INTERFACE_COMPONENT_VERSION, IFCDB_INTERFACE_COMPONENT_HINT, std::atoi(IFCDB_INTERFACE_COMPONENT_DATE.data()));
  availableComponents.addAvailable(PLUGIN_INTERFACE_COMPONENT_NAME, PLUGIN_INTERFACE_COMPONENT_VERSION, PLUGIN_INTERFACE_COMPONENT_HINT, std::atoi(PLUGIN_INTERFACE_COMPONENT_DATE.data()));

  return availableComponents.getInfo(requiredComponent);
}

const sdm::plugin::InitializationState& PluginPython3::getInitializationState() const
{
  return m_initState;
}
