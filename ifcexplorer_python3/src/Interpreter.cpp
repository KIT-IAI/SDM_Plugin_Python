#include <utility>
#include <Python.h>
#include <stdutils/filesystem.hpp>
#include <stdutils/executable.hpp>

#include "PluginModule.hpp"
#include "IfcDBModule.hpp"

#include "Interpreter.hpp"
#include "ModuleDocument.hpp"
#include "InterpreterError.hpp"
#include <string_view>

constexpr std::string_view PythonEmbeddedVersion = "python311.zip";

stdutils::fs::path findScriptsDir()
{
    auto executableDir = stdutils::findExecutableFileName().parent_path();

    if (auto scriptsDir = executableDir / "scripts"; exists(scriptsDir))
    {
        return scriptsDir;
    }

    if (auto scriptsDir = executableDir.parent_path() / "scripts"; exists(scriptsDir))
    {
        return scriptsDir;
    }

    if (auto scriptsDir = executableDir / "plugins" / "ifcexplorer_python3" / "scripts"; exists(scriptsDir))
    {
      return scriptsDir;
    }

    throw std::runtime_error("script dir not found");
}

std::string makeStringIfNotNull(const char* str)
{
    if (str)
    {
        return {str};
    }

    return {};
}

std::string makeErrorTypeStr(PyObject* pObject)
{
    const auto pTypeName = PyObject_GetAttrString(pObject, "__name__");

    if (!pTypeName)
    {
        return "type unresolved";
    }

    std::string str;

    if (PyUnicode_Check(pTypeName))
    {
        str = makeStringIfNotNull(PyUnicode_AsUTF8(pTypeName));
    }

    Py_DecRef(pTypeName);

    return str;
}

std::string makeShortMessageStr(PyObject* pObject)
{
    if (!pObject)
    {
        return {};
    }

    const auto pStr = PyObject_Str(pObject);

    if (!pStr)
    {
        return {};
    }

    std::string str;

    if (PyUnicode_Check(pStr))
    {
        str = PyUnicode_AsUTF8(pStr);
    }

    Py_DecRef(pStr);

    return str;
}

std::string makeLongMessageStr(PyObject* pObject)
{
    if (!pObject)
    {
        return {};
    }

    if (!PyTraceBack_Check(pObject))
    {
        return {};
    }

    auto pTraceBack = reinterpret_cast<PyTracebackObject*>(pObject);

    std::string str;

    while (pTraceBack)
    {
        const auto lineNo = PyFrame_GetLineNumber(pTraceBack->tb_frame);
        const auto code = PyFrame_GetCode(pTraceBack->tb_frame);

        std::string filename;
        if (PyUnicode_Check(code->co_filename))
        {
            filename = makeStringIfNotNull(PyUnicode_AsUTF8(code->co_filename));
        }

        if (!filename.empty())
        {
            stdutils::fs::path path = filename;

            if (!str.empty())
            {
                str += " ";
            }

            str += path.filename().generic_string() + ":" + std::to_string(lineNo);
        }

        pTraceBack = pTraceBack->tb_next;
    }

    return str;
}

Interpreter::Interpreter(PluginState pluginState, IfcDBState ifcDBState)
: m_pluginState(std::move(pluginState)), m_ifcDbState(std::move(ifcDBState))
{
}

Interpreter::~Interpreter()
{
}

void Interpreter::run()
{
    const auto scriptsDir = findScriptsDir();

    PyImport_AppendInittab("plugin", []()
    {
        return PyModule_Create(&PluginModuleDefinition);
    });

    PyImport_AppendInittab("ifcdb", []() {
        return PyModule_Create(&IfcDBModuleDefinition);
    });

    if (hasVenv())
    {
        initVenv();
    }
    else
    {
        const auto pythonPath = stdutils::findExecutableFileName().parent_path();
        const auto stdLibraryPath = pythonPath / "plugins" / "ifcexplorer_python3" / PythonEmbeddedVersion;

        if (!exists(stdLibraryPath))
        {
            throw std::runtime_error(std::string{PythonEmbeddedVersion} + " not found");
        }

        Py_SetPath(stdLibraryPath.wstring().c_str());
        Py_Initialize();
    }

    {
        const auto pluginModule = PyImport_ImportModule("plugin");
        const auto pluginCapsule = PyCapsule_New(&m_pluginState, "plugin._C_API", nullptr);
        PyModule_AddObject(pluginModule, "_C_API", pluginCapsule);
    }

    {
        const auto pluginModule = PyImport_ImportModule("ifcdb");
        const auto pluginCapsule = PyCapsule_New(&m_ifcDbState, "ifcdb._C_API", nullptr);
        PyModule_AddObject(pluginModule, "_C_API", pluginCapsule);
    }

    m_ifcDbState.types = initTypes(PyImport_ImportModule("plugin"));

    if (hasVenv())
    {
        std::string script = m_scriptPath.stem().string();
        const auto mainModulePath = PyUnicode_DecodeFSDefault(script.c_str());
        const auto module = PyImport_Import(mainModulePath);

        checkError();

        if (mainModulePath)
        {
            Py_DecRef(mainModulePath);
        }

        if (module)
        {
            Py_DecRef(module);
        }
    }
    else
    {
        auto sys = PyImport_ImportModule("sys");
        auto path = PyObject_GetAttrString(sys, "path");
        PyList_Append(path, PyUnicode_FromString(scriptsDir.string().c_str()));

        const auto setupModulePath = PyUnicode_DecodeFSDefault("setup");
        const auto module = PyImport_Import(setupModulePath);

        checkError();

        if (setupModulePath)
        {
            Py_DecRef(setupModulePath);
        }

        if (module)
        {
            Py_DecRef(module);
        }
    }

    PyGILState_Ensure();
    Py_FinalizeEx();
}

void Interpreter::checkError() const
{
    PyObject* pType = nullptr;
    PyObject* pError = nullptr;
    PyObject* pTraceback = nullptr;
    PyErr_Fetch(&pType, &pError, &pTraceback);

    if (!pType)
    {
        return;
    }

    PyErr_NormalizeException(&pType, &pError, &pTraceback);

    std::string errorType = makeErrorTypeStr(pType);
    std::string shortMessage = makeShortMessageStr(pError);
    std::string longMessage = makeLongMessageStr(pTraceback);
    std::string file;

    Py_DecRef(pType);
    Py_DecRef(pError);
    Py_DecRef(pTraceback);

    throw InterpreterError(errorType, shortMessage, longMessage, file);
}

bool Interpreter::hasVenv() const
{
    return !m_venvDir.empty();
}

void Interpreter::initVenv()
{
    const auto pluginDir = stdutils::findDLLFileName().parent_path();
    const auto distribDir = pluginDir / "distrib";

    const auto scriptDir = m_scriptPath.parent_path();
    const auto venvDir = scriptDir / "venv";

    const auto pythonExecPath = distribDir / "python.exe";

    std::vector<std::wstring> pythonHome;

    pythonHome.emplace_back(scriptDir.wstring());
    pythonHome.emplace_back(distribDir / "DLLs");
    pythonHome.emplace_back(distribDir / "Lib");
    pythonHome.emplace_back(distribDir);
    pythonHome.emplace_back(venvDir);
    pythonHome.emplace_back(venvDir / "Lib" / "sites-packages");

    std::wstring pythonHomeStr;

    for (const auto& p : pythonHome)
    {
        if (!pythonHome.empty())
        {
            pythonHomeStr += L';';
        }

        pythonHomeStr += p;
    }

    PyConfig config = {0};
    PyConfig_InitPythonConfig(&config);
    config.isolated = 0;
    config.use_environment = 1;
    config.safe_path = 0;
    config.use_frozen_modules = 0;

    PyConfig_SetString(&config, &config.executable, pythonExecPath.c_str());
    PyConfig_SetString(&config, &config.exec_prefix, venvDir.c_str());

    PyConfig_SetString(&config, &config.base_executable, pythonExecPath.c_str());
    PyConfig_SetString(&config, &config.base_exec_prefix, distribDir.c_str());

    PyConfig_SetString(&config, &config.prefix, venvDir.c_str());
    PyConfig_SetString(&config, &config.pythonpath_env, pythonHomeStr.c_str());

    Py_InitializeFromConfig(&config);

    PyConfig_Clear(&config);
}
