#include "Polyline.hpp"
#include "ModuleDocument.hpp"
#include "IfcDBState.hpp"
#include <stdutils/utf8.hpp>

extern IfcDBState& getIfcDBState();

extern PyObject* getPyList(const IfcDB::Point& point);
extern PyObject* getPyList(const IfcDB::Direction& direction);
extern PyObject* getPyList(std::vector<IfcDB::Point>& points);

PyObject* polylineIsWithin(PyIfcdb::Polyline* polyline, PyObject* args)
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
    return PyList_New(0);
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

  if (polyline->curve)
  {
    // Matrix for projection
    IfcDB::Matrix4 matrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    IfcDB::ClipperCurve footprint(matrix, polyline->curve);

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

PyObject* polylineIntersects(PyIfcdb::Polyline* polyline, PyObject* args)
{
  PyObject* objPolyline;

  const auto r = PyArg_ParseTuple(args, "O", &objPolyline);

  if (r == 0)
  {
    // PyArg_ParseTuple raises an exception if parsing the tuple fails
    return nullptr;
  }

  if (polyline->curve)
  {
    const PyIfcdb::Polyline* otherPyPolyline = (PyIfcdb::Polyline*)objPolyline;

    bool overlaps(false);
    std::vector<IfcDB::Point> points;
    
    std::unique_ptr<IfcDB::CPolyline> basePolyline{ IfcDB::Curve::toPolyline(polyline->curve) };
    std::unique_ptr<IfcDB::CPolyline> otherPolyline{ IfcDB::Curve::toPolyline(otherPyPolyline->curve) };
    
    size_t count = basePolyline->intersectWith(*otherPolyline, points, overlaps);

    if (count > 0)
    {
      Py_RETURN_TRUE;
    }

  }

  Py_RETURN_FALSE;
}

PyObject* polylineIsClosed(PyIfcdb::Polyline* polyline, void*)
{
  if (polyline->curve != nullptr)
  {
    if (polyline->curve->isClosed())
    {
      Py_RETURN_TRUE;
    }
  }

  Py_RETURN_FALSE;
}

PyObject* polylineGetCoordinates(PyIfcdb::Polyline* polyline, void*)
{
  PyObject* pointList = nullptr;

  if (polyline->curve != nullptr)
  {
    switch (polyline->curve->getType())
    {
      case IfcDB::POLYLINE:
      {
        const auto pPolyline = dynamic_cast<IfcDB::CPolyline*>(polyline->curve);
        pointList = getPyList(pPolyline->getPoints());
        break;
      }
      case IfcDB::POLYLOOP:
      {
        const auto pPolyloop = dynamic_cast<IfcDB::Polyloop*>(polyline->curve);
        pointList = getPyList(pPolyloop->getPoints());
        break;
      }
      default:
        break;
    }
  }

  if (pointList)
  {
    return pointList;
  }

  Py_RETURN_NONE;
}

