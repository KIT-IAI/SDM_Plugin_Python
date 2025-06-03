#include "ManifestAction.hpp"
#include "Interpreter.hpp"

ManifestAction::ManifestAction(const PythonPluginManifest& manifest)
: m_manifest{manifest}, sdm::plugin::ActionFeatureHelper(manifest.actionName)
{
    if (!manifest.popupName.empty())
    {
        auto properties = getProperties();
        properties.popupName = manifest.popupName;
        setProperties(properties);
    }
}

void ManifestAction::execute() const
{
    try
    {
        m_logInterface->log(sdm::plugin::MessageLevel::Info, "PythonMessage", ">>> Python plugin: " + getName());

        Interpreter interpreter(PluginState{{}, getName()}, m_state);
        interpreter.setVenvDir(m_manifest.manifestDir / "venv");
        interpreter.setScriptPath(m_manifest.manifestDir / m_manifest.scriptName);
        interpreter.run();

        if (interpreter.getUpdateState() && m_state.states)
        {
            m_state.states->compile();
        }
    }
    catch (std::exception& e)
    {
        std::string message(e.what());
        sdm::plugin::MessageLevel level(sdm::plugin::MessageLevel::Verbose);

        if (message.find("Error]") != std::string::npos)
        {
            level = sdm::plugin::MessageLevel::Error;
        }

        m_logInterface->log(level, "PythonActionExecute", e.what());
    }

    m_logInterface->log(sdm::plugin::MessageLevel::Info, "PythonMessage", ">>> Python plugin finished");
}
