import plugin

import ifcdb_query

plugin.register("Get entity info for all entities", "", ifcdb_query.query_entities)
plugin.register("Get entity info of selected entities", "", ifcdb_query.query_selected_entities)
plugin.register("Get entity info of visible entities", "", ifcdb_query.query_visible_entities)
