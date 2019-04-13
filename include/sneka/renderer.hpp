#ifndef SNEKA_RENDERER_HPP
#define SNEKA_RENDERER_HPP

#include "sneka/snekaruntime.hpp"



namespace sneka {

	class Renderer {
	protected:
		const SnekaRuntime & runtime;

	public:
		Renderer(const SnekaRuntime &);

		virtual void renderFrame() = 0;
	};

}

#endif
