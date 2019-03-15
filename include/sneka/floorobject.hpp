#ifndef SNEKA_FLOOROBJECT_HPP
#define SNEKA_FLOOROBJECT_HPP

#include "sneka/renderobject.hpp"
#include "sneka/snekaruntime.hpp"



namespace sneka {

	class FloorObject : public virtual RenderObject {
	public:
		const unsigned int side_length;
		FloorObject(
				const SnekaRuntime & runtime,
				const std::string & tile_mesh_name, int unsigned side_length );
		virtual ~FloorObject();
	};

}

#endif
