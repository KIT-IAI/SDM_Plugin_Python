#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"
#include <AppFeatureHelper.hpp>
#include <SettingsFeature.h>

namespace
{
  struct Document
  {
    // warning: should be PyObject_HEAD, may change with later python versions, ob_base can be initialized
    PyObject ob_base{ 0 };
    IfcDB::Populationi* db = nullptr;
    IfcDB::utils::PopulationStates* states = nullptr;
    sdm::plugin::AppFeature* appFeature = nullptr;
    sdm::plugin::SettingsInterface* settingsInterface = nullptr;
  };
}

Types initTypes(PyObject* m);
