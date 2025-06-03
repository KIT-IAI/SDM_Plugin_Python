#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();
extern PyObject* processIfcProperties(IfcDB::ModelInfo* pModelInfo, IfcDB::Property* pProperty);

PyObject* processIfcMaterialProperties(IfcDB::ModelInfo* pModelInfo, const std::vector<IfcDB::Property*>& properties)
{
  if (properties.empty() == false)
  {
    PyObject* dictObj = PyDict_New();

    (L"MaterialProperties");

    for (auto pProperty : properties)
    {
      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pProperty->getName()).c_str()), processIfcProperties(pModelInfo, pProperty));
    }

    return dictObj;
  }

  Py_RETURN_NONE;
}

PyObject* processIfcMaterial(IfcDB::ModelInfo* pModelInfo, IfcDB::Material* pMaterial)
{
  PyObject* dictObj = PyDict_New();

  if(pMaterial->m_hasColor == true)
  {
    // pMaterial->m_color.red,
    // pMaterial->m_color.green,
    // pMaterial->m_color.blue,
    // pMaterial->m_color.alpha);
  }

  PyDict_SetItem(dictObj, PyUnicode_FromString("Material"), PyUnicode_FromString(toUtf8(pMaterial->m_MaterialName).c_str()));
  PyDict_SetItem(dictObj, PyUnicode_FromString("Category"), PyUnicode_FromString(toUtf8(pMaterial->m_Category).c_str()));

  PyObject* propertiesDict = processIfcMaterialProperties(pModelInfo, pMaterial->m_Properties);

  if (propertiesDict)
  {
    PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialProperties"), propertiesDict);
  }

  return dictObj;;
}

PyObject* processIfcMaterialLayer(IfcDB::ModelInfo* pModelInfo, IfcDB::MaterialLayer* pMaterialLayer)
{
  PyObject* dictObj = PyDict_New();

  PyDict_SetItem(dictObj, PyUnicode_FromString("Width"), PyFloat_FromDouble(pMaterialLayer->m_width));
  PyDict_SetItem(dictObj, PyUnicode_FromString("Material"), PyUnicode_FromString(toUtf8(pMaterialLayer->m_MaterialName).c_str()));
  PyDict_SetItem(dictObj, PyUnicode_FromString("IsVentilated"), PyUnicode_FromString(toUtf8(getLogicalAsString(pMaterialLayer->m_isVentilatd)).c_str()));

  if(pMaterialLayer->m_hasColor == true)
  {
    // pMaterialLayer->m_color.red,
    // pMaterialLayer->m_color.green,
    // pMaterialLayer->m_color.blue,
    // pMaterialLayer->m_color.alpha);
  }

  PyObject* propertiesDict = processIfcMaterialProperties(pModelInfo, pMaterialLayer->m_Properties);

  if (propertiesDict)
  {
    PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialProperties"), propertiesDict);
  }

  return dictObj;;
}

PyObject* processIfcMaterialProfile(IfcDB::ModelInfo* pModelInfo, IfcDB::MaterialProfile* pMaterialProfile)
{
  PyObject* dictObj = PyDict_New();

  PyDict_SetItem(dictObj, PyUnicode_FromString("Material"), PyUnicode_FromString(toUtf8(pMaterialProfile->m_Name).c_str()));
  PyDict_SetItem(dictObj, PyUnicode_FromString("Priority"), PyLong_FromLong(pMaterialProfile->m_Priority));
  PyDict_SetItem(dictObj, PyUnicode_FromString("Category"), PyUnicode_FromString(toUtf8(pMaterialProfile->m_Category).c_str()));

  if (pMaterialProfile->m_hasColor)
  {
    // pMaterialProfile->m_color.red,
    // pMaterialProfile->m_color.green,
    // pMaterialProfile->m_color.blue,
    // pMaterialProfile->m_color.alpha);
  }

  PyObject* propertiesDict = processIfcMaterialProperties(pModelInfo, pMaterialProfile->m_Properties);

  if (propertiesDict)
  {
    PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialProperties"), propertiesDict);
  }

  return dictObj;
}

PyObject* processIfcMaterialConstituent(IfcDB::ModelInfo* pModelInfo, IfcDB::MaterialConstituent* pMaterialConstituent, IfcDB::Geometry* pGeometry)
{
  // if(pGeometry == nullptr)
  // {
  //   // Geometry
  //   for (const auto& representation : pEntity->getRepresentations())
  //   {
  //     for (auto aspect : representation.m_aspects)
  //     {
  //       if (aspect.first == pMaterialConstituent->m_Name)
  //       {
  //         pGeometry = aspect.second.front();
  //       }
  //     }
  //   }
  // }

  std::tstring name = pMaterialConstituent->m_Name.c_str();

  if (pMaterialConstituent->m_pMaterial != nullptr)
  {
    name = pMaterialConstituent->m_pMaterial->m_MaterialName.c_str();
  }

  (L"Material", name);
  (L"Fraction", pMaterialConstituent->m_Fraction);
  (L"Category", pMaterialConstituent->m_Category.c_str());

  Py_RETURN_NONE;
}

PyObject* processIfcMaterialDefinition(IfcDB::ModelInfo* pModelInfo, IfcDB::MaterialBase* pMaterialBase)
{
  if (pMaterialBase == nullptr)
  {
    Py_RETURN_NONE;
  }

  if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL)
  {
    IfcDB::Material* pMaterial = dynamic_cast<IfcDB::Material*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("Material"));
    PyDict_SetItem(dictObj, PyUnicode_FromString("Material"), processIfcMaterial(pModelInfo, pMaterial));
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_LAYER_SET)
  {
    IfcDB::MaterialLayerSet* pMaterialLayerSet = dynamic_cast<IfcDB::MaterialLayerSet*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("MaterialLayerSet"));

    if (pMaterialLayerSet->m_MaterialLayers.empty() == false)
    {
      PyObject* listObj = PyList_New(0);

      for (auto pLayer : pMaterialLayerSet->m_MaterialLayers)
      {
        PyList_Append(listObj, processIfcMaterialLayer(pModelInfo, pLayer));
      }

      PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialLayers"), listObj);
    }

    return dictObj;
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_LAYER_SET_USAGE)
  {
    IfcDB::MaterialLayerSetUsage* pMaterialLayerSetUsage = dynamic_cast<IfcDB::MaterialLayerSetUsage*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("MaterialLayerSetUsage"));

    L"MaterialLayerSet", pMaterialLayerSetUsage->m_Name.c_str();
    L"LayerSetDirection", pMaterialLayerSetUsage->m_LayerSetDirection.c_str();
    L"DirectionSense", pMaterialLayerSetUsage->m_DirectionSense.c_str();
    L"OffsetFromReferenceLine", pMaterialLayerSetUsage->m_Offset;

    if (pMaterialLayerSetUsage->m_MaterialLayers.empty() == false)
    {
      PyObject* listObj = PyList_New(0);

      for (auto pLayer : pMaterialLayerSetUsage->m_MaterialLayers)
      {
        PyList_Append(listObj, processIfcMaterialLayer(pModelInfo, pLayer));
      }

      PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialLayers"), listObj);
    }

    return dictObj;
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_LIST)
  {
    IfcDB::MaterialList* pMaterialList = dynamic_cast<IfcDB::MaterialList*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("Materialist"));

    PyObject* listObj = PyList_New(0);

    for (auto pMaterial : pMaterialList->m_Materials)
    {
      PyList_Append(listObj, processIfcMaterial(pModelInfo, pMaterial));
    }

    PyDict_SetItem(dictObj, PyUnicode_FromString("Materials"), listObj);

    return dictObj;
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_PROFILE_SET)
  {
    IfcDB::MaterialProfileSet* pMaterialProfileSet = dynamic_cast<IfcDB::MaterialProfileSet*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("MaterialProfileSet"));

    L"MaterialProfileSet", pMaterialProfileSet->m_Name.c_str();

    if (pMaterialProfileSet->m_MaterialProfiles.empty() == false)
    {
      PyObject* listObj = PyList_New(0);

      for (auto pMaterialProfile : pMaterialProfileSet->m_MaterialProfiles)
      {
        PyList_Append(listObj, processIfcMaterialProfile(pModelInfo, pMaterialProfile));
      }

      PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialProfiles"), listObj);
    }

    return dictObj;
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_PROFILE_SET_USAGE)
  {
    IfcDB::MaterialProfileSetUsage* pMaterialProfileSetUsage = dynamic_cast<IfcDB::MaterialProfileSetUsage*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("MaterialLayerSetUsage"));

    L"MaterialProfileSetUsage", pMaterialProfileSetUsage->m_Name.c_str();
    L"CardinalPoint", pMaterialProfileSetUsage->m_CardinalPoint;
    L"ReferenceExtent", pMaterialProfileSetUsage->m_ReferenceExtent;

    if (pMaterialProfileSetUsage->m_MaterialProfiles.empty() == false)
    {
      PyObject* listObj = PyList_New(0);

      for (auto pMaterialProfile : pMaterialProfileSetUsage->m_MaterialProfiles)
      {
        PyList_Append(listObj, processIfcMaterialProfile(pModelInfo, pMaterialProfile));
      }

      PyDict_SetItem(dictObj, PyUnicode_FromString("MaterialProfiles"), listObj);
    }

    return dictObj;
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_CONSTITUENT)
  {
    IfcDB::MaterialConstituent* pMaterialConstituent = dynamic_cast<IfcDB::MaterialConstituent*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("MaterialConstituent"));

    //processIfcMaterialConstituent(pReqP, baseLevel, pMaterialConstituent, nullptr);

    return dictObj;
  }
  else if (pMaterialBase->getMaterialType() == IfcDB::MATERIAL_CONSTITUENT_SET)
  {
    IfcDB::MaterialConstituentSet* pMaterialConstituentSet = dynamic_cast<IfcDB::MaterialConstituentSet*>(pMaterialBase);

    PyObject* dictObj = PyDict_New();
    PyDict_SetItem(dictObj, PyUnicode_FromString("Type"), PyUnicode_FromString("MaterialConstituentSet"));

    //L"MaterialConstituentSet", pMaterialConstituentSet->m_Name.c_str());

    if (pMaterialConstituentSet->m_MaterialConstituents.empty() == false)
    {
      for (auto pMaterialConstituent : pMaterialConstituentSet->m_MaterialConstituents)
      {
      }
    }

    return dictObj;
  }

  Py_RETURN_NONE;
}