#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *texture = NULL;

void *pixels;
uint8_t *base;
int pitch;

/*
the pixel dimentions that we are rendering to, it
will be stretched to fit the window pixel dimentions
*/
const unsigned int RENDER_WIDTH = 300;
const unsigned int RENDER_HEIGHT = 200;

// the window dimentions
const unsigned int WINDOW_WIDTH = 1080;
const unsigned int WINDOW_HEIGHT = 720;

// pixel array we will be rendering to
uint32_t pixel_data[RENDER_WIDTH * RENDER_HEIGHT];

// framerate variables
double oldTime = 0;
double myTime = 0;
double frameTime = 0;

int done = 0;


void init_sdl() {
	// set up sdl screen
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	window = SDL_CreateWindow("", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);

	// done to properly render alpha channel
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}

void render_frame() {
	// clear the renderer
	SDL_RenderClear(renderer);

	// used for optimization purposes
	SDL_LockTexture(texture, NULL, &pixels, &pitch);

	// copy the pixel_data array to the texture
	memcpy((Uint8 *)pixels, pixel_data, pitch * RENDER_HEIGHT);

	SDL_UnlockTexture(texture);
	// if texture isn't same dimentions as window, it stretches the texture to fit the screen
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void handle_sdl_events() {
	int event_occured = SDL_PollEvent(&event);
	const uint8_t *keyboard = SDL_GetKeyboardState(NULL);

	// handle SDL inputs
	if (keyboard[SDL_SCANCODE_A]) printf("%s\n", "left");
	if (keyboard[SDL_SCANCODE_D]) printf("%s\n", "right");

	if (keyboard[SDL_SCANCODE_W]) printf("%s\n", "up");
	if (keyboard[SDL_SCANCODE_S]) printf("%s\n", "down");

	// YOUR CUSTOME KEY INPUTS

	// handle SDL quit events
	if ((event_occured && event.type == SDL_QUIT) || keyboard[SDL_SCANCODE_ESCAPE]) {
		done = 1;
	}
}

void print_fps() {
	oldTime = myTime;
	myTime = SDL_GetTicks() / 1000.0;
	frameTime = myTime - oldTime;
	printf("%f\n", 1.0 / frameTime);
}

int main(void) {

	// initialize sdl window, renderer, and texture
	init_sdl();

	while (!done) {
		// change delay for different FPS
		SDL_Delay(10);

		// handle quit and input events
		handle_sdl_events();

		// draws the contents of pixel_data to the sdl window
		render_frame();

		print_fps();
	}
	
	// free memory associated with SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}