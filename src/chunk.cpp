#include "sneka/chunk.hpp"

#include <fstream>

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

	Chunk::Chunk(Chunk& cpy):
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
		auto iter = map.find(vecToHash(pos));
		if(iter != map.end()) {
			delete iter->second;
			map.erase(iter);
		}
	}



	LevelObject* Chunk::operator [] (ivec2 pos) {
		auto iter = map.find(vecToHash(pos));
		if(iter == map.end())
			return nullptr;
		else
			return iter->second;
	}

	LevelObject* Chunk::at(coord_t x, coord_t z) {
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


namespace sneka {

	FileChunkLoader::FileChunkLoader(
			LevelObjectLoader& objl,
			const std::map<char, std::string> & obj_id_map
	):
			obj_loader(objl), id_to_obj(obj_id_map)
	{ }


	Chunk* FileChunkLoader::load(std::string file_name) {
		#pragma GCC warning "temporary loader; invalid files may be buggy, also the design is just bad"

		char c; // Used for a read operation

		std::ifstream is = std::ifstream(file_name);
		if(! is.is_open())
			throw AssetLoadException(file_name, "could not open the file");

		// Get the size of the file, and create the buffer
		is.seekg(0, std::ios_base::end);
		const unsigned int size = is.tellg();
		char* buffer = new char[size];
		is.seekg(0, std::ios_base::beg);

		// Scan the first line to determine the actual length of a row/column
		unsigned int line__size = 0;
		while(is.get(c) && (c != '\n')) {
			switch(c) {
			case '\n':  break;
			case '\r':  continue;
			case  ' ':  continue;
			default:    buffer[line__size++] = c;
			}
		}

		const unsigned int line_size = line__size;

		/* If the file is valid, the side length of the chunk is now known.
		 * Perform arbitrary assertions on the two known sizes;
		 * if they are successful, initialize the chunk */
		const unsigned int line_size_sq = line_size * line_size;
		if(line_size > size) {
			throw AssetLoadException(file_name, "invalid file/line size " + std::to_string(line_size_sq) + ", " + std::to_string(size));
		}
		/* The return value is move-constructed afterwards and not
		 * new-allocated right away, because any exception after this
		 * next line will have to destroy the whole object.
		 * Less efficient, but less i-want-to-kill-myselfy. */
		Chunk retn = Chunk(file_name, Chunk::ivec2(), line_size);

		// Read the rest
		unsigned int i = line_size;
		unsigned int line = 0;

		while((is >> c) && (i < line_size_sq)) {
			switch(c) {
			case '\r':
				continue;
			case '\n':
				if(i % line_size != 0) {
					throw AssetLoadException(
							file_name,
							"invalid line size ("+std::to_string(line)+")" );
				} else
				if(line >= line_size) {
					throw AssetLoadException(
							file_name,
							"invalid file size - the chunk must be a square" );
				} else {
					++line;
				}
				break;
			default:
				buffer[i++] = c;
				break;
			}
		}

		std::map<char, std::string>::iterator iter;
		for(unsigned int z=0; z < line_size; ++z) {
			for(unsigned int x=0; x < line_size; ++x) {
				iter = id_to_obj.find(buffer[x+(z*line_size)]);
				if(iter != id_to_obj.end()) {
					retn.add(x, z, obj_loader.get(iter->second));
				}
			}
		}

		return new Chunk(std::move(retn));
	}

}
