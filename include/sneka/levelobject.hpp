#ifndef SNEKA_LEVELOBJECT_HPP
#define SNEKA_LEVELOBJECT_HPP

#include "sneka/gridobject.hpp"

#include "objcounter.tpp"



namespace sneka {

	using counter_t = unsigned int;
	using LevelObjectCounter = gla::ObjectCounter<counter_t>;

	class LevelObject : public GridObject {
	protected:
	public:
		const LevelObjectCounter counter;
		const LevelObjectCounter counter_shared;

		LevelObject(LevelObjectCounter&, Mesh&);
		virtual ~LevelObject() = default;
	};

}

#endif
