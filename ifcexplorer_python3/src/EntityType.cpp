#include "EntityType.hpp"
#include "Relation.hpp"
#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();
extern PyObject* processIfcProperties(IfcDB::ModelInfo* pModelInfo, IfcDB::Property* pProperty);
extern IfcDB::IfcRelationType getRelationType(const std::tstring& relationType);

PyObject* getEntityTypeId(PyIfcdb::EntityType* entityType, void*)
{
  return PyLong_FromLongLong(entityType->entityType->getEntityId());
}

PyObject* getEntityTypeOid(PyIfcdb::EntityType* entityType, void*)
{
  return PyLong_FromLongLong(entityType->entityType->getOid());
}

PyObject* getEntityTypeGuid(PyIfcdb::EntityType* entityType, void*)
{
  return PyUnicode_FromString(toUtf8(entityType->entityType->getGuid()).c_str());
}

PyObject* getEntityTypeName(PyIfcdb::EntityType* entityType, void*)
{
  return PyUnicode_FromString(toUtf8(entityType->entityType->getName()).c_str());
}

PyObject* getEntityTypeDescription(PyIfcdb::EntityType* entityType, void*)
{
  return PyUnicode_FromString(toUtf8(entityType->entityType->getDescription()).c_str());
}

PyObject* getPropertiesEntityType(PyIfcdb::EntityType* entityType, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entityType->entityType->getModelInfo();

  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* dictObj = PyDict_New();

  for (auto pProperty : entityType->entityType->getPropertySets())
  {
    if (pProperty != nullptr)
    {
      PyObject* pyObj = processIfcProperties(pModelInfo, pProperty);

      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pProperty->getName()).c_str()), pyObj);
    }
  }

  return dictObj;
}

PyObject* getRelationsEntityType(PyIfcdb::EntityType* entityType, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entityType->entityType->getModelInfo();

  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    return Py_BuildValue("s", "");
  }

  PyObject* obj = nullptr;
  std::tstring relationType;

  const auto r = PyArg_ParseTuple(args, "|U", &obj);

  if (r == 0)
  {
    return nullptr;
  }

  if (obj != nullptr)
  {
    relationType = PyUnicode_AsWideCharString(obj, nullptr);
  }

  IfcDB::IfcRelationList relations;

  if (relationType.empty() == true)
  {
    pDB->getRelation(entityType->entityType->getOid(), relations);
  }
  else
  {
    pDB->getRelation(entityType->entityType->getOid(), getRelationType(relationType), relations);
  }

  if (relations.empty() == false)
  {
    auto& state = getIfcDBState();

    PyObject* listObj = PyList_New(0);

    for (auto pRelation : relations)
    {
      auto item = (PyIfcdb::Relation*)state.types.relation->tp_alloc(state.types.relation, 0);
      item->relation = pRelation;

      PyList_Append(listObj, (PyObject*)item);
    }

    return listObj;
  }

  Py_RETURN_NONE;
}

PyObject* getEntityTypeAttributes(PyIfcdb::EntityType* entityType, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entityType->entityType->getModelInfo();

  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* dictObj = PyDict_New();

  //PyDict_SetItem(dictObj, PyUnicode_FromString(""), PyUnicode_FromString(toUtf8().c_str()));



  return dictObj;
}
