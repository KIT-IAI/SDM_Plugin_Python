#pragma once

#include <Plugin.hpp>
#include <DocumentObserver.hpp>
#include <LiveLogFeature.hpp>
#include <AppFeatureHelper.hpp>
#include <Python.h>
#include <string>
#include "ActionFeature.hpp"
#include "ModuleDocument.hpp"
#include "PythonAction.hpp"
#include "VenvAction.hpp"
#include "ManifestAction.hpp"
#include <stdutils/filesystem.hpp>
#include <SeparatorAction.hpp>

class PluginPython3 : public sdm::plugin::Plugin
{
public:
    PluginPython3();
    ~PluginPython3() override;

    sdm::plugin::Version getInterfaceVersion() const override;
    sdm::plugin::PluginInfo getInfo() const override;
    std::vector<sdm::plugin::Feature*> getFeatures() const override;

    void addFeature(std::string name);

    sdm::plugin::ComponentInfo getComponentInfo(const sdm::plugin::RequiredComponent& requiredComponent) const override;
    const sdm::plugin::InitializationState& getInitializationState() const override;

    Types m_types{nullptr};
    IfcDB::Populationi* m_db = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;

private:
    stdutils::fs::path findScriptsDir();

    sdm::plugin::DocumentObserverImpl m_documentObserver;
    sdm::plugin::LiveLogObserver m_liveLogObserver;
    sdm::plugin::AppFeatureHelper m_appFeature;
    std::vector<std::unique_ptr<PythonAction>> m_features;
    std::vector<std::string> m_errorMessages;
    sdm::plugin::InitializationState m_initState;

    VenvAction m_venvFeature;
    sdm::plugin::SeparatorAction m_separatorAction;
    std::vector<std::unique_ptr<ManifestAction>> m_manifestActions;
};