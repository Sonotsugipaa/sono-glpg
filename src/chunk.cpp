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


	void Chunk::set(Chunk::ivec2 pos, LevelObject* o) {
		hash_t hash = vecToHash(pos);
		if(o == nullptr) {
			map.erase(hash);
			//std::cout << "[chunk] erased " << absPosition(pos)[0] << ", " << absPosition(pos)[1] << std::endl;
		} else {
			map[hash] = o;
			o->setGridPosition(absPosition(pos));
			//std::cout << "[chunk] set " << o << " at " << absPosition(pos)[0] << ", " << absPosition(pos)[1] << std::endl;
		}
	}

	LevelObject* Chunk::at_nconst(Chunk::ivec2 pos) const {
		auto iter = map.find(vecToHash(pos));
		if(iter == map.end())
			return nullptr;
		else
			return iter->second;
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

	void Chunk::swap(ivec2 p1, ivec2 p2) {
		changed = true;
		LevelObject* o1 = at_nconst(p1);
		LevelObject* o2 = at_nconst(p2);

		set(p2, o1);
		set(p1, o2);
	}

	bool Chunk::swap(ivec2 src, ivec2 dest, Chunk& dest_c) {
		changed = true;

		if(! (isInside(src) && dest_c.isInside(dest)))
			return false;

		src = relPosition(src);
		dest = dest_c.relPosition(dest);
		LevelObject* src_o = at_nconst(src);
		LevelObject* dest_o = dest_c.at_nconst(dest);

		set(src, nullptr);
		set(dest, dest_o);
		dest_c.set(src, src_o);
		dest_c.set(dest, nullptr);

		return true;
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


	const LevelObject * Chunk::operator [] (ivec2 pos) const {
		return at_nconst(pos);
	}

	const LevelObject * Chunk::at(coord_t x, coord_t z) const {
		return Chunk::operator [] (ivec2(x, z));
	}


	bool Chunk::isInside(ivec2 pos) const {
		pos = relPosition(pos);
		return
				(pos[0] >= 0) && (pos[0] < static_cast<coord_t>(size)) &&
				(pos[1] >= 0) && (pos[1] < static_cast<coord_t>(size));
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
