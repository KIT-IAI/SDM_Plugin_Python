#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"

namespace PyIfcdb
{
  struct EntityType
  {
    PyObject ob_base{ 0 };
    IfcDB::IfcEntityType* entityType = nullptr;
  };
}
