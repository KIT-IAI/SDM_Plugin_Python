#include "Face.hpp"
#include "Polyline.hpp"
#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();

extern PyObject* getPyList(const IfcDB::Point& point);
extern PyObject* getPyList(const IfcDB::Direction& direction);
extern PyObject* getPyList(std::vector<IfcDB::Point>& points);

PyObject* faceCalcArea(PyIfcdb::Face* face, void*)
{
  return PyFloat_FromDouble(face->face->calcArea());
}

PyObject* faceIsWithin(PyIfcdb::Face* face, PyObject* args)
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

  if (face->face)
  {
    // Matrix for projection
    IfcDB::Matrix4 matrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    IfcDB::ClipperCurve footprint(matrix, face->face);

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

PyObject* faceNormal(PyIfcdb::Face* face, void*)
{
  if (face->face != nullptr)
  {
    IfcDB::Direction faceNormal = face->face->getFaceNormal();

    const auto normalList = getPyList(faceNormal);

    return normalList;
  }

  Py_RETURN_NONE;
}

PyObject* faceGetPolygons(PyIfcdb::Face* face, void*)
{
  if (face->face != nullptr)
  {
    size_t nInnerLoops = face->face->getInnerLoops().size();

    PyObject* polygonList = PyList_New(nInnerLoops+1);

    // outer loop
    IfcDB::Polyloop* pOuterLoop = face->face->getOuterLoop();

    const auto pointList = getPyList(pOuterLoop->getPoints());

    PyList_SetItem(polygonList, 0, (PyObject*)pointList);

    // inner loops

    for (size_t iInnerLoop = 0; iInnerLoop < face->face->getInnerLoops().size(); ++iInnerLoop)
    {
      IfcDB::Polyloop* pInnerLoop = face->face->getInnerLoops()[iInnerLoop];

      for (size_t iPnt = 0; iPnt < pInnerLoop->getPoints().size(); ++iPnt)
      {
        const auto pointList = getPyList(pInnerLoop->getPoints());

        PyList_SetItem(polygonList, iInnerLoop+1, (PyObject*)pointList);
      }
    }

    return polygonList;
  }

  Py_RETURN_NONE;
}

PyObject* faceGetOuterLoop(PyIfcdb::Face* face, void*)
{
  if (face->face != nullptr)
  {
    auto& state = getIfcDBState();

    // outer loop
    auto item = (PyIfcdb::Polyline*)state.types.polyline->tp_alloc(state.types.polyline, 0);
    item->curve = face->face->getOuterLoop();

    return (PyObject*)item;
  }

  Py_RETURN_NONE;
}

PyObject* faceGetOuterLoopCoordinates(PyIfcdb::Face* face, void*)
{
  if (face->face != nullptr)
  {
    // outer loop
    IfcDB::Polyloop* pOuterLoop = face->face->getOuterLoop();

    const auto pointList = getPyList(pOuterLoop->getPoints());

    return pointList;
  }

  Py_RETURN_NONE;
}

PyObject* faceGetInnerLoops(PyIfcdb::Face* face, void*)
{
  if (face->face != nullptr)
  {
    auto& state = getIfcDBState();

    size_t nInnerLoops = face->face->getInnerLoops().size();

    PyObject* polylineList = PyList_New(nInnerLoops);

    // inner loops
    for (size_t iInnerLoop = 0; iInnerLoop < face->face->getInnerLoops().size(); ++iInnerLoop)
    {
      IfcDB::Polyloop* pInnerLoop = face->face->getInnerLoops()[iInnerLoop];

      auto item = (PyIfcdb::Polyline*)state.types.polyline->tp_alloc(state.types.polyline, 0);
      item->curve = pInnerLoop;

      PyList_SetItem(polylineList, iInnerLoop, (PyObject*)item);
    }

    return polylineList;
  }

  Py_RETURN_NONE;
}

PyObject* faceGetInnerLoopsCoordinates(PyIfcdb::Face* face, void*)
{
  if (face->face != nullptr)
  {
    size_t nInnerLoops = face->face->getInnerLoops().size();

    PyObject* polylineList = PyList_New(nInnerLoops);

    // inner loops

    for (size_t iInnerLoop = 0; iInnerLoop < face->face->getInnerLoops().size(); ++iInnerLoop)
    {
      IfcDB::Polyloop* pInnerLoop = face->face->getInnerLoops()[iInnerLoop];

      const auto pointList = getPyList(pInnerLoop->getPoints());

      PyList_SetItem(polylineList, iInnerLoop + 1, (PyObject*)pointList);
    }

    return polylineList;
  }

  Py_RETURN_NONE;
}