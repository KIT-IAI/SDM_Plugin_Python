#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"

namespace PyIfcdb
{
  struct Polyline
  {
    PyObject ob_base{ 0 };
    IfcDB::Curve* curve = nullptr;
  };
}
