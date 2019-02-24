#ifndef SNEKA_WORLDOBJECT_HPP
#define SNEKA_WORLDOBJECT_HPP

#include "sneka/gridobject.hpp"

#include "objcounter.tpp"



namespace sneka {

	using counter_t = unsigned int;
	using WorldObjectCounter = gla::ObjectCounter<counter_t>;

	class WorldObject : public GridObject {
	protected:
	public:
		const WorldObjectCounter counter;
		const WorldObjectCounter counter_shared;

		WorldObject(WorldObjectCounter&, Mesh&);
		WorldObject(WorldObjectCounter&, std::string);
		virtual ~WorldObject() = default;
	};

}

#endif
