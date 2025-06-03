#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <ifcdb/utils/PopulationObserver.hpp>
#include "Entity.hpp"
#include "EntityType.hpp"
#include "Relation.hpp"
#include "Geometry.hpp"
#include "Face.hpp"
#include "Polyline.hpp"
#include "Interpreter.hpp"
#include <stdutils/utf8.hpp>

/**
 * Document
 */
extern PyObject* loadData(PyObject* self, PyObject* args);
extern PyObject* getEntities(PyObject* self, void*);
extern PyObject* getEntitiesByType(PyObject* self, PyObject* args);
extern PyObject* getEntityByOid(PyObject* self, PyObject* args);
extern PyObject* createFeature(PyObject* self, PyObject* args);
extern PyObject* createEntity(PyObject* self, PyObject* args);
extern PyObject* logMessage(PyObject* self, PyObject* args);
extern PyObject* srsTransform(PyObject* self, PyObject* args);
extern PyObject* getSelectedEntities(PyObject* self, PyObject* args);
extern PyObject* getVisibleEntities(PyObject* self, PyObject* args);
extern PyObject* getDataPath(PyObject* self, PyObject* args);
extern PyObject* getScriptPath(PyObject* self, PyObject* args);
extern PyObject* getFilePath(PyObject* self, PyObject* args);
extern PyObject* getLatLonBoundingBox(PyObject* self, void*);
extern PyObject* getReferencePoint(PyObject* self, void*);
extern PyObject* getDefaultSRS(PyObject* self, void*);
extern PyObject* deleteEntity(PyObject* self, PyObject* args);

/**
 * Entity
 */
extern PyObject* getEntityId(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityOid(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityGuid(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityName(PyIfcdb::Entity* entity, void*);
extern int       setEntityName(PyIfcdb::Entity* entity, PyObject* args, void*);
extern PyObject* getEntityDescription(PyIfcdb::Entity* entity, void*);
extern int       setEntityDescription(PyIfcdb::Entity* entity, PyObject* args, void*);
extern PyObject* getEntityClassType(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityType(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityObjectType(PyIfcdb::Entity* entity, void*);
extern int       setEntityObjectType(PyIfcdb::Entity* entity, PyObject* args, void*);
extern PyObject* getEntityLayer(PyIfcdb::Entity* entity, void*);
extern int       setEntityLayer(PyIfcdb::Entity* entity, PyObject* args, void*);
extern PyObject* getEntityColor(PyIfcdb::Entity* entity, void*);
extern int       setEntityColor(PyIfcdb::Entity* entity, PyObject* args, void*);
extern PyObject* getEntityAttributes(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityLongName(PyIfcdb::Entity* entity, void*);
extern PyObject* getStringAttribute(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* addGeometryFace(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* addGeometryPolyline(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* addStringAttribute(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* addDateAttribute(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* calcQuantities(PyIfcdb::Entity* entity, void*);
extern PyObject* setParent(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* findPropertyValue(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* getGeometry(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* getRelationsEntity(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* getTypeObject(PyIfcdb::Entity* entity, void*);
extern PyObject* getGeometryInfo(PyIfcdb::Entity* entity, void*);
extern PyObject* getModelType(PyIfcdb::Entity* entity, void*);
extern PyObject* getModelInfo(PyIfcdb::Entity* entity, void*);
extern PyObject* getPropertiesEntity(PyIfcdb::Entity* entity, void*);
extern PyObject* isPointWithin(PyIfcdb::Entity* entity, PyObject* args);
extern PyObject* getEntityBoundingBox(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityChildren(PyIfcdb::Entity* entity, void*);
extern PyObject* getEntityChildrenByType(PyIfcdb::Entity* entity, PyObject* args);

/**
 * EntityType
 */
extern PyObject* getEntityTypeId(PyIfcdb::EntityType* entityType, void*);
extern PyObject* getEntityTypeOid(PyIfcdb::EntityType* entityType, void*);
extern PyObject* getEntityTypeGuid(PyIfcdb::EntityType* entityType, void*);
extern PyObject* getEntityTypeName(PyIfcdb::EntityType* entityType, void*);
extern PyObject* getEntityTypeDescription(PyIfcdb::EntityType* entityType, void*);
extern PyObject* getEntityTypeAttributes(PyIfcdb::EntityType* entityType, PyObject* args);
extern PyObject* getPropertiesEntityType(PyIfcdb::EntityType* entityType, PyObject* args);
extern PyObject* getRelationsEntityType(PyIfcdb::EntityType* entityType, PyObject* args);

/**
 * Relation
 */
extern PyObject* getRelationId(PyIfcdb::Relation* relation, void*);
extern PyObject* getRelationOid(PyIfcdb::Relation* relation, void*);
extern PyObject* getRelationGuid(PyIfcdb::Relation* relation, void*);
extern PyObject* getRelationName(PyIfcdb::Relation* relation, void*);
extern PyObject* getRelationDescription(PyIfcdb::Relation* relation, void*);
extern PyObject* getRelationType(PyIfcdb::Relation* relation, void*);
extern PyObject* getRelationAttributes(PyIfcdb::Relation* relation, PyObject* args);

 /**
  * Geometry
  */
extern PyObject* getGeometryType(PyIfcdb::Geometry* geometry, void*);
extern PyObject* getGeometryData(PyIfcdb::Geometry* geometry, void*);
extern PyObject* geometrySrsTransform(PyIfcdb::Geometry* geometry, PyObject* args);
extern PyObject* geometryIsWithin(PyIfcdb::Geometry* geometry, PyObject* args);
extern PyObject* geometryFaces(PyIfcdb::Geometry* geometry, void*);
extern PyObject* getGeometryBoundingBox(PyIfcdb::Geometry* geometry, void*);

/**
 * Face
 */
extern PyObject* faceIsWithin(PyIfcdb::Face* face, PyObject* args);
extern PyObject* faceNormal(PyIfcdb::Face* face, void*);
extern PyObject* faceCalcArea(PyIfcdb::Face* face, void*);
extern PyObject* faceGetPolygons(PyIfcdb::Face* face, void*);
extern PyObject* faceGetOuterLoop(PyIfcdb::Face* face, void*);
extern PyObject* faceGetOuterLoopCoordinates(PyIfcdb::Face* face, void*);
extern PyObject* faceGetInnerLoops(PyIfcdb::Face* face, void*);
extern PyObject* faceGetInnerLoopsCoordinates(PyIfcdb::Face* face, void*);

/**
 * Polyline
 */
extern PyObject* polylineIsWithin(PyIfcdb::Polyline* polyline, PyObject* args);
extern PyObject* polylineIntersects(PyIfcdb::Polyline* polyline, PyObject* args);
extern PyObject* polylineIsClosed(PyIfcdb::Polyline* polyline, void*);
extern PyObject* polylineGetCoordinates(PyIfcdb::Polyline* polyline, void*);

Types initTypes(PyObject* m)
{
  /**
   * Document
   */
  static PyMethodDef documentMethods[] =
  {
    { "load_data", (PyCFunction)loadData, METH_VARARGS, "load data from a file (if the file format is supported)" },
    { "get_entities", (PyCFunction)getEntities, METH_NOARGS, "return all entities" },
    { "get_entities_by_type", (PyCFunction)getEntitiesByType, METH_VARARGS, "return all entities of the specified type" },
    { "get_entity_by_oid", (PyCFunction)getEntityByOid, METH_VARARGS, "return entity with the given OID" },
    { "create_feature", (PyCFunction)createFeature, METH_VARARGS, "create a new feature" },
    { "create_entity", (PyCFunction)createEntity, METH_VARARGS, "create a new entity" },
    { "log_message", (PyCFunction)logMessage, METH_VARARGS, "save message to message log" },
    { "srs_transform", (PyCFunction)srsTransform, METH_VARARGS, "SRS transformation of a point coordinate" },
    { "get_selected_entities", (PyCFunction)getSelectedEntities, METH_NOARGS, "return selected entities" },
    { "get_visible_entities", (PyCFunction)getVisibleEntities, METH_NOARGS, "return visible entities" },
    { "get_data_path", (PyCFunction)getDataPath, METH_NOARGS, "path of the application data folder" },
    { "get_script_path", (PyCFunction)getScriptPath, METH_NOARGS, "path of the current python script" },
    { "get_file_path", (PyCFunction)getFilePath, METH_NOARGS, "path of the currently loaded file" },
    { "get_latlon_boundingbox", (PyCFunction)getLatLonBoundingBox, METH_VARARGS, "get model bounding in latitude and longitude coordinates" },
    { "get_reference_point", (PyCFunction)getReferencePoint, METH_VARARGS, "get coordinates of base reference point" },
    { "get_default_srs", (PyCFunction)getDefaultSRS, METH_VARARGS, "get default SRS" },
    { "delete_entity", (PyCFunction)deleteEntity, METH_VARARGS, "delete a entity" },
    { nullptr }
  };
  
  static PyTypeObject documentType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  documentType.tp_name      = "ifcdb.Document";
  documentType.tp_basicsize = sizeof(Document);
  documentType.tp_itemsize  = 0;
  documentType.tp_flags     = Py_TPFLAGS_DEFAULT;
  documentType.tp_methods   = documentMethods;

  //PyType_Ready(&documentType);

  /**
   * Entity
   */
  static PyGetSetDef entityMembers[] =
  {
    { "id", reinterpret_cast<getter>(getEntityOid), nullptr, "Internal unique ID", nullptr },
    { "oid", reinterpret_cast<getter>(getEntityId), nullptr, "External ID if available", nullptr },
    { "guid", reinterpret_cast<getter>(getEntityGuid), nullptr, "", nullptr },
    { "name", reinterpret_cast<getter>(getEntityName), reinterpret_cast<setter>(setEntityName), "", nullptr },
    { "description", reinterpret_cast<getter>(getEntityDescription), reinterpret_cast<setter>(setEntityDescription), "", nullptr },
    { "class_type", reinterpret_cast<getter>(getEntityClassType), nullptr, "", nullptr },
    { "type", reinterpret_cast<getter>(getEntityType), nullptr, "", nullptr },
    { "object_type", reinterpret_cast<getter>(getEntityObjectType), reinterpret_cast<setter>(setEntityObjectType), "", nullptr },
    { "layer", reinterpret_cast<getter>(getEntityLayer), reinterpret_cast<setter>(setEntityLayer), "", nullptr },
    { "color", reinterpret_cast<getter>(getEntityColor), reinterpret_cast<setter>(setEntityColor), "", nullptr },
    { "long_name", reinterpret_cast<getter>(getEntityLongName), nullptr, "", nullptr },
    { nullptr }
  };

  static PyMethodDef entityMethods[] =
  {
    { "add_geometry_polygon", (PyCFunction)addGeometryFace, METH_VARARGS, "add a list of points as polygon" },
    { "add_geometry_polyline", (PyCFunction)addGeometryPolyline, METH_VARARGS, "add a list of points as polyline" },
    { "add_string_attribute", (PyCFunction)addStringAttribute, METH_VARARGS, "add new string attribute" },
    { "add_date_attribute", (PyCFunction)addDateAttribute, METH_VARARGS, "add new string attribute" },
    { "calc_quantities", (PyCFunction)calcQuantities, METH_NOARGS, "calculates quantities" },
    { "set_parent", (PyCFunction)setParent, METH_VARARGS, "set parent child relationship" },
    { "find_property_value", (PyCFunction)findPropertyValue, METH_VARARGS, "find property value by the specified name" },
    { "get_attributes", (PyCFunction)getEntityAttributes, METH_NOARGS, "get entity attributes" },
    { "get_properties", (PyCFunction)getPropertiesEntity, METH_NOARGS, "get all properties" },
    { "get_string_attribute", (PyCFunction)getStringAttribute, METH_VARARGS, "return value of string attribute" },
    { "get_geometry", (PyCFunction)getGeometry, METH_VARARGS, "get geometry" },
    { "get_relations", (PyCFunction)getRelationsEntity, METH_VARARGS, "get relations" },
    { "get_type_object", (PyCFunction)getTypeObject, METH_VARARGS, "get type object" },
    { "get_geometry_info", (PyCFunction)getGeometryInfo, METH_NOARGS, "get information of the geometry" },
    { "get_model_type", (PyCFunction)getModelType, METH_NOARGS, "get model type" },
    { "get_model_info", (PyCFunction)getModelInfo, METH_NOARGS, "get information about the model or format used" },
    { "is_point_within", (PyCFunction)isPointWithin, METH_VARARGS, "checks whether a point is located within the projected geometry" },
    { "get_boundingbox", (PyCFunction)getEntityBoundingBox, METH_VARARGS, "get bounding box of the entity" },
    { "get_children", (PyCFunction)getEntityChildren, METH_NOARGS, "get all children of the entity" },
    { "get_children_by_type", (PyCFunction)getEntityChildrenByType, METH_VARARGS, "get all children of the specified type" },
    { nullptr }
  };

  static PyTypeObject entityType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  entityType.tp_name      = "ifcdb.Entity";
  entityType.tp_basicsize = sizeof(PyIfcdb::Entity);
  entityType.tp_itemsize  = 0;
  entityType.tp_flags     = Py_TPFLAGS_DEFAULT;
  entityType.tp_getset    = entityMembers;
  entityType.tp_methods   = entityMethods;

  /**
   * EntityType
   */
  static PyGetSetDef entityTypeMembers[] =
  {
    { "id", reinterpret_cast<getter>(getEntityTypeOid), nullptr, "Internal unique ID", nullptr },
    { "oid", reinterpret_cast<getter>(getEntityTypeId), nullptr, "External ID if available", nullptr },
    { "guid", reinterpret_cast<getter>(getEntityTypeGuid), nullptr, "", nullptr },
    { "name", reinterpret_cast<getter>(getEntityTypeName), nullptr, "", nullptr },
    { "description", reinterpret_cast<getter>(getEntityTypeDescription), nullptr, "", nullptr },
    { nullptr }
  };

  static PyMethodDef entityTypeMethods[] =
  {
    { "get_properties", (PyCFunction)getPropertiesEntityType, METH_NOARGS, "get all properties" },
    { "get_relations", (PyCFunction)getRelationsEntityType, METH_NOARGS, "get relations" },
    { "get_attributes", (PyCFunction)getEntityTypeAttributes, METH_NOARGS, "get entity type attributes" },
    { nullptr }
  };

  static PyTypeObject entityTypeType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  entityTypeType.tp_name      = "ifcdb.EntityType";
  entityTypeType.tp_basicsize = sizeof(PyIfcdb::EntityType);
  entityTypeType.tp_itemsize  = 0;
  entityTypeType.tp_flags     = Py_TPFLAGS_DEFAULT;
  entityTypeType.tp_getset    = entityTypeMembers;
  entityTypeType.tp_methods   = entityTypeMethods;

  /**
   * Relation
   */
  static PyGetSetDef relationMembers[] =
  {
    { "id", reinterpret_cast<getter>(getRelationOid), nullptr, "Internal unique ID", nullptr },
    { "oid", reinterpret_cast<getter>(getRelationId), nullptr, "External ID if available", nullptr },
    { "guid", reinterpret_cast<getter>(getRelationGuid), nullptr, "", nullptr },
    { "name", reinterpret_cast<getter>(getRelationName), nullptr, "", nullptr },
    { "description", reinterpret_cast<getter>(getRelationDescription), nullptr, "", nullptr },
    { "type", reinterpret_cast<getter>(getRelationType), nullptr, "", nullptr },
    { nullptr }
  };

  static PyMethodDef relationMethods[] =
  {
    { "get_attributes", (PyCFunction)getRelationAttributes, METH_NOARGS, "get relation attributes" },
    //{ "srs_transform", (PyCFunction)srsTransform, METH_VARARGS, "SRS transformation of a point coordinate" },
    { nullptr }
  };

  static PyTypeObject relationType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  relationType.tp_name      = "ifcdb.Relation";
  relationType.tp_basicsize = sizeof(PyIfcdb::Relation);
  relationType.tp_itemsize  = 0;
  relationType.tp_flags     = Py_TPFLAGS_DEFAULT;
  relationType.tp_getset    = relationMembers;
  relationType.tp_methods   = relationMethods;

  /**
   * Geometry
   */
  static PyGetSetDef geometryMembers[] =
  {
    { "type", reinterpret_cast<getter>(getGeometryType), nullptr, "", nullptr },
    { "data", reinterpret_cast<getter>(getGeometryData), nullptr, "", nullptr },
    { nullptr }
  };

  static PyMethodDef geometryMethods[] =
  {
    { "srs_transform", (PyCFunction)geometrySrsTransform, METH_VARARGS, "SRS transformation of a point coordinate" },
    //{ "calc_quantities", (PyCFunction)geometryCalcQuantities, METH_NOARGS, "calculates quantities" },
    { "is_within", (PyCFunction)geometryIsWithin, METH_VARARGS, "checks whether a point is located within the projected geometry" },
    { "get_faces", (PyCFunction)geometryFaces, METH_NOARGS, "returns all faces of the geometry" },
    { "get_boundingbox", (PyCFunction)getGeometryBoundingBox, METH_VARARGS, "get bounding box of the geometry" },
    { nullptr }
  };

  static PyTypeObject geometryType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  geometryType.tp_name      = "ifcdb.Geometry";
  geometryType.tp_basicsize = sizeof(PyIfcdb::Geometry);
  geometryType.tp_itemsize  = 0;
  geometryType.tp_flags     = Py_TPFLAGS_DEFAULT;
  geometryType.tp_getset    = geometryMembers;
  geometryType.tp_methods   = geometryMethods;

 /**
 * Face
 */
  static PyGetSetDef faceMembers[] =
  {
    { nullptr }
  };

  static PyMethodDef faceMethods[] =
  {
    { "calc_area", (PyCFunction)faceCalcArea, METH_NOARGS, "calculates the face area" },
    { "is_within", (PyCFunction)faceIsWithin, METH_VARARGS, "checks whether a point is located within the face" },
    { "get_normal", (PyCFunction)faceNormal, METH_NOARGS, "returns the face normal" },
    { "get_polygons", (PyCFunction)faceGetPolygons, METH_NOARGS, "returns the polygons of the face" },
    { "get_outer_loop", (PyCFunction)faceGetOuterLoop, METH_NOARGS, "returns the outer loop as a polyline object" },
    { "get_outer_loop_coordinates", (PyCFunction)faceGetOuterLoopCoordinates, METH_NOARGS, "Returns a list structure containing the coordinates of the outerr loop" },
    { "get_inner_loops", (PyCFunction)faceGetInnerLoops, METH_NOARGS, "returns the inner loops as a list of polyline objects" },
    { "get_inner_loops_coordinates", (PyCFunction)faceGetInnerLoopsCoordinates, METH_NOARGS, "Returns a list structure containing the coordinates of the inner loops" },
    { nullptr }
  };

  static PyTypeObject faceType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  faceType.tp_name = "ifcdb.Face";
  faceType.tp_basicsize = sizeof(PyIfcdb::Face);
  faceType.tp_itemsize = 0;
  faceType.tp_flags = Py_TPFLAGS_DEFAULT;
  faceType.tp_getset = faceMembers;
  faceType.tp_methods = faceMethods;

  /**
  * Polyline
  */
  static PyGetSetDef polylineMembers[] =
  {
    { nullptr }
  };

  static PyMethodDef polylineMethods[] =
  {
    { "is_closed", (PyCFunction)polylineIsClosed, METH_NOARGS, "check weather the first and last point are the same" },
    { "is_within", (PyCFunction)polylineIsWithin, METH_VARARGS, "checks whether a point is located within a closed polyline" },
    { "intersects_with", (PyCFunction)polylineIntersects, METH_VARARGS, "checks whether a polyline intersects with an other polyline" },
    { "get_coordinates", (PyCFunction)polylineGetCoordinates, METH_NOARGS, "returns the coordinates of the polyline" },
    { nullptr }
  };

  static PyTypeObject polylineType = { PyVarObject_HEAD_INIT(nullptr, 0) };
  polylineType.tp_name = "ifcdb.Polyline";
  polylineType.tp_basicsize = sizeof(PyIfcdb::Polyline);
  polylineType.tp_itemsize = 0;
  polylineType.tp_flags = Py_TPFLAGS_DEFAULT;
  polylineType.tp_getset = polylineMembers;
  polylineType.tp_methods = polylineMethods;

  PyModule_AddType(m, &documentType);
  PyModule_AddType(m, &entityType);
  PyModule_AddType(m, &entityTypeType);
  PyModule_AddType(m, &relationType);
  PyModule_AddType(m, &geometryType);
  PyModule_AddType(m, &faceType);
  PyModule_AddType(m, &polylineType);
  //PyModule_AddObject(m, "Document", reinterpret_cast<PyObject*>(&documentType));
  //PyModule_AddObject(m, "Entity", reinterpret_cast<PyObject*>(&entityType));

  Types types;
  types.document   = &documentType;
  types.entity     = &entityType;
  types.entityType = &entityTypeType;
  types.relation   = &relationType;
  types.geometry   = &geometryType;
  types.face       = &faceType;
  types.polyline   = &polylineType;

  return types;
}

IfcDBState& getIfcDBState()
{
  return *static_cast<IfcDBState*>(PyCapsule_Import("ifcdb._C_API", 0));
}

PyObject* getDocument(PyObject* self, PyObject* args)
{
  auto state = getIfcDBState();

  auto document = reinterpret_cast<Document*>(state.types.document->tp_alloc(state.types.document, 0));
  document->db = state.db;
  document->states = state.states;
  document->appFeature = state.appFeature;

  return (PyObject*)document;
}

static PyMethodDef IfcdbMethodDefinitions[] = {
        {"get_document", getDocument, METH_VARARGS, "get the current document"},
        {NULL, NULL, 0, NULL}
};

PyModuleDef IfcDBModuleDefinition = {
        PyModuleDef_HEAD_INIT, "ifcdb", nullptr, -1, IfcdbMethodDefinitions, nullptr
};