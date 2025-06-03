#pragma once

#include <Python.h>

#include "Population.hpp"
#include "IfcDBState.hpp"

namespace PyIfcdb
{
  struct Relation
  {
    PyObject ob_base{ 0 };
    IfcDB::IfcEntityRelation* relation = nullptr;
  };
}
