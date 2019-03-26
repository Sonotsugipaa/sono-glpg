#ifndef SNEKA_CHUNK_HPP
#define SNEKA_CHUNK_HPP

#include "sneka/levelobject.hpp"

#include <map>

#include <glm/vec2.hpp>



namespace sneka {

	class FileChunkLoader;

	class Chunk : public Asset {
		friend FileChunkLoader;
	public:
		/* not actually a hash, but a long integer containing
		 * both coordinates - it does behave like a hash,
		 * but for low relative coordinates they can be
		 * assumed to be unique identifiers (which is always
		 * the case for any respectable chunk). */
		using hash_t = unsigned long long int;

		using coord_t = unsigned short;

	private:
		using ivec2 = glm::ivec2;
		using map_t = std::map<hash_t, LevelObject*>;

		map_t map;
		ivec2 offset;
		hash_t size;

		bool changed;

	protected:
		Chunk(std::string asset_name, ivec2 chunk_offset, hash_t size);
		Chunk(ivec2 chunk_offset, const Chunk & cpy);

	public:
		Chunk(Chunk&);
		Chunk(Chunk&&) = default;
		~Chunk();

		void add(ivec2, LevelObjectTemplate&);
		void add(coord_t, coord_t, LevelObjectTemplate&);

		void remove(coord_t, coord_t);
		void remove(ivec2);

		LevelObject* operator [] (ivec2);
		LevelObject* at(coord_t, coord_t);

		/* returns 'true' if and only if the chunk has been modified
		 * after being loaded or saved (i.e. if the ChunkLoader
		 * can create a new Chunk identical to this one given the
		 * same position). */
		bool stateChanged();

		inline hash_t getSize() const { return size; }

		inline const map_t & getObjectMap() const { return map; }

		static hash_t vecToHash(ivec2);
		static ivec2 hashToVec(hash_t);
	};


	class FileChunkLoader : public AssetLoader<Chunk> {
		friend Chunk;

	protected:
		LevelObjectLoader& obj_loader;
		std::map<char, std::string> id_to_obj;

		virtual Chunk* load(std::string file_name);

	public:
		FileChunkLoader(
				LevelObjectLoader& object_loader,
				const std::map<char, std::string> & object_id_map // will be copied
		);
	};

}

#endif
