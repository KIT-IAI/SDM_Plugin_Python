#pragma once

#include "PluginState.hpp"
#include "IfcDBState.hpp"
#include "fs.hpp"

class Interpreter
{
  public:
    explicit Interpreter(PluginState pluginState, IfcDBState ifcDBState);
    ~Interpreter();

    void setVenvDir(const fs::path& venvDir) { m_venvDir = venvDir; }
    void setScriptPath(const fs::path& scriptPath) { m_scriptPath = scriptPath; }

    void run();

    auto getPluginState() const { return m_pluginState; }
    bool getUpdateState() { return m_ifcDbState.updateState; }

  private:
    PluginState m_pluginState;
    IfcDBState m_ifcDbState;
    void checkError() const;
    std::string makeErrorTypeString(PyObject* pObject) const;

    bool hasVenv() const;

    fs::path m_venvDir;
    fs::path m_scriptPath;

    void initVenv();
};