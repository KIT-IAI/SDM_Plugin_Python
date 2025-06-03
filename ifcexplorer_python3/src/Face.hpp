#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"

namespace PyIfcdb
{
  struct Face
  {
    PyObject ob_base{ 0 };
    IfcDB::Face* face = nullptr;
  };
}
