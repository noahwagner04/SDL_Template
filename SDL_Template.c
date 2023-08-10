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

void handle_mouse_events() {
	int mouse_x = 0;
	int mouse_y = 0;
	uint32_t mouse = SDL_GetMouseState(&mouse_x, &mouse_y);

	// check if the left mouse button is currently being held down
	if(mouse & SDL_BUTTON(1)) printf("mouse left\n");

	printf("mouse x: %d, mouse y: %d\n", mouse_x, mouse_y);
}

// assumes SDL_PumpEvents has been called (which is automatically done by calling SDL_WaitEvent or SDL_PollEvent)
void handle_keyboard_events() {
	// get a snapshot of the keyboard
	const uint8_t *keyboard = SDL_GetKeyboardState(NULL);

	// check if WASD keys are currently being held down
	if (keyboard[SDL_SCANCODE_A]) printf("%s\n", "left");
	if (keyboard[SDL_SCANCODE_D]) printf("%s\n", "right");

	if (keyboard[SDL_SCANCODE_W]) printf("%s\n", "up");
	if (keyboard[SDL_SCANCODE_S]) printf("%s\n", "down");
}

void handle_events() {

	// poll until all events are handled
	while(SDL_PollEvent(&event)) {
		// handle quit event
		if(event.type == SDL_QUIT) done = 1;

		// custome event handling goes here
	}
	/*
	NOTE: the functions below only check if mouse and keyboard buttons are held down. To check if a key or 
	mouse button was just pressed or released, you could either 
	1. compare the keyboard / mouse states to the previous frames keyboard / mouse states 
		(would require an additional state for the previous frame). If a key or button state is different 
		from the previous frames state, then the key has been pressed or released
	2. execute callback functions to SDL_MOUSE_BOTTONDOWN, SDL_KEYDOWN, SDL_MOUSE_BOTTONUP, and SDL_KEYUP 
		directly inside the above event handler loop, and pass the event as an argument to the functions
	*/
	handle_mouse_events();
	handle_keyboard_events();
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
		handle_events();

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