#ifndef SNEKA_UTIL_VERTEX_HPP
#define SNEKA_UTIL_VERTEX_HPP

#include <array>



namespace sneka {

	/* (position)  X  Y  Z
	 * (normals)   X  Y  Z
	 * (color)     R  G  B  A
	*/

	namespace vtx {

		namespace position {
			constexpr size_t
				size = 3,
				offset = 0;
		}

		namespace normal {
			constexpr size_t
				size = position::size,
				offset = position::size;
		}

		namespace color {
			constexpr size_t
				size = 4,
				offset = normal::offset + normal::size;
		}

		constexpr size_t size = position::size + normal::size + color::size;

	}

	using Vertex = std::array<float, vtx::size>;

}

#endif
