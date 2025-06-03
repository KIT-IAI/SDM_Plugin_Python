#include "Geometry.hpp"
#include "Face.hpp"
#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();

PyObject* getGeometryType(PyIfcdb::Geometry* geometry, void*)
{
  return PyUnicode_FromString(toUtf8(getTypeOfGeometryAsString(geometry->geometry)).c_str());
}

PyObject* getPyList(const IfcDB::Point& point)
{
  PyObject* coordListObj = PyList_New(0);

  PyList_Append(coordListObj, PyFloat_FromDouble(point.x));
  PyList_Append(coordListObj, PyFloat_FromDouble(point.y));
  PyList_Append(coordListObj, PyFloat_FromDouble(point.z));

  return coordListObj;
}

PyObject* getPyList(const IfcDB::Direction& direction)
{
  PyObject* coordListObj = PyList_New(0);

  PyList_Append(coordListObj, PyFloat_FromDouble(direction.x));
  PyList_Append(coordListObj, PyFloat_FromDouble(direction.y));
  PyList_Append(coordListObj, PyFloat_FromDouble(direction.z));

  return coordListObj;
}

PyObject* getPyList(std::vector<IfcDB::Point>& points)
{
  PyObject* pointListObj = PyList_New(0);

  for (const IfcDB::Point& point : points)
  {
    PyList_Append(pointListObj, getPyList(point));
  }

  return pointListObj;
}

PyObject* getGeometryData(PyIfcdb::Geometry* geometry, void*)
{
  if (geometry->geometry != nullptr)
  {
    PyObject* geometryPy = nullptr;

    PyObject* geometryListPy = PyList_New(0);
    PyList_Append(geometryListPy, PyUnicode_FromString(toUtf8(getTypeOfGeometryAsString(geometry->geometry)).c_str()));

    auto& state = getIfcDBState();

    switch (geometry->geometry->getType())
    {
      case IfcDB::VERTEX:
      {
        IfcDB::cVertex* pVertex = dynamic_cast<IfcDB::cVertex*>(geometry->geometry);

        geometryPy = PyList_New(0);

        PyList_Append(geometryPy, PyFloat_FromDouble(pVertex->getx()));
        PyList_Append(geometryPy, PyFloat_FromDouble(pVertex->gety()));
        PyList_Append(geometryPy, PyFloat_FromDouble(pVertex->getz()));

        break;
      }
      case IfcDB::LINE:
      {
        IfcDB::Line* pLine = dynamic_cast<IfcDB::Line*>(geometry->geometry);

        IfcDB::Points points{ pLine->getStartPoint(), pLine->getStartPoint() + pLine->getDirection() };

        geometryPy = getPyList(points);

        break;
      }

      case IfcDB::POLYLINE:
      {
        IfcDB::CPolyline* pPolyline = dynamic_cast<IfcDB::CPolyline*>(geometry->geometry);

        geometryPy = getPyList(pPolyline->getPoints());

        break;
      }
      default:
        break;
    }

    if (geometryPy != nullptr)
    {
      PyList_Append(geometryListPy, geometryPy);
    }

    return geometryListPy;
  }

  Py_RETURN_NONE;
}

PyObject* geometrySrsTransform(PyIfcdb::Geometry* geometry, PyObject* args)
{
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

//PyObject* calcQuantities(PyIfcdb::Geometry* geometry, PyObject* args)
//{
//  IfcDB::cEntityQuantities calculator();
//  calculator.calcQuantities(geometry->geometry);
//  auto& quantitiesMap = calculator.getQuantities();
//
//  PyObject* dictObj = PyDict_New();
//  for (auto& it : quantitiesMap)
//  {
//    const IfcDB::Quantity& quantity = it.second;
//
//    PyDict_SetItem(dictObj, PyUnicode_FromString(toUtf8(it.first).c_str()), PyUnicode_FromString(toUtf8(std::to_wstring(quantity.m_value)).c_str()));
//  }
//
//  return dictObj;
//}

PyObject* geometryIsWithin(PyIfcdb::Geometry* geometry, PyObject* args)
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

  if (geometry->geometry)
  {
    // Matrix for projection
    IfcDB::Matrix4 matrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    IfcDB::ClipperCurve footprint(matrix, (IfcDB::Curve*)nullptr);

    std::vector<IfcDB::Face*> vFaces;
    IfcDB::Geometry::getFaces(geometry->geometry, vFaces);

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

PyObject* geometryFaceNormals(PyIfcdb::Geometry* geometry, void*)
{
  if (geometry->geometry != nullptr)
  {
    std::vector<IfcDB::Face*> faces;
    IfcDB::Geometry::getFaces(geometry->geometry, faces);

    if (!faces.empty())
    {
      PyObject* list = PyList_New(faces.size());

      for (size_t i = 0; i < faces.size(); ++i)
      {
        IfcDB::Direction faceNormal = faces[i]->getFaceNormal();

        const auto normalList = PyList_New(3);

        PyList_SetItem(normalList, 0, (PyObject*)PyFloat_FromDouble(faceNormal.x));
        PyList_SetItem(normalList, 1, (PyObject*)PyFloat_FromDouble(faceNormal.y));
        PyList_SetItem(normalList, 2, (PyObject*)PyFloat_FromDouble(faceNormal.z));

        PyList_SetItem(list, i, (PyObject*)normalList);
      }

      return list;
    }
  }

  Py_RETURN_NONE;
}

PyObject* geometryFaces(PyIfcdb::Geometry* geometry, void*)
{
  if (geometry->geometry != nullptr)
  {
    std::vector<IfcDB::Face*> faces;
    IfcDB::Geometry::getFaces(geometry->geometry, faces);

    if (!faces.empty())
    {
      auto& state = getIfcDBState();

      PyObject* faceList = PyList_New(faces.size());

      for (size_t iFace = 0; iFace < faces.size(); ++iFace)
      {
        auto item = (PyIfcdb::Face*)state.types.face->tp_alloc(state.types.face, 0);
        item->face = faces[iFace];

        PyList_SetItem(faceList, iFace, (PyObject*)item);
      }

      return faceList;
    }
  }

  Py_RETURN_NONE;
}

PyObject* getGeometryBoundingBox(PyIfcdb::Geometry* geometry, void* args)
{
  PyObject* objPoint;

  IfcDB::Point min, max;

  if (geometry->geometry->getBoundingBox(min, max))
  {
    PyObject* listObj = PyList_New(0);

    PyList_Append(listObj, getPyList(min));
    PyList_Append(listObj, getPyList(max));

    return listObj;
  }

  Py_RETURN_NONE;
}