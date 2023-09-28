# Pyhon PlugIn

## Dependencies

### Use of vcpkg:

|Package Name         |Install Command                            |
|:---                 |:---                                       |
|python3              |vcpkg install python3 triplet=x64-windows  |
|catch2               |vcpkg install catch2 triplet=x64-windows   |


## Python Interface

### User Interface
  
The integration of own Python scripts is done via the file "scripts/setup.py". 
The "register" function from the "plugin" module is used to link a menu item to a Python script.

<u>Example setup.py:</u>
```
import plugin

import ifcdb_query

plugin.register("Get Entities", ifcdb_query.queryEntities)  
plugin.register("Get CityGML Buildings", ifcdb_query.queryCityGMLBuildings)  
plugin.register("Get CityGML Building Function", ifcdb_query.queryCityGMLBuildingFunction)  
plugin.register("Create Feature test", ifcdb_query.createFeature)  
```

### Document: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_entities()                         |Returns a list of all Entities |
|get_entities_by_type(type)             |Returns a list of all entities of the specified type |
|get_entity_by_oid(oid)                 |Returns the entity with the given OID |
|create_feature(gmlID, featureType)     |Creates a new feature, GML ID and feature type is passed |
|create_entity(entityType)              |Crates a new entity, entity typ is passed |
|log_message(message)                   |Creates a new entry in the live log window |
|srs_transform(coordinates, sourceSrs, targetSrs) |Transforms the coordinates of a point (see point definition below), sourceSrs and targetSrs are specified by the EPSG code as string|
|get_selected_entities()                |Returns a list of selected entities |
|get_visible_entities()                 |Returns a list of visible entities |

### Entity: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
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
|add_geometry_polyline(coordinates)     |A polyline is generated from a passed list of coordinate values |
|add_geometry_polygon(coordinates)      |A polygon (face) is generated from a passed list of coordinate values |
|calc_quantities()                      |Calculates geometric parameters like volume, center of gravity, extension, ... and provides them in a dictionary |
|set_parent(parentOid)                  |Sets the parent-child relationship of the current entity. The OID of the parent entity is passed |
|find_property_value(propertySet, propertyName) |Returns the value as sting for the passed property set and property name (e.g. "Pset_WallCommon", "ThermalTransmittance") |
|get_properties()                       |Returns a dictionary structur containing all properties related by IfcRelDefinesByProperty |
|get_relations(relationType='')         |Returns a list of all relations or the specified relation type only |
|get_geometry_info()                    |Returns the geometry structure assigned to the object |
|get_model_type()                       |Returns the the model / format / service type for the object |
|get_model_info()                       |Returns the model information details |

### EntityType: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|oid [get]              |Objekt ID                      |integer  |
|guid [get]             |Global unique ID               |string   |
|name [set; get]        |Name                           |string   |
|description [set; get] |Description                    |string   |

### EntityType: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |
|get_properties()                       |Returns a dictionary structur containing all properties related by IfcRelDefinesByProperty |
|get_relations(relationType='')         |Returns a list of all relations or the specified relation type only |

### Relation: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|oid [get]              |Objekt ID                      |integer  |
|guid [get]             |Global unique ID               |string   |
|name [set; get]        |Name                           |string   |
|description [set; get] |Description                    |string   |
|type [get]             |Relation type                  |string   |

### Relation: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |

### Geometry: Attributes

|Attribute              |Description                    |Type     |
|:---                   |:---                           |:---     |
|type [get]             | Geometry type                 |string   |

### Geometry: Methods

|Method                                 |Description                      |
|:---                                   |:---                             |

### Geometry Structure

* Point   
2D [x,y] or 3D [x,y,z] cordinate tupel

* Polyline  
List of points [ [x,y], [x,y], [x,y], [x,y] ]

* Polygon / Face  
List of points. If several lists are passed, the first list is interpreted as an ${\color{blue}outer}$ ${\color{blue}contour}$ and all further lists are ${\color{green}inner}$ ${\color{green}contours}$
${\color{blue}[ [x,y], [x,y], [x,y], [x,y] ]}$ ${\color{green}[ [x,y], [x,y], [x,y], [x,y] ] [ [x,y], [x,y], [x,y], [x,y] ]}$
