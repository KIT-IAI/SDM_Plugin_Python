#include "ModuleDocument.hpp"
#include "Entity.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>
#include <stdutils/filesystem.hpp>
#include <stdutils/executable.hpp>

extern IfcDBState& getIfcDBState();
extern PyObject* getPyList(const IfcDB::Point& point);

PyObject* loadData(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* objFilename;
  const auto r = PyArg_ParseTuple(args, "U", &objFilename);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring filename = PyUnicode_AsWideCharString(objFilename, nullptr);

  document->appFeature->loadDocument(filename);

  Py_RETURN_NONE;
}

PyObject* getEntities(PyObject* self, void*)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::IfcEntityList entities;
  document->db->getAll(entities);

  if (entities.empty())
  {
    return PyList_New(0);
  }

  auto applicationState = getIfcDBState();

  const auto list = PyList_New(entities.size());

  for (std::size_t i = 0; i < entities.size(); ++i)
  {
    const auto entity = entities[i];

    auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
    item->entity = entity;

    PyList_SetItem(list, i, (PyObject*)item);
  }

  return list;
}

PyObject* getEntitiesByType(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
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

  IfcDB::IfcEntityList entities;
  document->db->getList(entityType, entities);

  if (entities.empty())
  {
    return PyList_New(0);
  }

  auto applicationState = getIfcDBState();

  const auto list = PyList_New(entities.size());

  for (std::size_t i = 0; i < entities.size(); ++i)
  {
    const auto entity = entities[i];

    auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
    item->entity = entity;

    PyList_SetItem(list, i, (PyObject*)item);
  }

  return list;
}

PyObject* getEntityByOid(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  IfcDB::ifcOid oid;
  const auto r = PyArg_ParseTuple(args, "L", &oid);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  auto pEntity = document->db->get(oid);

  if (pEntity == nullptr)
  {
    Py_RETURN_NONE;
  }

  auto applicationState = getIfcDBState();

  auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
  item->entity = pEntity;

  return (PyObject*)item;
}

PyObject* createFeature(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  auto applicationState = getIfcDBState();

  PyObject* objGmlId;
  PyObject* objClassName;
  const auto r = PyArg_ParseTuple(args, "UU", &objGmlId, &objClassName);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring gmlId = PyUnicode_AsWideCharString(objGmlId, nullptr);
  std::tstring className = PyUnicode_AsWideCharString(objClassName, nullptr);

  auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
  item->entity = document->db->createFeature(gmlId, className, 0);
  document->db->incrementSerialNo();

  applicationState.updateState = true;

  return (PyObject*)item;
}

PyObject* createEntity(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  auto applicationState = getIfcDBState();

  PyObject* objEntityType;
  const auto r = PyArg_ParseTuple(args, "U", &objEntityType);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  std::tstring entityType = PyUnicode_AsWideCharString(objEntityType, nullptr);

  IfcDB::DefaultFactory factory;
  IfcDB::IfcClass ifcClass = factory.getIfcDBClassType(toUtf8(entityType).c_str());

  auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
  item->entity = new IfcDB::IfcEntity();
  item->entity->createGuid();
  item->entity->setEntityType(entityType);
  item->entity->setType(ifcClass);

  document->db->add(item->entity);
  document->db->incrementSerialNo();

  applicationState.updateState = true;

  return (PyObject*)item;
}

PyObject* logMessage(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* obj = nullptr;

  const auto r = PyArg_ParseTuple(args, "O", &obj);

  if (r == 0)
  {
      // PyArg_ParseTuple raises an exception if parsing the tuple fails
      return nullptr;
  }

  if (obj != nullptr)
  {
    PyObject* objStr = PyObject_Str(obj);

    std::tstring messageStr = PyUnicode_AsWideCharString(objStr, nullptr);

    IfcDB::Message* pMessage = new IfcDB::Message(IfcDB::MT_COMMENT, _T("PythonMessage"), messageStr);

    document->db->getMessageLogger()->addMessage(pMessage);
  }

  Py_RETURN_NONE;
}

PyObject* srsTransform(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  /*
  point Coordinates, list of double
  Source SRS
  Target SRS
  */

  PyObject* objCoordinates;
  PyObject* objSourceSrs;
  PyObject* objTargetSrs;

  const auto r = PyArg_ParseTuple(args, "OUU", &objCoordinates, &objSourceSrs, &objTargetSrs);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  Py_ssize_t coordinates_size = PyList_Size(objCoordinates);

  if (coordinates_size <= 0)
  {
    return PyList_New(0);
  }

  IfcDB::SrsType sourceSrs = IfcDB::cSrsManager::getSrsType(PyUnicode_AsWideCharString(objSourceSrs, nullptr));
  IfcDB::SrsType targetSrs = IfcDB::cSrsManager::getSrsType(PyUnicode_AsWideCharString(objTargetSrs, nullptr));

  IfcDB::Point pnt;

  for (Py_ssize_t i = 0; i < coordinates_size; i++)
  {
    PyObject* coorinateObj = PyList_GetItem(objCoordinates, i);

    double coordinate = PyFloat_AsDouble(coorinateObj);

    switch (i)
    {
      case 0: pnt.x = coordinate; break;
      case 1: pnt.y = coordinate; pnt.dim = 2; break;
      case 2: pnt.z = coordinate; pnt.dim = 3; break;
    }
  }

  if(IfcDB::cSrsManager::convertSrs(sourceSrs, targetSrs, pnt, IfcDB::Point(), false) == true)
  {
    auto applicationState = getIfcDBState();

    const auto list = PyList_New(coordinates_size);

    for (std::size_t i = 0; i < coordinates_size; ++i)
    {
      double value(0.0);

      switch (i)
      {
        case 0: value = pnt.x; break;
        case 1: value = pnt.y; break;
        case 2: value = pnt.z; break;
      }

      PyList_SetItem(list, i, (PyObject*)PyFloat_FromDouble(value));
    }

    return list;
  }

  return PyList_New(0);
}

PyObject* getSelectedEntities(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  auto applicationState = getIfcDBState();

  IfcDB::tOidSet oids = applicationState.states->getSelectedEntities();

  if (oids.empty())
  {
    return PyList_New(0);
  }

  size_t i(0);
  const auto list = PyList_New(oids.size());

  for (auto oid : oids)
  {
    IfcDB::IfcEntity* entity = document->db->get(oid);

    auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
    item->entity = entity;

    PyList_SetItem(list, i++, (PyObject*)item);
  }

  return list;
}

PyObject* getVisibleEntities(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  auto applicationState = getIfcDBState();

  std::vector<IfcDB::utils::VisibleEntity> visibleEntities = applicationState.states->getVisibleEntities();

  if (visibleEntities.empty())
  {
    return PyList_New(0);
  }

  size_t i(0);
  const auto list = PyList_New(visibleEntities.size());

  for (auto visibleEntity : visibleEntities)
  {
    IfcDB::IfcEntity* entity = document->db->get(visibleEntity.oid);

    auto item = (PyIfcdb::Entity*)applicationState.types.entity->tp_alloc(applicationState.types.entity, 0);
    item->entity = entity;

    PyList_SetItem(list, i++, (PyObject*)item);
  }

  return list;
}

PyObject* getDataPath(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  return PyUnicode_FromString(toUtf8(document->db->getDataPath()).c_str());
}

PyObject* getScriptPath(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  auto executableDir = stdutils::findExecutableFileName().parent_path();

  if (auto scriptsDir = executableDir / "scripts"; exists(scriptsDir))
  {
    return PyUnicode_FromString(scriptsDir.string().c_str());
  }

  if (auto scriptsDir = executableDir.parent_path() / "scripts"; exists(scriptsDir))
  {
    return PyUnicode_FromString(scriptsDir.string().c_str());
  }

  if (auto scriptsDir = executableDir / "plugins" / "ifcexplorer_python3" / "scripts"; exists(scriptsDir))
  {
    return PyUnicode_FromString(scriptsDir.string().c_str());
  }

  Py_RETURN_NONE;
}

PyObject* getFilePath(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  std::filesystem::path path(std::filesystem::temp_directory_path());

  auto modelInfo = document->db->getModelInfo();

  if (!modelInfo.empty())
  {
    if (modelInfo.front()->m_ModelInfoType != IfcDB::ModelInfo::MT_WEBSERVICE && !modelInfo.front()->m_ModelLocation.empty())
    {
      std::filesystem::path modelLocation(modelInfo.front()->m_ModelLocation);
      modelLocation = modelLocation.parent_path();

      if (std::filesystem::exists(modelLocation))
      {
        path = modelLocation;
      }
    }
  }

  return PyUnicode_FromString(toUtf8(path.wstring()).c_str());
}

PyObject* getLatLonBoundingBox(PyObject* self, void*)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  if (document->db->getGeoReference().isLatLonInitialized())
  {
    PyObject* listObj = PyList_New(0);

    PyList_Append(listObj, getPyList(document->db->getGeoReference().m_LatLonBoundigBox.m_MinCoordinate));
    PyList_Append(listObj, getPyList(document->db->getGeoReference().m_LatLonBoundigBox.m_MaxCoordinate));

    return listObj;
  }

  Py_RETURN_NONE;
}

PyObject* getReferencePoint(PyObject* self, void*)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  if (document->db->getGeoReference().isLatLonInitialized())
  {
    return getPyList(document->db->getGeoReference().getRefPoint());
  }

  Py_RETURN_NONE;
}

PyObject* getDefaultSRS(PyObject* self, void*)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  if (document->db->getGeoReference().isLatLonInitialized())
  {
    return PyUnicode_FromString(toUtf8(document->db->getGeoReference().getDefaultSrsAsString()).c_str());
  }

  Py_RETURN_NONE;
}

PyObject* deleteEntity(PyObject* self, PyObject* args)
{
  const auto document = reinterpret_cast<Document*>(self);

  if (!document->db)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  auto applicationState = getIfcDBState();

  IfcDB::ifcOid oid;
  const auto r = PyArg_ParseTuple(args, "L", &oid);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  document->db->removeEntity(oid, true);

  document->states->compile();
  document->db->incrementSerialNo();

  Py_RETURN_NONE;
}