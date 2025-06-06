import ifcdb


def query_entities():
    document = ifcdb.get_document()

    for entity in document.get_entities():
        document.log_message(f"OID: {entity.oid}; Name: {entity.name}; Type:{entity.type}")


def query_selected_entities():
    document = ifcdb.get_document()

    if len(document.get_selected_entities()) == 0:
        document.log_message("No entity selected")
        
    for entity in document.get_selected_entities():

        document.log_message(f"OID: {entity.oid}; Name: {entity.name}; Type:{entity.type}")

        geometry = entity.get_geometry()

        if geometry != None:
            document.log_message(f"Geometry Type: {geometry.type}")
            
            faces = geometry.get_faces()

            for face in faces:
                
                document.log_message(f"Face normal: {face.get_normal()}")
                document.log_message(f"Face area: {face.calc_area()}")
                
                polygons = face.get_polygons()
                
                for polygon in polygons:
                    document.log_message(f"polygon: {polygon}")
        
        properties = entity.get_properties()
        
        if properties != None:
            for key, value in properties.items():
                document.log_message(f"key: {key}; value: {value}")       
        
        relations = entity.get_relations()

        if relations != None:
            for relation in relations:
                document.log_message(f"OID: {relation.oid}; Name: {relation.name}; Type:{relation.type}")       
        
        entityType = entity.get_type_object()
        
        if entityType != None:
            document.log_message(f"OID: {entityType.oid}; Name: {entityType.name}")
        
        document.log_message(f"Properties: {entity.get_properties()}")


def query_visible_entities():
    document = ifcdb.get_document()

    for entity in document.get_visible_entities():

        document.log_message(f"OID: {entity.oid}; Name: {entity.name}; Type:{entity.type}")

        document.log_message(f"GeometryInfo: {entity.get_geometry_info()}")
        
        document.log_message(f"Properties: {entity.get_properties()}")

