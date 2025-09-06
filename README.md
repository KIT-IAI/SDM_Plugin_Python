# Pyhon Plugin

The [KITModelViewer](https://github.com/KIT-IAI/SDM_KITModelViewer) Python API is based on the open plugin interface. 
To be able to create the Python plugin, the [plugin SDK](https://github.com/KIT-IAI/SDM_Plugin_SDK) is required. Simply copy the content of the current SDK version into the "ifcexplorer_python3" folder.

## Dependencies

### Use of vcpkg:

|Package Name         |Install Command                             |
|:---                 |:---                                        |
|python3              |vcpkg install python3 triplet=x64-windows   |
|wxwidgets            |vcpkg install wxwidgets triplet=x64-windows |
|fmt                  |vcpkg install fmt triplet=x64-windows       |


## Python Interface

### User Interface
  
The integration of own Python scripts is done via the file "scripts/setup.py". 
The "register" function from the "plugin" module is used to link a menu item to a Python script.

<u>Example setup.py:</u>
```
import plugin
import ifcdb_query

plugin.register("Get entity info for all entities", "", ifcdb_query.query_entities)
plugin.register("Get entity info of selected entities", "", ifcdb_query.query_selected_entities)
plugin.register("Get entity info of visible entities", "", ifcdb_query.query_visible_entities)
```

### Document: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|load_data(filename)                    |Load data from a file (if the file format is supported) |
|get_application_settings()             |Returns a dictionary containing application specific settings |
|get_entities()                         |Returns a list of all Entities |
|get_entities_by_type(type)             |Returns a list of all entities of the specified type |
|get_entity_by_oid(oid)                 |Returns the entity with the given OID |
|create_feature(gmlID, featureType)     |Creates a new feature, GML ID and feature type is passed |
|create_entity(entityType)              |Crates a new entity, entity typ is passed |
|log_message(message)                   |Creates a new entry in the live log window |
|srs_transform(coordinates, sourceSrs, targetSrs) |Transforms the coordinates of a point (see [point definition](#geometry-structures) below), sourceSrs and targetSrs are specified by the EPSG code as string |
|get_selected_entities()                |Returns a list of selected entities |
|get_visible_entities()                 |Returns a list of visible entities |
|get_data_path()                        |Returns the path to the application data folder |
|get_script_path()                      |Returns the path to the current Python script |
|get_file_path()                        |Returns the path to the currently loaded file |
|get_latlon_boundingbox()               |Returns the model bounding box in latitude / longitude (see [point definition](#geometry-structures))|
|get_reference_point()               	|Returns the coordinates of the model reference point according to the EPSG of the default SRS (see [point definition](#geometry-structures))|
|get_default_srs()               	|Returns the EPSG code of the default SRS|

### Entity: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|id [get]               |internal objekt ID             |integer  |
|oid [get]              |Objekt ID                      |integer  |
|guid [get]             |Global unique ID               |string   |
|name [set; get]        |Name                           |string   |
|description [set; get] |Description                    |string   |
|class_type [get]       |Entity type                    |string   |
|type [get]             |Internal entity type           |string   |
|object_type [set; get] |Object type                    |string   |
|layer [set; get]       |CAD layer name                 |string   |
|color [set; get]       |Entity color as list<br> [red, green, blue, alpha]|integer list|
|long_name [set; get]   |IFC spezific attribute for e.g. spaces|string|

### Entity: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_string_attribute(attributeName)    |Returns the value of a GML string attribute |
|set_parent(parentOid)                  |Sets the parent-child relationship of the current entity. The OID of the parent entity is passed |
|find_property_value(propertySet, propertyName) |Returns the value as sting for the passed property set and property name (e.g. "Pset_WallCommon", "ThermalTransmittance") |
|get_properties()                       |Returns a dictionary structur containing all properties |
|get_attributes()                       |Returns a dictionary structur containing all attributes |
|get_type_object()                      |Returns the IfcTypeObject releted by IfcRelDefinesByType |
|get_relations(relationType='')         |Returns a list of all relations or the specified relation type only |
|get_model_type()                       |Returns the the model / format or service type of the object |
|get_model_info()                       |Returns information about the model / format or service type of the object |
|get_geometry_info()                    |Returns the geometry structure assigned to the object |
|get_geometry()                         |Returns the related geometry objects |
|calc_quantities()                      |Calculates geometric parameters like volume, center of gravity, extension, ... and provides them in a dictionary |
|add_string_attribute(name, value)      |Add new string attribute |
|add_date_attribute(name, year, month, day) |Add new data attribute |
|add_geometry_polyline(coordinates)     |A polyline is generated from a passed list of coordinate values (see [Geometry Structures](#geometry-structures) below) |
|add_geometry_polygon(coordinates)      |A polygon (face) is generated from a passed list of coordinate values (see [Geometry Structures](#geometry-structures) below)|
|is_point_within(coordinates)           |Checks whether a point is located within the projected geometry (see [point definition](#geometry-structures) below) |
|get_boundingbox()                      |Returns the bounding box of the entity|
|get_children()                         |Returns a list of children |
|get_children_by_type(type)             |Returns a list of children of the specified type |

### EntityType: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|id [get]               |internal objekt ID             |integer  |
|oid [get]              |Objekt ID                      |integer  |
|guid [get]             |Global unique ID               |string   |
|name [set; get]        |Name                           |string   |
|description [set; get] |Description                    |string   |

### EntityType: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_properties()                       |Returns a dictionary structur containing all properties related by IfcRelDefinesByProperty |
|get_attributes()                       |Returns a dictionary structur containing all attributes |
|get_relations(relationType='')         |Returns a list of all relations or the specified relation type only |

### Relation: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|id [get]               |internal objekt ID             |integer  |
|oid [get]              |Objekt ID                      |integer  |
|guid [get]             |Global unique ID               |string   |
|name [set; get]        |Name                           |string   |
|description [set; get] |Description                    |string   |
|type [get]             |Relation type                  |string   |

### Relation: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_attributes()                       |Returns a dictionary structur containing all attributes |

### Geometry: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|type [get]             | Geometry type                 |string   |
|data [get]             | Geometry data                 |string   |

### Geometry: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_faces                              |Returns a list of face objects |
|srs_transform(ourceSrs, targetSrs)     |Transforms the geometry, sourceSrs and targetSrs are specified by the EPSG code as string|
|is_within(coordinate)                  |Checks whether a point is located within the projected geometry (see [point definition](#geometry-structures) below) |
|get_boundingbox()                      |Returns the bounding box of the geometry|

### Face: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |

### Face: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_normal                             |Returns the face normal |
|get_polygons                           |Returns a list structure containing the coordinates (see [Geometry Structures](#geometry-structures) definition below) |
|get_outer_loop                         |Returns the outer loop as a polyline object |
|get_outer_loop_coordinates             |Returns a list structure containing the coordinates of the outerr loop (see [Geometry Structures](#geometry-structures) definition below) |
|get_inner_loops                        |Returns the inner loops as a list of polyline objects  |
|get_inner_loops_coordinates            |Returns a list structure containing the coordinates of the inner loops (see [Geometry Structures](#geometry-structures) definition below) |
|calc_area                              |Returns the area value of the face geometry |
|is_within(coordinate)                  |Checks whether a point is located within the face contour (see [point definition](#geometry-structures) below) |

### Polyline: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |

### Polyline: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|is_closed                              |Check weather the first and last point are the same |
|is_within(coordinate)                  |Checks whether a point is located within the polyline (see [point definition](#geometry-structures) below) |
|intersects_with(polyline)              |checks whether a polyline intersects with an other polyline |
|get_coordinates                        |Returns a list containing the coordinates (see [Geometry Structures](#geometry-structures) definition below) |

### Geometry Structures

* Point   
2D [x,y] or 3D [x,y,z] cordinate tupel

* Polyline  
List of points [ [x,y], [x,y], [x,y], [x,y] ]

* Polygon / Face  
List of points. If several lists are passed, the first list is interpreted as an ${\color{blue} outer \space contour}$ and all further lists are ${\color{green} inner \space contours}$.<br>
[ ${\color{blue} [ \space [x,y], [x,y], [x,y], [x,y] \space ] \color{black}, \color{green} [ \space [x,y], [x,y], [x,y], [x,y] \space ] \color{black}, \color{green} [ \space [x,y], [x,y], [x,y], [x,y] \space ]}$ ]

## How to cite

```bibtex
@software{SDM_Plugin_Python,
	title        = {SDM_Plugin_Python},
	author       = {{Steffen Hempel, Andreas Geiger}},
	url          = {https://github.com/KIT-IAI/SDM_Plugin_Python},
	date         = {2023}
}
```








