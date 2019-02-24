#include "sneka/worldobject.hpp"

#include <string>



namespace {

	sneka::WorldObjectCounter shcounter;

}


namespace sneka {

	WorldObject::WorldObject(WorldObjectCounter& counter, Mesh& mesh):
			GridObject::GridObject(mesh),
			counter(counter),
			counter_shared(shcounter)
	{ }

	WorldObject::WorldObject(WorldObjectCounter& counter, std::string mesh_name):
			GridObject::GridObject(mesh_name),
			counter(counter),
			counter_shared(shcounter)
	{ }
}
