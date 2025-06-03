#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"

namespace PyIfcdb
{
  struct Geometry
  {
    PyObject ob_base{ 0 };
    IfcDB::Geometry* geometry = nullptr;
  };
}
