#include "sneka/snekaruntime.hpp"



namespace {

	void update_keyboard_state() {
		SDL_PumpEvents();
	}

}


namespace sneka {

	bool SnekaRuntime::pollEvents() {
		SDL_Event event;

		keyboard_state = SDL_GetKeyboardState(nullptr);
		update_keyboard_state();

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					return false;  break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if(! event.key.repeat) {
						key_cb(
								*this,
								event.key.keysym.sym,
								event.key.keysym.mod,
								event.key.state == SDL_RELEASED );
					}
					break;
				case SDL_WINDOWEVENT:
					switch(event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						rsz_cb(*this, event.window.data1, event.window.data2);
						break;
					/*default:
						std::cout << "window event " << (int) event.window.event << std::endl;*/
					}
					break;
			}
		}

		return true;
	}

	unsigned char SnekaRuntime::getKeyboardState(unsigned int kc) const {

	}

	key_callback_t SnekaRuntime::setKeyCallback(key_callback_t cb) {
		key_callback_t cb_old = key_cb;
		key_cb = cb;
		return cb_old;
	}

	resize_callback_t SnekaRuntime::setResizeCallback(resize_callback_t cb) {
		resize_callback_t cb_old = rsz_cb;
		rsz_cb = cb;
		return cb_old;
	}

}
