#include "sneka/asset/mesh.hpp"



namespace sneka {

	Mesh::Mesh(Ari ari, size_t size):
			Asset (std::move(ari)),
			_data (new Vertex[size * vtx::size]),
			_size(size)
	{ }

	Mesh::Mesh(const Mesh& cpy):
			Asset (cpy.getAri()),
			_data (new Vertex[cpy._size * vtx::size]),
			_size(cpy._size)
	{
		for(size_t i=0; i < _size; ++i) {
			_data[i] = cpy[i];
		}
	}

	Mesh::Mesh(Mesh&& mov):
			Asset (mov.getAri()),
			_data (std::move(mov._data)),
			_size (std::move(mov._size))
	{
		mov._data = nullptr;
	}

	Mesh::~Mesh() {
		if(_data != nullptr) {
			delete[] _data;
			_data = nullptr;
		}
	}

	Mesh& Mesh::operator = (const Mesh& cpy) {
		this->Mesh::~Mesh();
		new (this) Mesh(cpy);
		return *this;
	}

	Mesh& Mesh::operator = (Mesh&& mov) {
		this->Mesh::~Mesh();
		new (this) Mesh(std::move(mov));
		return *this;
	}

}
