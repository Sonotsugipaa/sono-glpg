#include "sneka/levelobject.hpp"

#include <string>



namespace {

	sneka::LevelObjectCounter shcounter;

}


namespace sneka {

	LevelObject::LevelObject(LevelObjectCounter& counter, Mesh& mesh):
			GridObject::GridObject(mesh),
			counter(counter),
			counter_shared(shcounter)
	{ }

	LevelObject::LevelObject(LevelObjectCounter& counter, std::string mesh_name):
			GridObject::GridObject(mesh_name),
			counter(counter),
			counter_shared(shcounter)
	{ }
}
