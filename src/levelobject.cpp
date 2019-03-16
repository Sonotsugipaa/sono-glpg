#include "sneka/levelobject.hpp"

#include <string>



namespace {

	sneka::LevelObjectCounter shcounter;

}


namespace sneka {

	LevelObject::LevelObject(
			LevelObjectCounter& counter, Mesh& mesh
	):
			GridObject::GridObject(mesh),
			counter(counter),
			counter_shared(shcounter)
	{ }
}
