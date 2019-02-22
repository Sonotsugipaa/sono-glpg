#ifndef SNEKA_FLOOROBJECT_HPP
#define SNEKA_FLOOROBJECT_HPP

#include "sneka/renderobject.hpp"



namespace sneka {

	class FloorObject : public virtual RenderObject {
	protected:
	public:
		const unsigned int side_length;
		FloorObject(const std::string & tile_mesh_name, int unsigned side_length);
		virtual ~FloorObject();
	};

}

#endif
