#include <Python.h>

#include "Interpreter.hpp"

#include "PythonAction.hpp"

PythonAction::PythonAction(const std::string& name)
  : sdm::plugin::ActionFeatureHelper(name)
{
}

PythonAction::PythonAction(const std::string& name, const std::string& popupName)
  : sdm::plugin::ActionFeatureHelper(name, popupName)
{
  auto properties = getProperties();
  properties.popupName = popupName;
  setProperties(properties);
}

void PythonAction::execute() const
{
    if (m_state.db)
    {
        int serial = m_state.db->getSerialNo();
    }

    try
    {
        m_logInterface->log(sdm::plugin::MessageLevel::Info, "PythonMessage", ">>> Python script: " + getName());

        Interpreter interpreter(PluginState{{}, getName()}, m_state);
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
        Py_FinalizeEx();
    }

    m_logInterface->log(sdm::plugin::MessageLevel::Info, "PythonMessage", ">>> Python script finished");
}
