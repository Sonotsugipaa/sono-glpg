#include <SDL2/SDL.h>

#include <cstdio>

#define FPS            (2.0)
#define FRAMES_SECOND  (1000.0/FPS)



void render(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture) {
	SDL_Rect dstrect = { x, y, 50, 50 }; // x, y, w, h
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, &dstrect); // RAM (CPU) -> RAM (GPU)
	SDL_RenderPresent(renderer); // Commit
}


int main(int argc, char** argv) {
	int x=100, y=100;
	bool quit = false;
	bool ignore_ev = false;
	SDL_Renderer* renderer;
	SDL_Event event;
	SDL_Surface* image;
	SDL_Texture* texture;

	printf("Arguments: ");
	for(int i=1; i < argc; i+=1)
		printf("[%s]\n", argv[i]);

	SDL_Init(SDL_INIT_EVERYTHING);

	// window creation
	SDL_Window * window = SDL_CreateWindow(
		"My SDL Empty Window",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480, 0 );

	// renderer setup
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// texture creation
	image = SDL_LoadBMP("assets/image.bmp");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image); // not needed anymore

	// wait and process events
	render(x, y, renderer, texture);

	while(! quit) {
		SDL_PollEvent(&event);

		switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_LEFT:  x--; break;
					case SDLK_RIGHT: x++; break;
					case SDLK_UP:    y--; break;
					case SDLK_DOWN:  y++; break;
				}
				break;
			default:
				ignore_ev = true;
		}

		if(! ignore_ev) {
			printf("%d\n", event.type);
			render(x, y, renderer, texture);
			SDL_Delay(FRAMES_SECOND);
		} else {
			ignore_ev = false;
		}
	}

	// dealloc
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
