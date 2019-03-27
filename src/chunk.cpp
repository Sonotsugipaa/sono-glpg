#include "sneka/chunk.hpp"

/*
#pragma GCC warning "debug iostream"
#include <iostream>
*/



constexpr sneka::Chunk::hash_t hash_mask(
		sneka::Chunk::hash_t beg,
		sneka::Chunk::hash_t end
) {
	sneka::Chunk::hash_t mask = 0;
	for(sneka::Chunk::hash_t i=beg; i<end; ++i)
		mask |= (sneka::Chunk::hash_t(1) << i);
	return mask;
}
constexpr const sneka::Chunk::hash_t hash_shift = sizeof(sneka::Chunk::hash_t) * 4;
constexpr const sneka::Chunk::hash_t hash_mask1 = hash_mask(
		0,
		sizeof(sneka::Chunk::hash_t) * 4);
constexpr const sneka::Chunk::hash_t hash_mask2 = hash_mask(
		sizeof(sneka::Chunk::hash_t) * 4,
		sizeof(sneka::Chunk::hash_t) * 8 );


namespace sneka {

	Chunk::Chunk(std::string asset_name, Chunk::ivec2 off, Chunk::hash_t size):
			Asset::Asset(asset_name),
			offset(off), size(size), changed(false)
	{ }

	Chunk::Chunk(Chunk::ivec2 off, const Chunk & cpy):
			Asset::Asset(cpy.asset_name),
			map(cpy.map),
			offset(off), size(cpy.size), changed(true)
	{ }

	Chunk::Chunk(const Chunk & cpy):
			Asset::Asset(cpy.asset_name),
			map(cpy.map),
			offset(cpy.offset), size(cpy.size), changed(cpy.changed)
	{
		// Hard copy
		auto iter = cpy.map.begin();
		while(iter != cpy.map.end()) {
			map[iter->first] = iter->second;
			++iter;
		}
	}

	Chunk::~Chunk() {
		auto iter = map.begin();
		while(iter != map.end()) {
			delete static_cast<LevelObject*>(iter->second);
			map.erase(iter);
			iter = map.begin();
		}
	}


	void Chunk::add(Chunk::ivec2 pos, LevelObjectTemplate& obj_template) {
		changed = true;
		hash_t hash = vecToHash(pos);
		LevelObject obj = static_cast<LevelObject>(obj_template);
		obj.setGridPosition(pos);
		LevelObject* ptr = new LevelObject(std::move(obj));
		map[hash] = ptr;
	}

	void Chunk::add(Chunk::coord_t x, Chunk::coord_t z, LevelObjectTemplate& obj) {
		add(ivec2(x, z), obj);
	}

	void Chunk::remove(Chunk::coord_t x, Chunk::coord_t z) {
		remove(ivec2(x, z));
	}

	void Chunk::remove(Chunk::ivec2 pos) {
		changed = true;
		auto iter = map.find(vecToHash(pos));
		if(iter != map.end()) {
			delete iter->second;
			map.erase(iter);
		}
	}


	void Chunk::setOffset(Chunk::ivec2 off) {
		changed = true;
		Chunk::ivec2 delta = Chunk::ivec2(size) * (off - offset);
		offset = off;
		auto iter = map.begin();
		while(iter != map.end()) {
			iter->second->setGridPosition(iter->second->getGridPosition() + delta);
			++iter;
		}
	}

	void Chunk::setOffset(Chunk::coord_t x, Chunk::coord_t z) {
		setOffset(ivec2(x, z));
	}


	const LevelObject * Chunk::operator [] (ivec2 pos) {
		auto iter = map.find(vecToHash(pos));
		if(iter == map.end())
			return nullptr;
		else
			return iter->second;
	}

	const LevelObject * Chunk::at(coord_t x, coord_t z) {
		return Chunk::operator [] (ivec2(x, z));
	}


	Chunk::hash_t Chunk::vecToHash(Chunk::ivec2 pos) {
		return
				static_cast<hash_t>(pos[0]) |
				(static_cast<hash_t>(pos[1]) << hash_shift);
	}
	Chunk::ivec2 Chunk::hashToVec(Chunk::hash_t pos) {
		return Chunk::ivec2(pos & hash_mask1, (pos >> hash_shift) & hash_mask2);
	}

}
