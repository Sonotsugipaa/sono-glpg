#include "sneka/pool.hpp"

//#include <iostream> // Debug only



namespace {

	using namespace sneka::pool;

	void default_key_callback(unsigned int, unsigned int, bool) { }
	void default_resize_callback(unsigned int, unsigned int) { }

	const unsigned char * keyboard = nullptr;
	key_callback_t key_callback = default_key_callback;
	resize_callback_t resize_callback = default_resize_callback;

	void update_keyboard() {
		SDL_PumpEvents();
	}

}


namespace sneka::pool {

	bool poll_events() {
		SDL_Event event;

		keyboard = SDL_GetKeyboardState(nullptr);
		update_keyboard();

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					return false;  break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					update_keyboard();
					if(! event.key.repeat) {
						key_callback(
								event.key.keysym.sym,
								event.key.keysym.mod,
								event.key.state == SDL_RELEASED );
					}
					break;
				case SDL_WINDOWEVENT:
					switch(event.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						resize_callback(event.window.data1, event.window.data2);
						break;
					/*default:
						std::cout << "window event " << (int) event.window.event << std::endl;*/
					}
					break;
			}
		}

		return true;
	}

	key_callback_t set_key_callback(key_callback_t new_cb) {
		key_callback_t old = key_callback;
		key_callback = new_cb;
		return old;
	}

	resize_callback_t set_resize_callback(resize_callback_t new_cb) {
		resize_callback_t old = resize_callback;
		resize_callback = new_cb;
		return old;
	}

}
