#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"

namespace PyIfcdb
{
  struct Entity
  {
    PyObject ob_base{ 0 };
    IfcDB::IfcEntity* entity = nullptr;
  };
}
