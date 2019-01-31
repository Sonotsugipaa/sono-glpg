#ifndef SONOGLPG_SHAPES3D_HPP
#define SONOGLPG_SHAPES3D_HPP

#include <cstdint>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "globject.hpp"

#ifndef SONOGLPG_SHAPE_HPP_NOWARN
	#warning "shape3d.hpp assumes the vertex shader has the input scheme of the 'gltest' shaders."
#endif



namespace gla {

	class Shape {
	protected:
		VertexArray va;
		GLenum draw_mode;
		GLuint vectors_n;
	public:
		Shape(GLenum draw_mode, VertexBuffer& vertex_vb, VertexBuffer& color_vb, std::size_t vectors_n);
		void draw();
	};

}

#endif
