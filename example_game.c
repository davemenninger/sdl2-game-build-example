#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

/* Screen dimension constants */
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/* a color */
typedef struct rgb {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} rgb_t;

SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
Uint32 render_flags =
    0; // SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
SDL_Renderer *rend;
SDL_bool quit = SDL_FALSE;
SDL_Event event;
SDL_Rect rect;
rgb_t rect_color;
int right = 0;
int left = 0;
int up = 0;
int down = 0;
int velocity = 2;

int setup();
void update_rect();
void draw_game();
int screenshot();

int error(char *msg, const char *err) {
  printf("Error %s: %s\n", msg, err);
  return 1;
}

int main(int argc, char *args[]) {
  (void)argc;
  (void)args;

  setup();

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = SDL_TRUE;
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          right = 0;
          break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          left = 0;
          break;
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          up = 0;
          break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
          down = 0;
          break;
        default:
          (void)0;
        }
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_D:
        case SDL_SCANCODE_RIGHT:
          right = 1;
          break;
        case SDL_SCANCODE_A:
        case SDL_SCANCODE_LEFT:
          left = 1;
          break;
        case SDL_SCANCODE_W:
        case SDL_SCANCODE_UP:
          up = 1;
          break;
        case SDL_SCANCODE_S:
        case SDL_SCANCODE_DOWN:
          down = 1;
          break;
        default:
          (void)0;
        }
        break;
      default:
        (void)0;
      }
    }

    update_rect();
    draw_game();

    SDL_Delay(1000 / 60);
  }

  screenshot();

  /* close */
  SDL_FreeSurface(surface);
  surface = NULL;
  SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();

  return 0;
}

int setup() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    return error("init", SDL_GetError());

  window = SDL_CreateWindow("A Really Cool Game", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == NULL)
    return error("window", SDL_GetError());

  rend = SDL_CreateRenderer(window, -1, render_flags);
  SDL_RenderClear(rend);

  rect.x = 250;
  rect.y = 150;
  rect.w = 100;
  rect.h = 100;

  rect_color.red = 255;
  rect_color.green = 255;
  rect_color.blue = 127;
}

void update_rect() {
  if (right == 1) {
    rect.x += velocity;
  }

  if (left == 1) {
    rect.x -= velocity;
  }

  rect.x = rect.x % (SCREEN_WIDTH - rect.w);
  if (rect.x < 0)
    rect.x = SCREEN_WIDTH - rect.w;

  if (up == 1) {
    rect.y -= velocity;
  }

  if (down == 1) {
    rect.y += velocity;
  }

  rect.y = rect.y % (SCREEN_HEIGHT - rect.h);
  if (rect.y < 0)
    rect.y = SCREEN_HEIGHT - rect.h;

  rect_color.red += rand() % 6 - 3;
  rect_color.green += rand() % 10 - 5;
  rect_color.blue += rand() % 14 - 7;

  velocity = (rect_color.blue + rect_color.red + rect_color.green) / 120;
}

void draw_game() {
  SDL_SetRenderDrawColor(rend, rect_color.red, rect_color.green,
                         rect_color.blue, 255);
  SDL_RenderFillRect(rend, &rect);
  SDL_RenderDrawRect(rend, &rect);
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

  SDL_RenderPresent(rend);
}

int screenshot() {
  surface =
      SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);

  if (SDL_RenderReadPixels(rend, NULL, surface->format->format, surface->pixels,
                           surface->pitch) != 0) {
    return error("SDL_RenderReadPixels failed: ", SDL_GetError());
  }

  if (SDL_SaveBMP(surface, "screenshot.bmp") != 0) {
    return error("SDL_SaveBMP failed:", SDL_GetError());
  }
}
