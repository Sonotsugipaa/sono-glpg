#ifndef SNEKA_ASSET_MESH_HPP
#define SNEKA_ASSET_MESH_HPP

#include "sneka/asset/asset.hpp"

#include "sneka/util/vertex.tpp"



namespace sneka {

	class Mesh: public Asset {
	protected:
		Vertex* _data;
		size_t _size;

	public:
		Mesh(Ari id, size_t number_of_vertices);
		inline Mesh(): Mesh (Ari(), 0) { }

		Mesh(const Mesh&);
		Mesh(Mesh&&);

		~Mesh();

		Mesh& operator = (const Mesh&);
		Mesh& operator = (Mesh&&);

		// Returns the number of vertices, NOT the number of values!
		inline size_t size() const { return _size; }

		inline const Vertex* begin() const { return _data; }
		inline       Vertex* begin()       { return _data; }
		inline const Vertex* end() const { return _data + _size; }
		inline       Vertex* end()       { return _data + _size; }

		inline const Vertex& operator [] (size_t i) const {
			return _data[i]; }
		inline       Vertex& operator [] (size_t i) {
			return _data[i]; }
	};

}

#endif
