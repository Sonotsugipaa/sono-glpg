#include "sneka/pool.hpp"



namespace {

	using namespace sneka::pool;

	const unsigned char * keyboard = nullptr;
	key_callback_t key_callback = nullptr;

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
			}
		}

		return true;
	}

	key_callback_t set_key_callback(key_callback_t new_cb) {
		key_callback_t old = key_callback;
		key_callback = new_cb;
		return old;
	}

}
