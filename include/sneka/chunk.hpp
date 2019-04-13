#ifndef SNEKA_CHUNK_HPP
#define SNEKA_CHUNK_HPP

#include "sneka/levelobject.hpp"

#include <map>

#include <glm/vec2.hpp>



namespace sneka {

	class Chunk;
	class FileChunkLoader;
	class FolderChunkLoader;

	class Chunk : public Asset {
		friend FileChunkLoader;
	public:
		/* Not actually a hash, but a long integer containing
		 * both coordinates - it does behave like a hash,
		 * but for low relative coordinates they can be
		 * assumed to be unique identifiers (which is always
		 * the case for any respectable chunk).
		 * This is important because:
		 * hash(x,y) ->  vec(x,y)
		 * vec(x,y)  ->  hash(x,y) */
		using hash_t = unsigned long long int;

		using coord_t = signed short;

	private:
		using ivec2 = glm::ivec2;
		using map_t = std::map<hash_t, LevelObject*>;

		map_t map;
		ivec2 offset; // Expressed in chunks of the same size
		hash_t size;

		bool changed;

	protected:
		Chunk(std::string asset_name, ivec2 chunk_offset, hash_t size);
		Chunk(ivec2 chunk_offset, const Chunk & cpy);

		/* These functions are only used internally,
		 * and if they could be used externally there would be mismatches
		 * between the positions of objects and the positions assigned
		 * to them in the map.
		 * Also, nothing is created or deleted - only assigned/removed
		 * to/from the map. */
		void set(ivec2, LevelObject*);
		LevelObject* at_nconst(ivec2) const;


	public:
		Chunk(const Chunk &);
		Chunk(Chunk&&) = default;
		~Chunk();

		void add(ivec2, LevelObjectTemplate&);
		void add(coord_t, coord_t, LevelObjectTemplate&);

		void remove(coord_t, coord_t);
		void remove(ivec2);

		void swap(ivec2, ivec2);

		/* IMPORTANT:
		 * the first position MUST be in (*this), and
		 * the second position MUST be in dest_chunk.
		 * If (and only if) any of those arguments is invalid,
		 * the function returns 'false'. */
		bool swap(ivec2 abs_src_pos, ivec2 abs_dest_pos, Chunk& dest_chunk);

		void setOffset(ivec2);
		void setOffset(coord_t, coord_t);

		const LevelObject * operator [] (ivec2) const;
		const LevelObject * at(coord_t, coord_t) const;

		/* returns 'true' if and only if the chunk has been modified
		 * after being loaded or saved (i.e. if the ChunkLoader
		 * can create a new Chunk identical to this one given the
		 * same position). */
		bool stateChanged() const;

		inline hash_t getSize() const { return size; }

		inline const map_t & getObjectMap() const { return map; }

		inline ivec2 absPosition(ivec2 relative_pos) const {
				return relative_pos + (ivec2(size) * offset); }
		inline ivec2 relPosition(ivec2 absolute_pos) const {
				return absolute_pos - (ivec2(size) * offset); }

		bool isInside(ivec2 absolute_position) const;

		static hash_t vecToHash(ivec2);
		static ivec2 hashToVec(hash_t);
	};


	class FileChunkLoader : public AssetLoader<Chunk> {
		friend Chunk;
		friend FolderChunkLoader;

	protected:
		LevelObjectLoader& obj_loader;
		std::map<char, std::string> id_to_obj;

		virtual Chunk* load(std::string file_name);

	public:
		FileChunkLoader(
				LevelObjectLoader& object_loader,
				const std::map<char, std::string> object_id_map
		);
	};

	class FolderChunkLoader : public FileChunkLoader {
	protected:
		std::string prefix, suffix;
	public:
		FolderChunkLoader(
				LevelObjectLoader& object_loader,
				std::map<char, std::string> object_id_map,
				std::string folder,
				std::string prefix = "", std::string suffix = ".chunk" );

		Chunk& get(Chunk::coord_t x, Chunk::coord_t z);
	};

}

#endif
