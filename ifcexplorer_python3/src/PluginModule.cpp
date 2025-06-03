#include <Python.h>

#include "ModuleDocument.hpp"
#include "PluginState.hpp"

PluginState& getPluginState()
{
    return *static_cast<PluginState*>(PyCapsule_Import("plugin._C_API", 0));
}

PyObject* registerPlugin(PyObject* self, PyObject* args)
{
    auto& state = getPluginState();

    const char* name;
    const char* popup;
    PyObject* callable = nullptr;
    const auto r = PyArg_ParseTuple(args, "ssO", &name, &popup, &callable);

    if (r == 0)
    {
      // PyArg_ParseTuple raises an exception if parsing the tuple fails
      return nullptr;
    }

    if (state.featureName.empty())
    {
        state.pluginFeatures.emplace_back(PluginFeatures{ name, popup });
    }
    else if (state.featureName == name)
    {
        state.executed = true;
        const auto ret = PyObject_CallFunction(callable, nullptr);

        if (ret)
        {
            Py_DECREF(ret);
        }
        else
        {
            return nullptr; // propagate exception upwards
        }
    }

    return PyBool_FromLong(1);
}

static PyMethodDef PluginMethodDefinitions[] = {
        {"register", registerPlugin, METH_VARARGS,
                "Register a plugin."},
        {NULL, NULL, 0, NULL}
};

PyModuleDef PluginModuleDefinition = {
        PyModuleDef_HEAD_INIT, "plugin", NULL, -1, PluginMethodDefinitions,
        NULL, NULL, NULL, NULL
};