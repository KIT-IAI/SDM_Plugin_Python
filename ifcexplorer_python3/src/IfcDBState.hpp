#pragma once

#include <Python.h>
#include <ifcdb/IfcDbInclude.h>
#include <ifcdb/utils/PopulationObserver.hpp>
#include <AppFeatureHelper.hpp>

struct Types
{
    PyTypeObject* document   = nullptr;
    PyTypeObject* entity     = nullptr;
    PyTypeObject* entityType = nullptr;
    PyTypeObject* relation   = nullptr;
    PyTypeObject* geometry   = nullptr;
    PyTypeObject* face       = nullptr;
    PyTypeObject* polyline   = nullptr;
};

struct IfcDBState
{
    IfcDB::Populationi* db = nullptr;
    IfcDB::utils::PopulationStates* states = nullptr;
    sdm::plugin::AppFeature* appFeature = nullptr;
    bool updateState = false;
    Types types;
};