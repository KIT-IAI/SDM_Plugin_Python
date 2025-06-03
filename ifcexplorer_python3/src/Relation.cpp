#include "Relation.hpp"
#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();
extern PyObject* processIfcMaterialDefinition(IfcDB::ModelInfo* pModelInfo, IfcDB::MaterialBase* pMaterialBase);

PyObject* getRelationId(PyIfcdb::Relation* relation, void*)
{
  return PyLong_FromLongLong(relation->relation->getEntityId());
}

PyObject* getRelationOid(PyIfcdb::Relation* relation, void*)
{
  return PyLong_FromLongLong(relation->relation->getOid());
}

PyObject* getRelationGuid(PyIfcdb::Relation* relation, void*)
{
  return PyUnicode_FromString(toUtf8(relation->relation->getGuid()).c_str());
}

PyObject* getRelationName(PyIfcdb::Relation* relation, void*)
{
  return PyUnicode_FromString(toUtf8(relation->relation->getName()).c_str());
}

PyObject* getRelationDescription(PyIfcdb::Relation* relation, void*)
{
  return PyUnicode_FromString(toUtf8(relation->relation->getDescription()).c_str());
}

PyObject* getRelationType(PyIfcdb::Relation* relation, void*)
{
  return PyUnicode_FromString(toUtf8(getTypeOfRelationAsString(relation->relation->getRelationType())).c_str());
}

IfcDB::IfcRelationType getRelationType(const std::tstring& relationType)
{
  if (relationType == L"RelationOneToOne")                         return IfcDB::REL_ONE_TO_ONE;
  else if (relationType == L"RelationOneToMany")                   return IfcDB::REL_ONE_TO_MANY;
  else if (relationType == L"RelationManyToOne")                   return IfcDB::REL_MANY_TO_ONE;
  else if (relationType == L"EnergyPlus_RelationMaterial")         return IfcDB::ENERGYPLUS_REL_CONSTRUCTION;
  else if (relationType == L"EnergyPlus_RelationConstruction")     return IfcDB::ENERGYPLUS_REL_MATERIAL;
  else if (relationType == L"EnergyPlus_RelationZone")             return IfcDB::ENERGYPLUS_REL_ZONE;
  else if (relationType == L"IfcRelAggregates")                    return IfcDB::REL_AGGREGATES;
  else if (relationType == L"IfcRelAssignsToActor")                return IfcDB::REL_ASSIGNS_TO_ACTOR;
  else if (relationType == L"IfcRelAssignsToControl")              return IfcDB::REL_ASSIGNS_TO_CONTROL;
  else if (relationType == L"IfcRelAssignsToProcess")              return IfcDB::REL_ASSIGNS_TO_PROCESS;
  else if (relationType == L"IfcRelAssignsTasks")                  return IfcDB::REL_ASSIGNS_TASKS;
  else if (relationType == L"IfcRelAssignsToGroup")                return IfcDB::REL_ASSIGNS_TO_GROUP;
  else if (relationType == L"IfcRelAssignsToProduct")              return IfcDB::REL_ASSIGNS_TO_PRODUCT;
  else if (relationType == L"IfcRelAssociatesClassification")      return IfcDB::REL_ASSOCIATES_CLASSIFICATION;
  else if (relationType == L"IfcRelAssociatesDocument")            return IfcDB::REL_ASSOCIATES_DOCUMENT;
  else if (relationType == L"IfcRelAssociatesLibrary")             return IfcDB::REL_ASSOCIATES_LIBRARY;
  else if (relationType == L"IfcRelAssociatesMaterial")            return IfcDB::REL_ASSOCIATES_MATERIAL;
  else if (relationType == L"IfcRelAssociatesProfileProperties")   return IfcDB::REL_ASSOCIATES_PROFILE_PROPERTIES;
  else if (relationType == L"IfcRelConnectsElements")              return IfcDB::REL_CONNECTS_ELEMENTS;
  else if (relationType == L"IfcRelConnectsPathElements")          return IfcDB::REL_CONNECTS_PATH_ELEMENTS;
  else if (relationType == L"IfcRelConnectsWithRealizingElements") return IfcDB::REL_CONNECTS_WITH_REALIZING_ELEMENTS;
  else if (relationType == L"IfcRelConnectsPorts")                 return IfcDB::REL_CONNECTS_PORTS;
  else if (relationType == L"IfcRelConnectsPortToElement")         return IfcDB::REL_CONNECTS_PORTS_TO_ELEMENT;
  else if (relationType == L"IfcRelConnectsStructuralMember")      return IfcDB::REL_CONNECTS_STRUCTURAL_MEMBER;
  else if (relationType == L"IfcRelConnectsWithEccentricity")      return IfcDB::REL_CONNECTS_WITH_ECCENTRICITY;
  else if (relationType == L"IfcRelConnectsStructuralActivity")    return IfcDB::REL_CONNECTS_STRUCTURAL_ACTIVITY;
  else if (relationType == L"IfcRelConnectsStructuralElement")     return IfcDB::REL_CONNECTS_STRUCTURAL_ELEMENT;
  else if (relationType == L"IfcRelContainedInSpatialStructure")   return IfcDB::REL_CONTAINED_IN_SPATIAL_STRUCTURE;
  else if (relationType == L"IfcRelCoversSpaces")                  return IfcDB::REL_COVERS_SPACES;
  else if (relationType == L"IfcRelCoversBldgElements")            return IfcDB::REL_COVERS_BLDG_ELEMENTS;
  else if (relationType == L"IfcRelDeclares")                      return IfcDB::REL_DECLARES;
  else if (relationType == L"IfcRelDefinesByObject")               return IfcDB::REL_DEFINES_BY_OBJECT;
  else if (relationType == L"IfcRelDefinesByProperties")           return IfcDB::REL_DEFINES_BY_PROPERTIES;
  else if (relationType == L"IfcRelDefinesByTemplate")             return IfcDB::REL_DEFINES_BY_TEMPLATE;
  else if (relationType == L"IfcRelDefinesByType")                 return IfcDB::REL_DEFINES_BY_TYPE;
  else if (relationType == L"IfcRelFillsElement")                  return IfcDB::REL_FILLS_ELEMENT;
  else if (relationType == L"IfcRelInterferesElements")            return IfcDB::REL_INTERFERES_ELEMENTS;
  else if (relationType == L"IfcRelNests")                         return IfcDB::REL_NESTS;
  else if (relationType == L"IfcRelOccupiesSpaces")                return IfcDB::REL_OCCUPIES_SPACES;
  else if (relationType == L"IfcRelPositions")                     return IfcDB::REL_POSITIONS;
  else if (relationType == L"IfcRelReferencedInSpatialStructure")  return IfcDB::REL_REFERENCED_IN_SPATIAL_STRUCTURE;
  else if (relationType == L"IfcRelSpaceBoundary")                 return IfcDB::REL_SPACE_BOUNDARY;
  else if (relationType == L"IfcRelSpaceBoundary1stLevel")         return IfcDB::REL_SPACE_BOUNDARY_1STLEVEL;
  else if (relationType == L"IfcRelSpaceBoundary2ndLevel")         return IfcDB::REL_SPACE_BOUNDARY_2NDLEVEL;
  else if (relationType == L"IfcRelSequence")                      return IfcDB::REL_SEQUENCE;
  else if (relationType == L"IfcRelServicesBuildings")             return IfcDB::REL_SERVICES_BUILDINGS;
  else if (relationType == L"IfcRelVoidsElement")                  return IfcDB::REL_VOIDS_ELEMENT;
  else if (relationType == L"RelEnergyCalculation")                return IfcDB::REL_ENERGY_CALCULATION;
  else if (relationType == L"GmlRelFeature")                       return IfcDB::REL_GML_FEATURE;
  else if (relationType == L"gbxmlRelation")                       return IfcDB::REL_GBXML;
  //else if (relationType == L"")return REL_GBXML_AIRLOOP;
  else if (relationType == L"gbxmlRelConstruction")                return IfcDB::REL_GBXML_CONSTRUCTION;
  //else if (relationType == L"")return REL_GBXML_DAYSCHEDULE;
  //else if (relationType == L"")return REL_GBXML_DOCUMENTHISTORY;
  //else if (relationType == L"")return REL_GBXML_HYDRONICLOOP;
  //else if (relationType == L"")return REL_GBXML_EXT_EQUIP;
  //else if (relationType == L"")return REL_GBXML_INT_EQUIP;
  //else if (relationType == L"")return REL_GBXML_LIGHTING;
  //else if (relationType == L"")return REL_GBXML_LIGHTING_SYSTEM;
  //else if (relationType == L"")return REL_GBXML_LIGHTING_CONTROL;
  //else if (relationType == L"")return REL_GBXML_INFILTRATION_FLOW;
  //else if (relationType == L"")return REL_GBXML_METER;
  //else if (relationType == L"")return REL_GBXML_RESULTS;
  //else if (relationType == L"")return REL_GBXML_SCHEDULE;
  //else if (relationType == L"")return REL_GBXML_WEEKSCHEDULE;
  //else if (relationType == L"")return REL_GBXML_WINDOWTYPE;
  //else if (relationType == L"")return REL_GBXML_YEARSCHEDULE;
  //else if (relationType == L"")return REL_GBXML_ZONE;
  else if (relationType == L"LandXMLEntityRelation")               return IfcDB::REL_LANDXML_ENTITY;
  else if (relationType == L"LandXMLPipenetworkRelation")          return IfcDB::REL_LANDXML_PIPENETWORK;
  else if (relationType == L"AttributeRelation")                   return IfcDB::REL_ATTRIBUTE;
  else return IfcDB::UNKNOWN_RELATION;
}

PyObject* getRelationAttributes(PyIfcdb::Relation* relation, PyObject* args)
{
  IfcDB::ModelInfo* pModelInfo = relation->relation->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  PyObject* dictObj = PyDict_New();

  //PyDict_SetItem(dictObj, PyUnicode_FromString(""), PyUnicode_FromString(toUtf8().c_str()));

  switch (relation->relation->getRelationType())
  {
    case IfcDB::REL_ASSOCIATES_MATERIAL:
    {
      IfcDB::IfcRelMaterial* pRelMaterial = dynamic_cast<IfcDB::IfcRelMaterial*>(relation->relation);

      PyObject* material = processIfcMaterialDefinition(pModelInfo, pRelMaterial->getMaterial());
      PyDict_SetItem(dictObj, PyUnicode_FromString("RelatingMaterial"), material);

      break;
    }
    case IfcDB::REL_CONNECTS_STRUCTURAL_ACTIVITY:
    {
      IfcDB::IfcRelConnectsStructuralActivity* pRelConnects = dynamic_cast<IfcDB::IfcRelConnectsStructuralActivity*>(relation->relation);

      PyDict_SetItem(dictObj, PyUnicode_FromString("RelatingElement"), PyLong_FromLong(pModelInfo->getOidFromEntityId(pRelConnects->getRelatingElement())));
      PyDict_SetItem(dictObj, PyUnicode_FromString("IfcRelConnectsStructuralActivity"), PyLong_FromLong(pModelInfo->getOidFromEntityId(pRelConnects->getRelatedStructuralActivity())));
      break;
    }
    case IfcDB::REL_CONNECTS_STRUCTURAL_MEMBER:
    {
      IfcDB::IfcRelConnectsStructuralMember* pRelConnects = dynamic_cast<IfcDB::IfcRelConnectsStructuralMember*>(relation->relation);

      PyDict_SetItem(dictObj, PyUnicode_FromString("RelatingStructuralMember"), PyLong_FromLong(pModelInfo->getOidFromEntityId(pRelConnects->getRelatingStructuralMember())));
      PyDict_SetItem(dictObj, PyUnicode_FromString("RelatedStructuralConnection"), PyLong_FromLong(pModelInfo->getOidFromEntityId(pRelConnects->getRelatedStructuralConnection())));
      break;
    }
  }

  return dictObj;
}
