#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();
extern void processAttributes(PyObject* dictObj, const IfcDB::Attribute* pAttribute);

PyObject* processIfcProperties(IfcDB::ModelInfo* pModelInfo, IfcDB::Property* pProperty)
{
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  if (!pDB)
  {
    PyErr_SetString(PyExc_AttributeError, "db");

    Py_RETURN_NONE;
  }

  if (pProperty)
  {
    IfcDB::ComplexProperty* pComplexProperty = nullptr;
    IfcDB::SimpleProperty*  pSimpleProperty = nullptr;

    if (pComplexProperty = dynamic_cast<IfcDB::ComplexProperty*>(pProperty))
    {
      PyObject* dictObj = PyDict_New();

      std::vector<IfcDB::Property*> subProps = pComplexProperty->getProperties();

      std::sort(subProps.begin(), subProps.end(), [](IfcDB::Property* pLhs, IfcDB::Property* pRHS){ return pLhs->getName() < pRHS->getName(); });

      for (auto pSubProp : subProps)
      {
        PyObject* pyObj = processIfcProperties(pModelInfo, pSubProp);

        PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pSubProp->getName()).c_str()), pyObj);
      }

      return dictObj;
    }
    else if (pSimpleProperty = dynamic_cast<IfcDB::SimpleProperty*>(pProperty))
    {
      switch(pSimpleProperty->getType())
      {
        case IfcDB::SimpleProperty::Single:
        {
          PyObject* listObj = PyList_New(0);

          IfcDB::SingleValue* pSingleValue = dynamic_cast<IfcDB::SingleValue*>(pSimpleProperty);
          std::tstring value(pSingleValue->getNominalValue());
          IfcDB::ifcOid oidUnit = pModelInfo->getOidFromEntityId(pSingleValue->getUnit());
          IfcDB::UnitDef::Unit* pUnit = pDB->getUnit(oidUnit);

          PyList_Append(listObj, PyUnicode_FromString(toUtf8(value).c_str()));

          if (pUnit != nullptr)
          {
            PyList_Append(listObj, PyUnicode_FromString(toUtf8(pUnit->getFormatedStringWithUnit(value)).c_str()));
          }

          return listObj;
        }
        case IfcDB::SimpleProperty::Enumerated:
        {
          PyObject* listObj = PyList_New(0);

          IfcDB::EnumeratedValue* pEnumeratedValue = dynamic_cast<IfcDB::EnumeratedValue*>(pSimpleProperty);
          std::tstring value(pEnumeratedValue->toString());
          IfcDB::ifcOid oidUnit = pModelInfo->getOidFromEntityId(pEnumeratedValue->getUnit());
          IfcDB::UnitDef::Unit* pUnit = pDB->getUnit(oidUnit);

          PyList_Append(listObj, PyUnicode_FromString(toUtf8(value).c_str()));

          if (pUnit != nullptr)
          {
            PyList_Append(listObj, PyUnicode_FromString(toUtf8(pUnit->getFormatedStringWithUnit(value)).c_str()));
          }

          return listObj;
        }
        case IfcDB::SimpleProperty::Bounded:
        {
          PyObject* listObj = PyList_New(0);

          IfcDB::BoundedValue* pBoundedValue = dynamic_cast<IfcDB::BoundedValue*>(pProperty);

          PyList_Append(listObj, PyUnicode_FromString(toUtf8(pBoundedValue->getValue()).c_str()));

          return listObj;
        }
        case IfcDB::SimpleProperty::Table:
        {
          PyObject* listObj = PyList_New(0);

          IfcDB::TableValue* pTableValue = dynamic_cast<IfcDB::TableValue*>(pProperty);

          PyList_Append(listObj, PyUnicode_FromString(toUtf8(pTableValue->getValue()).c_str()));

          return listObj;
        }
        case IfcDB::SimpleProperty::Reference:
        {
          PyObject* listObj = PyList_New(0);

          IfcDB::ReferenceValue* pReferenceValue = dynamic_cast<IfcDB::ReferenceValue*>(pProperty);

          PyList_Append(listObj, PyUnicode_FromString(toUtf8(pReferenceValue->getUsageName()).c_str()));

          if (pReferenceValue->getPropertyReference() != 0)
          {
            IfcDB::ifcOid oidRelatingClassification = pModelInfo->getOidFromEntityId(pReferenceValue->getPropertyReference());
            IfcDB::SimpleEntity* pSimpleEntity = pDB->getSimpleEntity(oidRelatingClassification);
          }

          return listObj;
        }
        case IfcDB::SimpleProperty::List:
        {
          PyObject* listObj = PyList_New(0);

          IfcDB::ListValue* pListValue = dynamic_cast<IfcDB::ListValue*>(pProperty);

          PyList_Append(listObj, PyUnicode_FromString(toUtf8(pListValue->getValue()).c_str()));

          return listObj;
        }
        default:
          ;
      }
    }
  }

  Py_RETURN_NONE;
}

std::tstring formatDoubleAttribute(double value, IfcDB::UOM* pUOM)
{
  std::tstringstream valStream;
  valStream << value;

  if (pUOM != nullptr)
  {
    if (pUOM->getIdentifier() == L"scale")
    {
      valStream << value * 100;
    }

    valStream << L" [" << pUOM->getSign() << L"]";
  }

  return valStream.str();
}

PyObject* processGmlProperties(IfcDB::Feature* pFeature)
{
  std::vector<std::tstring>       attributName;
  std::vector<std::tstring>       strAttributWert;
  std::vector<int64_t>            intAttributWert;
  std::vector<double>             doubleAttributWert;
  std::vector<bool>               booleanAttributWert;
  std::vector<IfcDB::UOM*>        vUOMs;

  PyObject* dictObj = nullptr;

  if (pFeature->hasAttributes())
  {
    dictObj = PyDict_New();

    size_t anz = pFeature->getStringAttributWerte(attributName, strAttributWert);

    for (size_t i = 0; i < anz; i++)
    {
      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(attributName[i]).c_str()), PyUnicode_FromString(toUtf8(strAttributWert[i]).c_str()));
    }

    attributName.clear();
    strAttributWert.clear();

    anz = pFeature->getDateAttributWerte(attributName, strAttributWert);
    for (size_t i = 0; i < anz; i++)
    {
      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(attributName[i]).c_str()), PyUnicode_FromString(toUtf8(strAttributWert[i]).c_str()));
    }

    attributName.clear();
    strAttributWert.clear();

    anz = pFeature->getURLAttributWerte(attributName, strAttributWert);
    for (size_t i = 0; i < anz; i++)
    {
      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(attributName[i]).c_str()), PyUnicode_FromString(toUtf8(strAttributWert[i]).c_str()));
    }

    attributName.clear();
    intAttributWert.clear();

    anz = pFeature->getIntegerAttributWerte(attributName, intAttributWert);
    for (size_t i = 0; i < anz; i++)
    {
      std::tstringstream valStream;
      valStream << intAttributWert[i];

      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(attributName[i]).c_str()), PyUnicode_FromString(toUtf8(valStream.str()).c_str()));
    }

    vUOMs.clear();
    attributName.clear();
    doubleAttributWert.clear();

    anz = pFeature->getDoubleAttributWerte(attributName, doubleAttributWert, vUOMs);
    for (size_t i = 0; i < anz; i++)
    {
      std::wstring value = formatDoubleAttribute(doubleAttributWert[i], vUOMs[i]);

      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(attributName[i]).c_str()), PyUnicode_FromString(toUtf8(value).c_str()));
    }

    attributName.clear();
    booleanAttributWert.clear();

    anz = pFeature->getBoolAttributWerte(attributName, booleanAttributWert);
    for (size_t i = 0; i < anz; i++)
    {
      std::wstring value = (booleanAttributWert[i] == true) ? L"True" : L"False";

      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(attributName[i]).c_str()), PyUnicode_FromString(toUtf8(value).c_str()));
    }
  }

  IfcDB::ModelInfo* pModelInfo = pFeature->getModelInfo();
  IfcDB::Populationi* pDB = pModelInfo->getPopulation();

  std::vector<IfcDB::IfcEntityRelation*> relations;
  size_t relAnz = pDB->getRelation(pFeature->getOid(), relations);

  for (size_t i = 0; i < relAnz; i++)
  {
    IfcDB::IfcRelGmlFeature* pRel = (IfcDB::IfcRelGmlFeature*)relations[i];
    IfcDB::ifcOid refObjId = pRel->getRefObjId();
    IfcDB::GmlAbstractObject* pObjRel = (IfcDB::Feature*)pDB->get(refObjId);

    if (pObjRel->getEntityType().find(L"Attribute") != std::string::npos)
    {
      if (!dictObj)
      {
        dictObj = PyDict_New();
      }

      IfcDB::Feature* pFeatureRel = dynamic_cast<IfcDB::Feature*>(pObjRel);

      if (pFeatureRel != nullptr)
      {
        std::vector<std::tstring>       attributName;
        std::vector<std::tstring>       strAttributWert;
        std::vector<int64_t>            intAttributWert;
        std::vector<double>             doubleAttributWert;
        std::vector<bool>               booleanAttributWert;
        std::vector<IfcDB::UOM*>        vUOMs;

        size_t anz = pFeatureRel->getStringAttributWerte(attributName, strAttributWert);

        std::tstring name;
        std::tstring strValue;
        bool hasName(false);
        bool hasValue(false);

        for (size_t i = 0; i < anz; i++)
        {
          if (attributName[i].find(L"name") != std::string::npos)
          {
            hasName = true;
            name = strAttributWert[i];
          }
          else if (attributName[i].find(L"value") != std::string::npos || attributName[i].find(L"wert") != std::string::npos)
          {
            hasValue = true;
            strValue = strAttributWert[i];
          }
        }

        if (hasName && hasValue)
        {
          PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(name).c_str()), PyUnicode_FromString(toUtf8(strValue).c_str()));
        }

        hasName = false;
        hasValue = false;
        attributName.clear();
        strAttributWert.clear();

        anz = pFeature->getDateAttributWerte(attributName, strAttributWert);
        for (size_t i = 0; i < anz; i++)
        {
          if (attributName[i].find(L"name") != std::string::npos)
          {
            hasName = true;
            name = strAttributWert[i];
          }
          else if (attributName[i].find(L"value") != std::string::npos || attributName[i].find(L"wert") != std::string::npos)
          {
            hasValue = true;
            strValue = strAttributWert[i];
          }
        }

        if (hasName && hasValue)
        {
          PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(name).c_str()), PyUnicode_FromString(toUtf8(strValue).c_str()));
        }

        hasName = false;
        hasValue = false;
        attributName.clear();
        strAttributWert.clear();

        anz = pFeature->getURLAttributWerte(attributName, strAttributWert);
        for (size_t i = 0; i < anz; i++)
        {
          if (attributName[i].find(L"name") != std::string::npos)
          {
            hasName = true;
            name = strAttributWert[i];
          }
          else if (attributName[i].find(L"value") != std::string::npos || attributName[i].find(L"wert") != std::string::npos)
          {
            hasValue = true;
            strValue = strAttributWert[i];
          }
        }

        if (hasName && hasValue)
        {
          PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(name).c_str()), PyUnicode_FromString(toUtf8(strValue).c_str()));
        }

        hasName = false;
        hasValue = false;
        attributName.clear();
        intAttributWert.clear();

        anz = pFeature->getIntegerAttributWerte(attributName, intAttributWert);
        for (size_t i = 0; i < anz; i++)
        {
          if (attributName[i].find(L"name") != std::string::npos)
          {
            hasName = true;
            name = strAttributWert[i];
          }
          else if (attributName[i].find(L"value") != std::string::npos || attributName[i].find(L"wert") != std::string::npos)
          {
            std::tstringstream valStream;
            valStream << intAttributWert[i];

            hasValue = true;
            strValue = valStream.str();
          }
        }

        if (hasName && hasValue)
        {
          PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(name).c_str()), PyUnicode_FromString(toUtf8(strValue).c_str()));
        }

        hasName = false;
        hasValue = false;
        vUOMs.clear();
        attributName.clear();
        doubleAttributWert.clear();

        anz = pFeature->getDoubleAttributWerte(attributName, doubleAttributWert, vUOMs);
        for (size_t i = 0; i < anz; i++)
        {

          if (attributName[i].find(L"name") != std::string::npos)
          {
            hasName = true;
            name = strAttributWert[i];
          }
          else if (attributName[i].find(L"value") != std::string::npos || attributName[i].find(L"wert") != std::string::npos)
          {
            hasValue = true;
            strValue = formatDoubleAttribute(doubleAttributWert[i], vUOMs[i]);
          }
        }

        if (hasName && hasValue)
        {
          PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(name).c_str()), PyUnicode_FromString(toUtf8(strValue).c_str()));
        }

        hasName = false;
        hasValue = false;
        attributName.clear();
        booleanAttributWert.clear();

        anz = pFeature->getBoolAttributWerte(attributName, booleanAttributWert);
        for (size_t i = 0; i < anz; i++)
        {
          if (attributName[i].find(L"name") != std::string::npos)
          {
            hasName = true;
            name = strAttributWert[i];
          }
          else if (attributName[i].find(L"value") != std::string::npos || attributName[i].find(L"wert") != std::string::npos)
          {
            hasValue = true;
            strValue = (booleanAttributWert[i] == true) ? L"True" : L"False";
          }
        }
      }
    }
  }

  if (dictObj)
  {
    return dictObj;
  }

  Py_RETURN_NONE;
}

PyObject* processAttributes(IfcDB::IfcEntity* pEntity)
{
  IfcDB::Attributes* pAttributes = pEntity->getAttributes();

  if (pAttributes != nullptr && pAttributes->hasAttributes() == true)
  {
    PyObject* dictObj = PyDict_New();

    for (auto pAttribute : pAttributes->getAttributes())
    {
      processAttributes(dictObj, pAttribute);
    }

    return dictObj;
  }

  Py_RETURN_NONE;
}

void processAttributes(PyObject* dictObj, const IfcDB::Attribute* pAttribute)
{
  if (pAttribute->getType() == IfcDB::AT_LIST)
  {
    const IfcDB::Attributes::ListAttribute* pListAttribute = dynamic_cast<const IfcDB::Attributes::ListAttribute*>(pAttribute);

    PyObject* subDictObj = PyDict_New();

    PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pAttribute->getKey()).c_str()), subDictObj);

    for (auto pAttribute : pListAttribute->getAttributes())
    {
      processAttributes(subDictObj, pAttribute);
    }
  }
  else
  {
    if (pAttribute->getType() == IfcDB::AT_INTERNALREFERENCE)
    {
      const IfcDB::Attributes::InternalReferenceAttribute* pInternalRefAttribute = dynamic_cast<const IfcDB::Attributes::InternalReferenceAttribute*>(pAttribute);
      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pAttribute->getKey()).c_str()), PyUnicode_FromString(toUtf8(pAttribute->getValue()).c_str()));
    }
    else if (pAttribute->getType() == IfcDB::AT_INTEGER)
    {
      std::wstring unit;

      const IfcDB::Attributes::IntAttribute* pIntAttribute = dynamic_cast<const IfcDB::Attributes::IntAttribute*>(pAttribute);

      //if (pIntAttribute->getUnit())
      //{
      //  unit = pIntAttribute->getUnit()->getFormatedStringWithUnit(pIntAttribute->getIntegerValue());
      //}

      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pAttribute->getKey()).c_str()), PyUnicode_FromString(toUtf8(pAttribute->getValue()).c_str()));
    }
    else if (pAttribute->getType() == IfcDB::AT_FLOAT)
    {
      std::wstring unit;

      const IfcDB::Attributes::DoubleAttribute* pDoubleAttribute = dynamic_cast<const IfcDB::Attributes::DoubleAttribute*>(pAttribute);

      //if (pDoubleAttribute->getUnit())
      //{
      //  unit = pDoubleAttribute->getUnit()->getFormatedStringWithUnit(pDoubleAttribute->getDoubleValue());
      //}

      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pAttribute->getKey()).c_str()), PyUnicode_FromString(toUtf8(pAttribute->getValue()).c_str()));
    }
    else
    {
      PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(pAttribute->getKey()).c_str()), PyUnicode_FromString(toUtf8(pAttribute->getValue()).c_str()));
    }
  }
}