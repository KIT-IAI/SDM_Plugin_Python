#include "Entity.hpp"
#include "Geometry.hpp"
#include "Relation.hpp"
#include "EntityType.hpp"
#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();
extern PyObject* getPyList(const IfcDB::Point& point);
extern PyObject* processAttributes(IfcDB::IfcEntity* pEntity);
extern PyObject* processIfcProperties(IfcDB::ModelInfo* pModelInfo, IfcDB::Property* pProperty);
extern PyObject* processGmlProperties(IfcDB::Feature* pFeature);
extern IfcDB::IfcRelationType getRelationType(const std::tstring& relationType);

PyObject* getEntityId(PyIfcdb::Entity* entity, void*)
{
  return PyLong_FromLongLong(entity->entity->getEntityId());
}

PyObject* getEntityOid(PyIfcdb::Entity* entity, void*)
{
  return PyLong_FromLongLong(entity->entity->getOid());
}

PyObject* getEntityGuid(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(entity->entity->getGuid()).c_str());
}

PyObject* getEntityName(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(entity->entity->getName()).c_str());
}

int setEntityName(PyIfcdb::Entity* entity, PyObject* args, void*)
{
  entity->entity->setName(PyUnicode_AsWideCharString(args, nullptr));

  auto& state = getIfcDBState();
  state.updateState = true;

  return 0;
}

PyObject* getEntityDescription(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(entity->entity->getDescription()).c_str());
}

int setEntityDescription(PyIfcdb::Entity* entity, PyObject* args, void*)
{
  entity->entity->setDescription(PyUnicode_AsWideCharString(args, nullptr));

  auto& state = getIfcDBState();
  state.updateState = true;

  return 0;
}

PyObject* getEntityClassType(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(getTypeOfEntityAsString(entity->entity->getType())).c_str());
}

PyObject* getEntityType(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(entity->entity->getEntityType()).c_str());
}

PyObject* getEntityObjectType(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(entity->entity->getObjectType()).c_str());
}

int setEntityObjectType(PyIfcdb::Entity* entity, PyObject* args, void*)
{
  entity->entity->setObjectType(PyUnicode_AsWideCharString(args, nullptr));

  auto& state = getIfcDBState();
  state.updateState = true;

  return 0;
}

PyObject* getEntityLayer(PyIfcdb::Entity* entity, void*)
{
  return PyUnicode_FromString(toUtf8(entity->entity->getLayer().m_name).c_str());
}

int setEntityLayer(PyIfcdb::Entity* entity, PyObject* args, void*)
{
  entity->entity->setLayer(PyUnicode_AsWideCharString(args, nullptr));

  auto& state = getIfcDBState();
  state.updateState = true;

  return 0;
}

PyObject* getEntityColor(PyIfcdb::Entity* entity, void*)
{
  IfcDB::RGB_Color color = entity->entity->getColor();

  const auto colorList = PyList_New(4);

  PyList_SetItem(colorList, 0, (PyObject*)PyLong_FromLong(color.red));
  PyList_SetItem(colorList, 1, (PyObject*)PyLong_FromLong(color.green));
  PyList_SetItem(colorList, 2, (PyObject*)PyLong_FromLong(color.blue));
  PyList_SetItem(colorList, 3, (PyObject*)PyLong_FromLong(color.alpha));

  return colorList;
}

int setEntityColor(PyIfcdb::Entity* entity, PyObject* args, void*)
{
  IfcDB::RGB_Color color;

  Py_ssize_t colorList_size = PyList_Size(args);

  for (Py_ssize_t i = 0; i < colorList_size; i++)
  {
    PyObject* colorObject = PyList_GetItem(args, i);

    switch (i)
    {
      case 0: color.red = PyLong_AsLong(colorObject); break;
      case 1: color.green = PyLong_AsLong(colorObject); break;
      case 2: color.blue = PyLong_AsLong(colorObject); break;
      case 3: color.alpha = PyLong_AsLong(colorObject); break;
    }
  }

  entity->entity->setColor(color);

  auto& state = getIfcDBState();
  state.updateState = true;

  return 0;
}

PyObject* getEntityLongName(PyIfcdb::Entity* entity, void*)
{
  const auto extension = dynamic_cast<IfcDB::IfcSpaceExtension*>(entity->entity->getEntityExtension());

  if (extension)
  {
    return PyUnicode_FromString(toUtf8(extension->getLongName()).c_str());
  }

  Py_RETURN_NONE;
}

PyObject* getStringAttribute(PyIfcdb::Entity* entity, PyObject* args)
{
  std::tstring value;

  PyObject* obj;
  const auto r = PyArg_ParseTuple(args, "U", &obj);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring attributName = PyUnicode_AsWideCharString(obj, nullptr);

  IfcDB::Feature* pFeature = dynamic_cast<IfcDB::Feature*>(entity->entity);

  if (pFeature != nullptr)
  {
    std::tstring namespaceKuerzel(pFeature->getNamespaceCode() + L":");

    attributName = namespaceKuerzel + attributName;

    if (pFeature->getStringAttributWert(attributName, value))
    {
      return PyUnicode_FromString(toUtf8(value).c_str());
    }
  }
  else
  {
    if (entity->entity->getAttribute(attributName, value))
    {
      return PyUnicode_FromString(toUtf8(value).c_str());
    }
  }

  Py_RETURN_NONE;
}

PyObject* addGeometryFace(PyIfcdb::Entity* entity, PyObject* args)
{
  PyObject* pointList;
  const auto r = PyArg_ParseTuple(args, "O!", &PyList_Type, &pointList);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  Py_ssize_t pointList_size = PyList_Size(pointList);

  if (pointList_size > 0)
  {
    IfcDB::Face* pFace = new IfcDB::Face();
    IfcDB::Polyloop* pPolyloop = new IfcDB::Polyloop();
    pFace->addOuterLoop(pPolyloop, false);

    IfcDB::sRepresentation representation(L"PyGeometry", L"Face", pFace);

    entity->entity->addRepresentation(representation);

    for (Py_ssize_t i = 0; i < pointList_size; i++)
    {
      IfcDB::Point pnt;

      PyObject* coordinateList = PyList_GetItem(pointList, i);

      Py_ssize_t coordinateList_size = PyList_Size(coordinateList);

      for (Py_ssize_t i = 0; i < coordinateList_size; i++)
      {
        PyObject* coorinateObj = PyList_GetItem(coordinateList, i);

        double coordinate = PyFloat_AsDouble(coorinateObj);

        switch (i)
        {
          case 0: pnt.x = coordinate; break;
          case 1: pnt.y = coordinate; pnt.dim = 2; break;
          case 2: pnt.z = coordinate; pnt.dim = 3; break;
        }
      }

      pPolyloop->addPoint(pnt);
    }

    auto& state = getIfcDBState();
    state.states->registerEntityHash(entity->entity->getOid(), L"PyGeometry", L"Face", {});
    state.states->showRepresentationIdentifier(L"PyGeometry", true);
    state.updateState = true;
  }

  Py_RETURN_NONE;
}

PyObject* addGeometryPolyline(PyIfcdb::Entity* entity, PyObject* args)
{
  PyObject* pointList;
  const auto r = PyArg_ParseTuple(args, "O!", &PyList_Type, &pointList);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  Py_ssize_t pointList_size = PyList_Size(pointList);

  if (pointList_size > 0)
  {
    IfcDB::CPolyline* pPolyline = new IfcDB::CPolyline();

    IfcDB::sRepresentation representation(L"PyGeometry", L"Polyline", pPolyline);

    entity->entity->addRepresentation(representation);

    for (Py_ssize_t i = 0; i < pointList_size; i++)
    {
      IfcDB::Point pnt;

      PyObject* coordinateList = PyList_GetItem(pointList, i);

      Py_ssize_t coordinateList_size = PyList_Size(coordinateList);

      for (Py_ssize_t i = 0; i < coordinateList_size; i++)
      {
        PyObject* coorinateObj = PyList_GetItem(coordinateList, i);

        double coordinate = PyFloat_AsDouble(coorinateObj);

        switch (i)
        {
          case 0: pnt.x = coordinate; break;
          case 1: pnt.y = coordinate; pnt.dim = 2; break;
          case 2: pnt.z = coordinate; pnt.dim = 3; break;
        }
      }

      pPolyline->addPoint(pnt);
    }

    auto& state = getIfcDBState();
    state.states->registerEntityHash(entity->entity->getOid(), L"PyGeometry", L"Polyline", {});
    state.states->showRepresentationIdentifier(L"PyGeometry", true);
    state.updateState = true;
  }

  Py_RETURN_NONE;
}

PyObject* addStringAttribute(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::Feature* pFeature = dynamic_cast<IfcDB::Feature*>(entity->entity);

  if (!pFeature)
  {
    IfcDB::Message* pMessage = new IfcDB::Message(IfcDB::MT_COMMENT, L"PythonMessage", L"Unable to create string attribute: Feature instance required");

    pDB->getMessageLogger()->addMessage(pMessage);

    Py_RETURN_NONE;
  }

  PyObject* objPropertyName;
  PyObject* objPropertyValue;
  const auto r = PyArg_ParseTuple(args, "UU", &objPropertyName, &objPropertyValue);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring propertyName = PyUnicode_AsWideCharString(objPropertyName, nullptr);
  std::tstring propertyValue = PyUnicode_AsWideCharString(objPropertyValue, nullptr);

  pFeature->setStringAttribut(propertyName, propertyValue);

  Py_RETURN_NONE;
}

PyObject* addDateAttribute(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::Feature* pFeature = dynamic_cast<IfcDB::Feature*>(entity->entity);

  if (!pFeature)
  {
    IfcDB::Message* pMessage = new IfcDB::Message(IfcDB::MT_COMMENT, L"PythonMessage", L"Unable to create date attribute: Feature instance required");

    pDB->getMessageLogger()->addMessage(pMessage);

    Py_RETURN_NONE;
  }

  PyObject* objPropertyName;
  int propertyValueYear(0);
  int propertyValueMonth(0);
  int propertyValueDay(0);
  const auto r = PyArg_ParseTuple(args, "Uiii", &objPropertyName, &propertyValueYear, &propertyValueMonth, &propertyValueDay);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring propertyName = PyUnicode_AsWideCharString(objPropertyName, nullptr);

  IfcDB::TimePosition* pTime = new IfcDB::TimePosition(propertyValueYear, propertyValueMonth, propertyValueDay);

  pFeature->setDateAttribut(propertyName, pTime);

  Py_RETURN_NONE;
}

PyObject* calcQuantities(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::cEntityQuantities calculator(pDB, entity->entity);
  calculator.calcQuantities(entity->entity->getGeometry());
  auto& quantitiesMap = calculator.getQuantities();

  PyObject* dictObj = PyDict_New();
  for (auto& it : quantitiesMap)
  {
    const IfcDB::Quantity& quantity = it.second;

    PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(it.first).c_str()), PyUnicode_FromString(toUtf8(std::to_wstring(quantity.m_value)).c_str()));
  }

  return dictObj;
}

PyObject* setParent(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::ifcOid oidParent;
  const auto r = PyArg_ParseTuple(args, "L", &oidParent);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  entity->entity->setParentOid(oidParent);

  pDB->removeChildRelationship(entity->entity->getParentOid(), entity->entity->getOid());
  pDB->setChildRelationship(oidParent, entity->entity->getOid());
  pDB->incrementSerialNo();

  auto& state = getIfcDBState();
  state.updateState = true;

  Py_RETURN_NONE;
}

PyObject* findPropertyValue(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    return Py_BuildValue("s", "");
  }

  PyObject* objPropertyType;
  PyObject* objPropertyName;
  const auto r = PyArg_ParseTuple(args, "UU", &objPropertyType, &objPropertyName);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring propertyType = PyUnicode_AsWideCharString(objPropertyType, nullptr);
  std::tstring propertyName = PyUnicode_AsWideCharString(objPropertyName, nullptr);
  std::tstring propertyValue;

  if (pDB->findPropertyValue(entity->entity->getOid(), propertyType, propertyName, propertyValue) != nullptr)
  {
    return PyUnicode_FromString(toUtf8(propertyValue).c_str());
  }

  return Py_BuildValue("s", "");
}

PyObject* getGeometry(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    return Py_BuildValue("s", "");
  }

  IfcDB::Geometry* pGeometry = entity->entity->getGeometry();

  if (pGeometry != nullptr)
  {
    auto& state = getIfcDBState();

    auto item = (PyIfcdb::Geometry*)state.types.geometry->tp_alloc(state.types.geometry, 0);
    item->geometry = pGeometry;

    return (PyObject*)item;
  }

  Py_RETURN_NONE;
}

PyObject* getRelationsEntity(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
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
    pDB->getRelation(entity->entity->getOid(), relations);
  }
  else
  {
    pDB->getRelation(entity->entity->getOid(), getRelationType(relationType), relations);
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

PyObject* getTypeObject(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    return Py_BuildValue("s", "");
  }

  IfcDB::IfcRelationList relations;
  pDB->getRelation(entity->entity->getOid(), IfcDB::REL_DEFINES_BY_TYPE, relations);

  if (relations.size() == 1)
  {
    auto& state = getIfcDBState();

    IfcDB::IfcRelDefType* pRelDefType = dynamic_cast<IfcDB::IfcRelDefType*>(relations.front());

    auto item = (PyIfcdb::EntityType*)state.types.entityType->tp_alloc(state.types.entityType, 0);
    item->entityType = pRelDefType->getRelatingType();

    return (PyObject*)item;
  }

  Py_RETURN_NONE;
}

void processGeometry(IfcDB::Geometry* pGeometry, std::vector<std::tstring>& geometryStrings)
{
  if (pGeometry == nullptr)
  {
    return;
  }

  geometryStrings.emplace_back(IfcDB::getTypeOfGeometryAsString(pGeometry));

  switch (pGeometry->getType())
  {
    case IfcDB::SET_OF_GEOMETRY:
    case IfcDB::COMPOSITE_SURFACE:
    case IfcDB::MULTI_SURFACE:
    case IfcDB::SURFACE:
    {
      IfcDB::SetOfGeometry* pSetOfGeometry = dynamic_cast<IfcDB::SetOfGeometry*>(pGeometry);

      for (auto pGeometryPart : pSetOfGeometry->getGeometries())
      {
        processGeometry(pGeometryPart, geometryStrings);
      }
    }
    break;

    case IfcDB::MAPPED_GEOMETRY:
    {
      IfcDB::MappedGeometry* pMappedGeometry = dynamic_cast<IfcDB::MappedGeometry*>(pGeometry);

      processGeometry(pMappedGeometry->getGeometry(), geometryStrings);
    }
    break;

    //case IfcDB::IMPLICIT_GEOMETRY:
    //{
    //  IfcDB::ImplicitGeometry* pImplicitGeometry = dynamic_cast<IfcDB::ImplicitGeometry*>(pGeometry);

    //  assert(0);
    //}
    //break;

    case IfcDB::GEOMETRY_REFERENCE:
    {
      IfcDB::GeometryReference* pGeometryReference = dynamic_cast<IfcDB::GeometryReference*>(pGeometry);
      processGeometry(pGeometryReference->getReferencedGeometry(), geometryStrings);
    }
    break;

    case IfcDB::TRIANGULATED_FACESET:
    {
      IfcDB::TriangulatedFaceSet* pTriangulatedFaceSet = dynamic_cast<IfcDB::TriangulatedFaceSet*>(pGeometry);

      std::tstringstream triangesString;
      triangesString << pTriangulatedFaceSet->getTriangleIndices().size() << L" Trinagles";
      geometryStrings.emplace_back(triangesString.str());
    }
    break;

    case IfcDB::POLYGONAL_FACESET:
    {
      IfcDB::PolygonalFaceSet* pPolygonalFaceSet = dynamic_cast<IfcDB::PolygonalFaceSet*>(pGeometry);

      std::tstringstream facesString;
      facesString << pPolygonalFaceSet->getFaces().size() << L" Faces";
      geometryStrings.emplace_back(facesString.str());
    }
    break;

    case IfcDB::BREP:
    {
      IfcDB::Brep* pBrep = dynamic_cast<IfcDB::Brep*>(pGeometry);

      std::tstringstream facesString;
      facesString << pBrep->getFaces().size() << L" Faces";
      geometryStrings.emplace_back(facesString.str());
    }
    break;

    case  IfcDB::GML_SOLID:
    {
      IfcDB::GmlSolid* pGmlSolid = dynamic_cast<IfcDB::GmlSolid*>(pGeometry);

      processGeometry(pGmlSolid->getExterior(), geometryStrings);
    }
    break;

    case IfcDB::SURFACE_MODEL:
    {
      IfcDB::SurfaceModel* pSurfaceModel = dynamic_cast<IfcDB::SurfaceModel*>(pGeometry);

      std::tstringstream facesString;
      facesString << pSurfaceModel->getFaces().size() << L" Faces";
      geometryStrings.emplace_back(facesString.str());
    }
    break;

    default:
      ;
  }
}

PyObject* getGeometryInfo(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::Geometry* pGeometry = entity->entity->getGeometry();

  if (pGeometry != nullptr)
  {
    std::vector<std::tstring> geometryStrings;

    processGeometry(pGeometry, geometryStrings);

    PyObject* listObj = PyList_New(0);

    for (const auto& geometryString : geometryStrings)
    {
      PyList_Append(listObj, PyUnicode_FromString(toUtf8(geometryString).c_str()));
    }

    return listObj;
  }

  Py_RETURN_NONE;
}

PyObject* getModelType(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    return Py_BuildValue("s", "");
  }

  if (pModelInfo != nullptr)
  {
    return PyUnicode_FromString(toUtf8(IfcDB::ModelInfo::getModelInfoTypeAsString(pModelInfo->m_ModelInfoType)).c_str());
  }

  return Py_BuildValue("s", "");
}

PyObject* getModelInfo(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* dictObj = PyDict_New();

  PyDict_SetItem(dictObj, PyUnicode_FromString("DataType"), PyUnicode_FromString(toUtf8(pModelInfo->m_DataType).c_str()));
  PyDict_SetItem(dictObj, PyUnicode_FromString("ModelName"), PyUnicode_FromString(toUtf8(pModelInfo->m_ModelName).c_str()));
  PyDict_SetItem(dictObj, PyUnicode_FromString("ModelType"), PyUnicode_FromString(toUtf8(IfcDB::ModelInfo::getModelInfoTypeAsString(pModelInfo->m_ModelInfoType)).c_str()));
  PyDict_SetItem(dictObj, PyUnicode_FromString("ModelLocation"), PyUnicode_FromString(toUtf8(pModelInfo->m_ModelLocation).c_str()));

  return dictObj;
}

PyObject* getPropertiesEntity(PyIfcdb::Entity* entity, void*)
{
  IfcDB::Feature* pFeature = dynamic_cast<IfcDB::Feature*>(entity->entity);
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();

  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* dictObjAttributes = processAttributes(entity->entity);

  if (!pFeature)
  {
    PyObject* dictObjIfc = PyDict_New();

    IfcDB::IfcRelationList relations;
    pDB->getRelation(entity->entity->getOid(), IfcDB::REL_DEFINES_BY_PROPERTIES, relations);

    for (auto pRelation : relations)
    {
      IfcDB::IfcRelProperty* pRelProperty = dynamic_cast<IfcDB::IfcRelProperty*>(pRelation);

      IfcDB::Property* pProperty = pRelProperty->getRelatingProperty();

      if (pProperty != nullptr)
      {
        PyObject* pyObj = processIfcProperties(pModelInfo, pProperty);

        PyDict_SetItem(dictObjIfc, PyUnicode_FromString(toUtf8(pProperty->getName()).c_str()), pyObj);
      }
    }

    if (dictObjAttributes != Py_None && dictObjIfc != Py_None)
    {
      PyObject* dictObj = PyDict_New();

      PyDict_SetItem(dictObj, PyUnicode_FromString("Attributes"), dictObjAttributes);
      PyDict_SetItem(dictObj, PyUnicode_FromString("IFC Properties"), dictObjIfc);

      return dictObj;
    }

    if (dictObjIfc != Py_None)
    {
      return dictObjIfc;
    }
  }
  else
  {
    PyObject* dictObjGml = processGmlProperties(pFeature);

    if (dictObjAttributes != Py_None && dictObjGml != Py_None)
    {
      PyObject* dictObj = PyDict_New();

      PyDict_SetItem(dictObj, PyUnicode_FromString("Attributes"), dictObjAttributes);
      PyDict_SetItem(dictObj, PyUnicode_FromString("GML Properties"), dictObjGml);

      return dictObj;
    }

    if (dictObjGml != Py_None)
    {
      return dictObjGml;
    }
  }

  if (dictObjAttributes != Py_None)
  {
    return dictObjAttributes;
  }

  Py_RETURN_NONE;
}

PyObject* processIfcStructuralLoads(IfcDB::IfcStructuralLoad* pStructuralLoad)
{
  IfcDB::IfcStructuralLoadConfiguration* pConfiguration = nullptr;
  IfcDB::IfcStructuralLoadSingleForce* pSingleForce = nullptr;
  IfcDB::IfcStructuralLoadSingleForceWarping* pSingleForceWarping = nullptr;
  IfcDB::IfcStructuralLoadLinearForce* pLinearForce = nullptr;
  IfcDB::IfcStructuralLoadPlanarForce* pPlanarForce = nullptr;
  IfcDB::IfcStructuralLoadSingleDisplacement* pDisplacement = nullptr;
  IfcDB::IfcStructuralLoadSingleDisplacementDistortion* pDisplacementDistortion = nullptr;
  IfcDB::IfcStructuralLoadTemperature* pTemperature = nullptr;

  if (pConfiguration = dynamic_cast<IfcDB::IfcStructuralLoadConfiguration*>(pStructuralLoad))
  {
    if (pConfiguration->getValues().empty() == false)
    {
      (L"Load Configuration", pConfiguration->getName().c_str());

      for (const auto& pLoad : pConfiguration->getValues())
      {
        processIfcStructuralLoads(pLoad);
      }
    }

    if (pConfiguration->getLocations().empty() == false)
    {
      (L"Locations");

      for (const auto& location : pConfiguration->getLocations())
      {
        std::tstringstream locationStr;
        locationStr << L"[" << location.first << L"], [" << location.second << L"]";

        (L"Loaction", locationStr.str());
      }
    }
  }
  else if (pSingleForce = dynamic_cast<IfcDB::IfcStructuralLoadSingleForce*>(pStructuralLoad))
  {
    PyObject* dictObj = PyDict_New();

    PyDict_SetItem(dictObj, PyUnicode_FromString("LoadSingleForce"), PyUnicode_FromString(toUtf8(pSingleForce->getName()).c_str()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("ForceX"), PyFloat_FromDouble(pSingleForce->getForceX()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("ForceY"), PyFloat_FromDouble(pSingleForce->getForceY()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("ForceZ"), PyFloat_FromDouble(pSingleForce->getForceZ()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("MomentX"), PyFloat_FromDouble(pSingleForce->getMomentX()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("MomentY"), PyFloat_FromDouble(pSingleForce->getMomentY()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("MomentZ"), PyFloat_FromDouble(pSingleForce->getMomentZ()));

    return dictObj;
  }
  else if (pLinearForce = dynamic_cast<IfcDB::IfcStructuralLoadLinearForce*>(pStructuralLoad))
  {
    (L"Load Linear Force", pLinearForce->getName().c_str());
    (L"LinearForceX", std::to_string(pLinearForce->getLinearForceX()));
    (L"LinearForceY", std::to_string(pLinearForce->getLinearForceY()));
    (L"LinearForceZ", std::to_string(pLinearForce->getLinearForceZ()));
    (L"LinearMomentX", std::to_string(pLinearForce->getLinearMomentX()));
    (L"LinearMomentY", std::to_string(pLinearForce->getLinearMomentY()));
    (L"LinearMomentZ", std::to_string(pLinearForce->getLinearMomentZ()));
  }
  else if (pSingleForceWarping = dynamic_cast<IfcDB::IfcStructuralLoadSingleForceWarping*>(pStructuralLoad))
  {
    (L"Load Single Force Warping", pSingleForceWarping->getName().c_str());
    (L"ForceX", std::to_string(pSingleForceWarping->getForceX()));
    (L"ForceY", std::to_string(pSingleForceWarping->getForceY()));
    (L"ForceZ", std::to_string(pSingleForceWarping->getForceZ()));
    (L"MomentX", std::to_string(pSingleForceWarping->getMomentX()));
    (L"MomentY", std::to_string(pSingleForceWarping->getMomentY()));
    (L"MomentZ", std::to_string(pSingleForceWarping->getMomentZ()));
    (L"WarpingMoment", std::to_string(pSingleForceWarping->getWarpingMoment()));
  }
  else if (pPlanarForce = dynamic_cast<IfcDB::IfcStructuralLoadPlanarForce*>(pStructuralLoad))
  {
    (L"Load Planar Force", pPlanarForce->getName().c_str());
    (L"PlanarForceX", std::to_string(pPlanarForce->getPlanarForceX()));
    (L"PlanarForceY", std::to_string(pPlanarForce->getPlanarForceY()));
    (L"PlanarForceZ", std::to_string(pPlanarForce->getPlanarForceZ()));
  }
  else if (pDisplacement = dynamic_cast<IfcDB::IfcStructuralLoadSingleDisplacement*>(pStructuralLoad))
  {
    (L"Load Single Displacement", pDisplacement->getName().c_str());
    (L"DisplacementX", std::to_string(pDisplacement->getDisplacementX()));
    (L"DisplacementY", std::to_string(pDisplacement->getDisplacementY()));
    (L"DisplacementZ", std::to_string(pDisplacement->getDisplacementZ()));
    (L"RotationalDisplacementRX", std::to_string(pDisplacement->getRotationalDisplacementRX()));
    (L"RotationalDisplacementRY", std::to_string(pDisplacement->getRotationalDisplacementRY()));
    (L"RotationalDisplacementRZ", std::to_string(pDisplacement->getRotationalDisplacementRZ()));
  }
  else if (pDisplacementDistortion = dynamic_cast<IfcDB::IfcStructuralLoadSingleDisplacementDistortion*>(pStructuralLoad))
  {
    (L"Load Single Displacement Distortion", pDisplacementDistortion->getName().c_str());
    (L"DisplacementX", std::to_string(pDisplacementDistortion->getDisplacementX()));
    (L"DisplacementY", std::to_string(pDisplacementDistortion->getDisplacementY()));
    (L"DisplacementZ", std::to_string(pDisplacementDistortion->getDisplacementZ()));
    (L"RotationalDisplacementRX", std::to_string(pDisplacementDistortion->getRotationalDisplacementRX()));
    (L"RotationalDisplacementRY", std::to_string(pDisplacementDistortion->getRotationalDisplacementRY()));
    (L"RotationalDisplacementRZ", std::to_string(pDisplacementDistortion->getRotationalDisplacementRZ()));
    (L"Distortion", std::to_string(pDisplacementDistortion->getDistortion()));
  }
  else if (pTemperature = dynamic_cast<IfcDB::IfcStructuralLoadTemperature*>(pStructuralLoad))
  {
    (L"Load Temperature", pTemperature->getName().c_str());
    (L"DeltaT_Constant", std::to_string(pTemperature->getDeltaT_Constant()));
    (L"DeltaT_Y", std::to_string(pTemperature->getDeltaT_Y()));
    (L"DeltaT_Z", std::to_string(pTemperature->getDeltaT_Z()));
  }

  Py_RETURN_NONE;
}

PyObject* getEntityAttributes(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::IfcEntityExtension* pEntityExtension = entity->entity->getEntityExtension();

  if (pEntityExtension == nullptr)
  {
    Py_RETURN_NONE;
  }

  PyObject* dictObj = PyDict_New();

  //PyDict_SetItem(dictObj, PyUnicode_FromString(""), PyUnicode_FromString(toUtf8().c_str()));

  if (IfcDB::IfcProjectExtension* pProjectExtension = dynamic_cast<IfcDB::IfcProjectExtension*>(pEntityExtension))
  {
    PyDict_SetItem(dictObj, PyUnicode_FromString("LongName"), PyUnicode_FromString(toUtf8(pProjectExtension->getLongName()).c_str()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("Phase"), PyUnicode_FromString(toUtf8(pProjectExtension->getPhase()).c_str()));
  }
  else if (IfcDB::IfcSiteExtension* pSiteExtension = dynamic_cast<IfcDB::IfcSiteExtension*>(pEntityExtension))
  {
    PyDict_SetItem(dictObj, PyUnicode_FromString("LongName"), PyUnicode_FromString(toUtf8(pSiteExtension->getLongName()).c_str()));
    PyDict_SetItem(dictObj, PyUnicode_FromString("CompositionType"), PyUnicode_FromString(toUtf8(IfcDB::getCompositionTypeAsString(pSiteExtension->getCompositionType())).c_str()));

    if (pSiteExtension->isDefined_RefLatitude())
    {
      std::vector<int> latitudeList = pSiteExtension->getRefLatitude();
      PyDict_SetItem(dictObj, PyUnicode_FromString("RefLatitude"), PyUnicode_FromString(IfcDB::Geo_Utils::convertDMS2DMSString(latitudeList[0], latitudeList[1], latitudeList[2], (latitudeList.size() > 3) ? latitudeList[3] : 0, false).c_str()));
    }

    if (pSiteExtension->isDefined_RefLongitude())
    {
      std::vector<int> longitudeList = pSiteExtension->getRefLongitude();
      PyDict_SetItem(dictObj, PyUnicode_FromString("RefLongitude"), PyUnicode_FromString(IfcDB::Geo_Utils::convertDMS2DMSString(longitudeList[0], longitudeList[1], longitudeList[2], (longitudeList.size() > 3) ? longitudeList[3] : 0, true).c_str()));
    }

    if (pSiteExtension->getRefElevation().isValid)
    {
      PyDict_SetItem(dictObj, PyUnicode_FromString("RefElevation"), PyFloat_FromDouble(pSiteExtension->getRefElevation().value));
    }

    PyDict_SetItem(dictObj, PyUnicode_FromString("LandTitleNumber"), PyUnicode_FromString(toUtf8(pSiteExtension->getLandTitleNumber()).c_str()));

    // North direction from geometric representation context
    IfcDB::GeometricContext* pGeometricContext = pDB->getGeometricContext(entity->entity->getModelInfo(), L"Model");
    if (pGeometricContext != nullptr)
    {
      // y direction -> 0.0 degree
      double angle = (IfcDB::Rad2Deg * IfcDB::calcAngle2D(pGeometricContext->getTrueNorth())) - 90;
      PyDict_SetItem(dictObj, PyUnicode_FromString("North Direction (GeometricRepresentationContext: \"Model\")"), PyFloat_FromDouble(angle));

      if (pGeometricContext->getSrsData() != nullptr)
      {
        if (pGeometricContext->getSrsData()->getType() == IfcDB::EMapConversion)
        {
          IfcDB::MapConversion* pMapConversion = dynamic_cast<IfcDB::MapConversion*>(pGeometricContext->getSrsData());

          IfcDB::Point origin = pMapConversion->getOrigin();
          IfcDB::Direction northDirection = pMapConversion->getNorthDirection();
          double angle = IfcDB::Rad2Deg * calcAngle2D(northDirection);

          PyDict_SetItem(dictObj, PyUnicode_FromString("SRS Name"), PyUnicode_FromString(toUtf8(IfcDB::cSrsManager::getSrsTypeAsString(pMapConversion->getSrsType())).c_str()));
          PyDict_SetItem(dictObj, PyUnicode_FromString("Eastings"), PyFloat_FromDouble(origin.x));
          PyDict_SetItem(dictObj, PyUnicode_FromString("Northings"), PyFloat_FromDouble(origin.y));
          PyDict_SetItem(dictObj, PyUnicode_FromString("OrthogonalHeight"), PyFloat_FromDouble(origin.z));
          PyDict_SetItem(dictObj, PyUnicode_FromString("NorthDirection"), PyFloat_FromDouble(angle));
        }
        else if (pGeometricContext->getSrsData()->getType() == IfcDB::ERigidOperation)
        {
          IfcDB::RigidOperation* pRigidOperation = dynamic_cast<IfcDB::RigidOperation*>(pGeometricContext->getSrsData());

          PyDict_SetItem(dictObj, PyUnicode_FromString("SRS Name"), PyUnicode_FromString(toUtf8(IfcDB::cSrsManager::getSrsTypeAsString(pRigidOperation->getSrsType())).c_str()));
          PyDict_SetItem(dictObj, PyUnicode_FromString("FirstCoordinate"), PyFloat_FromDouble(pRigidOperation->getFirstCoordinate()));
          PyDict_SetItem(dictObj, PyUnicode_FromString("SecondCoordinate"), PyFloat_FromDouble(pRigidOperation->getSecondCoordinate()));
          PyDict_SetItem(dictObj, PyUnicode_FromString("Height"), PyFloat_FromDouble(pRigidOperation->getHeight()));
        }
      }
    }
  }
  else if (IfcDB::IfcBuildingExtension* pBuildingExtension = dynamic_cast<IfcDB::IfcBuildingExtension*>(pEntityExtension))
  {
    (L"LongName", pBuildingExtension->getLongName());
    (L"CompositionType", getCompositionTypeAsString(pBuildingExtension->getCompositionType()));
    (L"ElevationOfRefHeight", std::to_string(pBuildingExtension->getElevationOfRefHeight()));
    (L"ElevationOfTerrain", std::to_string(pBuildingExtension->getElevationOfTerrain()));
  }
  else if (IfcDB::IfcBuildingStoreyExtension* pBuildingStoreyExtension = dynamic_cast<IfcDB::IfcBuildingStoreyExtension*>(pEntityExtension))
  {
    (L"LongName", pBuildingStoreyExtension->getLongName());
    (L"CompositionType", getCompositionTypeAsString(pBuildingStoreyExtension->getCompositionType()));
    (L"Elevation", std::to_string(pBuildingStoreyExtension->getElevation()));
  }
  else if (IfcDB::IfcSpaceExtension* pSpaceExtension = dynamic_cast<IfcDB::IfcSpaceExtension*>(pEntityExtension))
  {
    (L"LongName", pSpaceExtension->getLongName());
    (L"CompositionType", getCompositionTypeAsString(pSpaceExtension->getCompositionType()));
    (L"ElevationWithFlooring", std::to_string(pSpaceExtension->getElevationWithFlooring()));

    //if (ifcSchema != IfcDB::IfcHeaderData::IFC_UNKNOWN && ifcSchema <= IfcDB::IfcHeaderData::IFC_2X3)
    //{
    //  (L"InteriorOrExteriorSpace", getInternalExternalAsString(pSpaceExtension->getInteriorOrExteriorSpace()));
    //}
  }
  else if (IfcDB::IfcDistributionPortExtension* pDistributionPort = dynamic_cast<IfcDB::IfcDistributionPortExtension*>(pEntityExtension))
  {
    (L"FlowDirection", pDistributionPort->getFlowDirection());
  }
  else if (IfcDB::IfcStructuralConnectionExtension* pStructuralConnection = dynamic_cast<IfcDB::IfcStructuralConnectionExtension*>(pEntityExtension))
  {
    if (pStructuralConnection->getAppliedCondition() != nullptr)
    {
      IfcDB::IfcBoundaryCondition* pAppliedCondition = pStructuralConnection->getAppliedCondition();

      if (pAppliedCondition && pAppliedCondition->getConditions().hasAttributes())
      {
        PyObject* conditionDict = PyDict_New();

        PyDict_SetItem(conditionDict, PyUnicode_FromString("Name"), PyUnicode_FromString(toUtf8(pAppliedCondition->getName()).c_str()));
        PyDict_SetItem(conditionDict, PyUnicode_FromString("Type"), PyUnicode_FromString(toUtf8(IfcDB::getTypeOfBoundaryConditionAsString(pAppliedCondition->getType())).c_str()));

        for (auto pAttribute : pAppliedCondition->getConditions().getAttributes())
        {
          PyDict_SetItem(conditionDict, PyUnicode_FromString(toUtf8(pAttribute->getKey()).c_str()), PyUnicode_FromString(toUtf8(pAttribute->getValue()).c_str()));
        }

        PyDict_SetItem(dictObj, PyUnicode_FromString("AppliedCondition"), conditionDict);
      }
    }
  }
  else if (IfcDB::IfcStructuralActivityExtension* pStructuralActivity = dynamic_cast<IfcDB::IfcStructuralActivityExtension*>(pEntityExtension))
  {
    if (pStructuralActivity->getAppliedLoad() != nullptr)
    {
      PyObject* appliedLoad = processIfcStructuralLoads(pStructuralActivity->getAppliedLoad());
      PyDict_SetItem(dictObj, PyUnicode_FromString("AppliedLoad"), appliedLoad);
    }

    (L"GlobalOrLocal", pStructuralActivity->getGlobalOrLocal().c_str());
  }
  else if (IfcDB::IfcWallExtension* pWallExtension = dynamic_cast<IfcDB::IfcWallExtension*>(pEntityExtension))
  {
    (L"Wall Extension");
    (L"Height", std::to_string(pWallExtension->getHeight()));
    (L"Length", std::to_string(pWallExtension->getLength()));
    (L"Width", std::to_string(pWallExtension->getWidth()));
  }
  else if (IfcDB::IfcDoorExtension* pDoorExtension = dynamic_cast<IfcDB::IfcDoorExtension*>(pEntityExtension))
  {
    (L"OverallHeight", std::to_string(pDoorExtension->getOverallHeight()));
    (L"OverallWidth", std::to_string(pDoorExtension->getOverallWidth()));
  }
  else if (IfcDB::IfcWindowExtension* pWindowExtension = dynamic_cast<IfcDB::IfcWindowExtension*>(pEntityExtension))
  {
    (L"OverallHeight", std::to_string(pWindowExtension->getOverallHeight()));
    (L"OverallWidth", std::to_string(pWindowExtension->getOverallWidth()));
  }
  else if (IfcDB::IfcTendonExtension* pTendonExtension = dynamic_cast<IfcDB::IfcTendonExtension*>(pEntityExtension))
  {
    (L"SteelGrade", pTendonExtension->getSteelGrade());
    (L"PredefinedType", pTendonExtension->getPredefinedType());
    (L"NominalDiameter", std::to_string(pTendonExtension->getNominalDiameter()));
    (L"CrossSectionArea", std::to_string(pTendonExtension->getCrossSectionArea()));
    (L"TensionForce", std::to_string(pTendonExtension->getTensionForce()));
    (L"PreStress", std::to_string(pTendonExtension->getPreStress()));
    (L"FrictionCoefficient", std::to_string(pTendonExtension->getFrictionCoefficient()));
    (L"AnchorageSlip", std::to_string(pTendonExtension->getAnchorageSlip()));
    (L"MinCurvatureRadius", std::to_string(pTendonExtension->getMinCurvatureRadius()));
  }
  else if (IfcDB::IfcTendonAnchorExtension* pTendonAnchorExtension = dynamic_cast<IfcDB::IfcTendonAnchorExtension*>(pEntityExtension))
  {
    (L"SteelGrade", pTendonAnchorExtension->getSteelGrade());
  }
  else if (IfcDB::IfcReinforcingBarExtension* pReinforcingBarExtension = dynamic_cast<IfcDB::IfcReinforcingBarExtension*>(pEntityExtension))
  {
    (L"SteelGrade", pReinforcingBarExtension->getSteelGrade());
    (L"NominalDiameter", std::to_string(pReinforcingBarExtension->getNominalDiameter()));
    (L"CrossSectionArea", std::to_string(pReinforcingBarExtension->getCrossSectionArea()));
    (L"BarLength", std::to_string(pReinforcingBarExtension->getBarLength()));
    (L"BarRole", pReinforcingBarExtension->getBarRole());
    (L"BarSurface", pReinforcingBarExtension->getBarSurface());
  }
  else if (IfcDB::IfcReinforcingMeshExtension* pReinforcingMeshExtension = dynamic_cast<IfcDB::IfcReinforcingMeshExtension*>(pEntityExtension))
  {
    (L"SteelGrade", pReinforcingMeshExtension->getSteelGrade());
    (L"MeshLength", std::to_string(pReinforcingMeshExtension->getMeshLength()));
    (L"MeshWidth", std::to_string(pReinforcingMeshExtension->getMeshWidth()));
    (L"LongitudinalBarNominalDiameter", std::to_string(pReinforcingMeshExtension->getLongitudinalBarNominalDiameter()));
    (L"TransverseBarNominalDiameter", std::to_string(pReinforcingMeshExtension->getTransverseBarNominalDiameter()));
    (L"LongitudinalBarCrossSectionArea", std::to_string(pReinforcingMeshExtension->getLongitudinalBarCrossSectionArea()));
    (L"TransverseBarCrossSectionArea", std::to_string(pReinforcingMeshExtension->getTransverseBarCrossSectionArea()));
    (L"LongitudinalBarSpacing", std::to_string(pReinforcingMeshExtension->getLongitudinalBarSpacing()));
    (L"TransverseBarSpacing", std::to_string(pReinforcingMeshExtension->getTransverseBarSpacing()));
  }
  else if (IfcDB::IfcWorkPlanExtension* pWorkPlanExtension = dynamic_cast<IfcDB::IfcWorkPlanExtension*>(pEntityExtension))
  {
    (L"Identification", pWorkPlanExtension->getIdentification());
    (L"CreationDate", pWorkPlanExtension->getCreationDate());
    (L"Creators", pWorkPlanExtension->getCreators());
    (L"Purpose", pWorkPlanExtension->getPurpose());
    (L"Duration", pWorkPlanExtension->getDuration());
    (L"TotalFloat", pWorkPlanExtension->getTotalFloat());
    (L"StartTime", pWorkPlanExtension->getStartTime());
    (L"FinishTime", pWorkPlanExtension->getFinishTime());
  }
  else if (IfcDB::IfcTaskExtension* pTaskExtension = dynamic_cast<IfcDB::IfcTaskExtension*>(pEntityExtension))
  {
    (L"Identification", pTaskExtension->getIdentification());
    (L"LongDescription", pTaskExtension->getLongDescription());
    (L"Status", pTaskExtension->getStatus());
    (L"WorkMethod", pTaskExtension->getWorkMethod());
    (L"IsMilestone", (pTaskExtension->getIsMilestone() == true) ? L"True" : L"False");
    (L"Priority", std::to_string(pTaskExtension->getPriority()));

    IfcDB::IfcTaskExtension::IfcTaskTIme* pTaskTime = pTaskExtension->getTaskTime();

    if (pTaskTime != nullptr)
    {
      (L"TaskTime");

      (L"Name", pTaskTime->m_Name);
      (L"DataOrigin", pTaskTime->m_DataOrigin);
      (L"UserDefinedDataOrigin", pTaskTime->m_UserDefinedDataOrigin);
      (L"DurationType", pTaskTime->m_DurationType);
      (L"ScheduleDuration", pTaskTime->m_ScheduleDuration);
      (L"ScheduleStart", pTaskTime->m_ScheduleStart);
      (L"ScheduleFinish", pTaskTime->m_ScheduleFinish);
      (L"EarlyStart", pTaskTime->m_EarlyStart);
      (L"EarlyFinish", pTaskTime->m_EarlyFinish);
      (L"LateStart", pTaskTime->m_LateStart);
      (L"LateFinish", pTaskTime->m_LateFinish);
      (L"FreeFloat", pTaskTime->m_FreeFloat);
      (L"TotalFloat", pTaskTime->m_TotalFloat);
      (L"IsCritical", (pTaskTime->m_IsCritical == true) ? L"True" : L"False");
      (L"StatusTime", pTaskTime->m_StatusTime);
      (L"ActualDuration", pTaskTime->m_ActualDuration);
      (L"ActualStart", pTaskTime->m_ActualStart);
      (L"ActualFinish", pTaskTime->m_ActualFinish);
      (L"RemainingTime", pTaskTime->m_RemainingTime);
      (L"Completion", pTaskTime->m_Completion);
      (L"Recurrence", pTaskTime->m_Recurrence);
    }
  }
  else if (IfcDB::IfcCostItemExtension* pCostItemExtension = dynamic_cast<IfcDB::IfcCostItemExtension*>(pEntityExtension))
  {
    (L"Cost Values");

    for (auto pCostValue : pCostItemExtension->getCostValues())
    {
      (L"CostValue");

      (L"Name", pCostValue->m_Name);
      (L"Description", pCostValue->m_Description);
      (L"AppliedValue", pCostValue->m_AppliedValue);
      (L"UnitBasis", pCostValue->m_UnitBasis);
      (L"ApplicableDate", pCostValue->m_ApplicableDate);
      (L"FixedUntilDate", pCostValue->m_FixedUntilDate);
      (L"Category", pCostValue->m_Category);
      (L"Condition", pCostValue->m_Condition);
      (L"ArithmeticOperator", pCostValue->m_ArithmeticOperator);
      (L"Components", pCostValue->m_Components);
    }

    (L"Cost Quantities");

    for (auto pCostQuantity : pCostItemExtension->getCostQuantities())
    {
      //pCostQuantity;
    }
  }
/*
  else if (IfcDB::IfcAlignmentHorizontalExtension* pAlignmentExtension = dynamic_cast<IfcDB::IfcAlignmentHorizontalExtension*>(pEntityExtension))
  {
    tempCString.Format(L"%d", pAlignmentExtension->getStartDistAlong());
    cPropertiesTree::saveToTree(pReqP, 2, -1, 0, 2, L"StartDistAlong", tempCString);

    if (pAlignmentExtension->getSegments().empty() == false)
    {
      TCHAR buffer[20];
      _itot_s((int)pAlignmentExtension->getSegments().size(), buffer, 20, 10);
      cPropertiesTree::saveToTree(pReqP, 2, -1, 0, 2, L"IfcAlignmentHorizontalSegments", buffer);

      for (auto pSegment : pAlignmentExtension->getSegments())
      {
        (L"IfcAlignmentHorizontalSegment");
        (L"StartTag", pSegment->getStartTag());
        (L"EndTag", pSegment->getEndTag());

        std::tstringstream startPointStr;
        IfcDB::Point startPoint = pSegment->getStartPoint();
        startPointStr << std::fixed << startPoint.x << L", " << startPoint.y << L", " << startPoint.z;
        (L"StartPoint", startPointStr.str());

        std::to_string(pSegment->getStartDirection());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartDirection", tempCString);
        std::to_string(pSegment->getStartRadiusOfCurvature());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartRadiusOfCurvature", tempCString);
        std::to_string(pSegment->getEndRadiusOfCurvature());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"EndRadiusOfCurvature", tempCString);
        std::to_string(pSegment->getSegmentLength());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"SegmentLength", tempCString);
        std::to_string(pSegment->getGravityCenterLineHeight());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"m_GravityCenterLineHeight", tempCString);
        (L"PredefinedType", pSegment->getPredefinedType());
      }
    }
  }
  else if (IfcDB::IfcAlignmentVerticalExtension* pAlignmentExtension = dynamic_cast<IfcDB::IfcAlignmentVerticalExtension*>(pEntityExtension))
  {
    if (pAlignmentExtension->getSegments().empty() == false)
    {
      TCHAR buffer[20];
      _itot_s((int)pAlignmentExtension->getSegments().size(), buffer, 20, 10);
      cPropertiesTree::saveToTree(pReqP, 2, -1, 0, 2, L"IfcAlignmentVerticalSegments", buffer);

      for (auto pSegment : pAlignmentExtension->getSegments())
      {
        (L"IfcAlignmentVerticalSegment");
        (L"StartTag", pSegment->getStartTag());
        (L"EndTag", pSegment->getEndTag());
        std::to_string(pSegment->getStartDistAlong());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartDistAlong", tempCString);
        std::to_string(pSegment->getHorizontalLength());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"HorizontalLength", tempCString);
        std::to_string(pSegment->getStartHeight());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartHeight", tempCString);
        std::to_string(pSegment->getStartGradient());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartGradient", tempCString);
        std::to_string(pSegment->getEndGradient());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"EndGradient", tempCString);
        std::to_string(pSegment->getRadiusOfCurvature());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"RadiusOfCurvature", tempCString);
        (L"PredefinedType", pSegment->getPredefinedType());
      }
    }
  }
  else if (IfcDB::IfcAlignmentCantExtension* pAlignmentExtension = dynamic_cast<IfcDB::IfcAlignmentCantExtension*>(pEntityExtension))
  {
    if (pAlignmentExtension->getSegments().empty() == false)
    {
      TCHAR buffer[20];
      _itot_s((int)pAlignmentExtension->getSegments().size(), buffer, 20, 10);
      cPropertiesTree::saveToTree(pReqP, 2, -1, 0, 2, L"IfcAlignmentCantSegments", buffer);

      for (auto pSegment : pAlignmentExtension->getSegments())
      {
        (L"IfcAlignmentCantSegment");
        (L"StartTag", pSegment->getStartTag());
        (L"EndTag", pSegment->getEndTag());
        std::to_string(pSegment->getStartDistAlong());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartDistAlong", tempCString);
        std::to_string(pSegment->getHorizontalLength());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"HorizontalLength", tempCString);
        std::to_string(pSegment->getStartCantLeft());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartCantLeft", tempCString);
        std::to_string(pSegment->getEndCantLeft());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"EndCantLeft", tempCString);
        std::to_string(pSegment->getStartCantRight());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"StartCantRight", tempCString);
        std::to_string(pSegment->getEndCantRight());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"EndCantRight", tempCString);
        std::to_string(pSegment->getSmoothingLength());
        cPropertiesTree::saveToTree(pReqP, 4, -1, 0, 2, L"SmoothingLength", tempCString);
        (L"PredefinedType", pSegment->getPredefinedType());
      }
    }
  }
*/

  return dictObj;
}

PyObject* isPointWithin(PyIfcdb::Entity* entity, PyObject* args)
{
  PyObject* objPoint;

  const auto r = PyArg_ParseTuple(args, "O", &objPoint);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  Py_ssize_t coordinates_size = PyList_Size(objPoint);

  if (coordinates_size <= 0)
  {
    Py_RETURN_FALSE;
  }

  IfcDB::Point pnt;

  for (Py_ssize_t i = 0; i < coordinates_size; i++)
  {
    PyObject* coorinateObj = PyList_GetItem(objPoint, i);

    double coordinate = PyFloat_AsDouble(coorinateObj);

    switch (i)
    {
      case 0: pnt.x = coordinate; break;
      case 1: pnt.y = coordinate; pnt.dim = 2; break;
      case 2: pnt.z = coordinate; pnt.dim = 3; break;
    }
  }

  IfcDB::Geometry* pGeometry = entity->entity->getGeometry();

  if (pGeometry)
  {
    // Matrix for projection
    IfcDB::Matrix4 matrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    IfcDB::ClipperCurve footprint(matrix, (IfcDB::Curve*)nullptr);

    std::vector<IfcDB::Face*> vFaces;
    IfcDB::Geometry::getFaces(pGeometry, vFaces);

    for (auto pFace : vFaces)
    {
      IfcDB::ClipperCurve curveElement(matrix, pFace);

      if (footprint.isEmpty())
      {
        footprint = std::move(curveElement);
      }
      else
      {
        footprint.unite(&curveElement);
      }
    }

    IfcDB::Polylines polylines = footprint.toPolylines();

    bool isInside(false);

    for (auto pPolyline : polylines)
    {
      isInside |= pPolyline->Point2DIn(pnt);

      delete pPolyline;
    }

    if (isInside)
    {
      Py_RETURN_TRUE;
    }
  }

  Py_RETURN_FALSE;
}

PyObject* getEntityBoundingBox(PyIfcdb::Entity* entity, void*)
{
  PyObject* objPoint;

  IfcDB::Point min, max;

  if (entity->entity->getBoundingBox(min, max))
  {
    PyObject* listObj = PyList_New(0);

    PyList_Append(listObj, getPyList(min));
    PyList_Append(listObj, getPyList(max));

    return listObj;
  }

  Py_RETURN_NONE;
}

PyObject* getEntityChildren(PyIfcdb::Entity* entity, void*)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::IfcEntityList children;
  pDB->getAllChildren(entity->entity->getOid(), children);

  if (children.empty())
  {
    return PyList_New(0);
  }

  auto applicationState = getIfcDBState();

  const auto list = PyList_New(children.size());

  for (std::size_t i = 0; i < children.size(); ++i)
  {
    const auto entity = children[i];

    auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
    item->entity = entity;

    PyList_SetItem(list, i, (PyObject*)item);
  }

  return list;
}

PyObject* getEntityChildrenByType(PyIfcdb::Entity* entity, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = entity->entity->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* objClassName;
  const auto r = PyArg_ParseTuple(args, "U", &objClassName);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring entityType = PyUnicode_AsWideCharString(objClassName, nullptr);

  IfcDB::IfcEntityList childern;
  pDB->getChildren(entityType, entity->entity->getOid(), childern);

  if (childern.empty())
  {
    return PyList_New(0);
  }

  auto applicationState = getIfcDBState();

  const auto list = PyList_New(childern.size());

  for (std::size_t i = 0; i < childern.size(); ++i)
  {
    const auto entity = childern[i];

    auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
    item->entity = entity;

    PyList_SetItem(list, i, (PyObject*)item);
  }

  return list;
}