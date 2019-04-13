#include "sneka/chunk.hpp"

#include <fstream>

#pragma GCC warning "debug iostream"
#include <iostream>


namespace {

	std::string& ensure_leading_slash(std::string& path) {
		if(! (path[path.size()-1] == '/')) {
			path.resize(path.size()+1);
			path[path.size()-1] = '/';
		}
		return path;
	}

}


namespace sneka {

	FileChunkLoader::FileChunkLoader(
			LevelObjectLoader& objl,
			std::map<char, std::string> obj_id_map
	):
			obj_loader(objl), id_to_obj(std::move(obj_id_map))
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


namespace sneka {

	FolderChunkLoader::FolderChunkLoader(
			LevelObjectLoader& obj_l,
			std::map<char, std::string> obj_id_map,
			std::string dir, std::string prefix, std::string suffix
	):
			FileChunkLoader::FileChunkLoader(obj_l, std::move(obj_id_map)),
			prefix(ensure_leading_slash(dir) + prefix),
			suffix(suffix)
	{ }


	Chunk& FolderChunkLoader::get(Chunk::coord_t x, Chunk::coord_t z) {
		Chunk& got = FileChunkLoader::get(
				prefix +
				std::to_string(x) + "_" +
				std::to_string(z) +
				suffix );
		got.setOffset(x, z);
		return got;
	}
}
