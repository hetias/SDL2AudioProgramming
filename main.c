#include<stdio.h>
#include<stdbool.h>

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

int initialize_sdl();
void free_sdl();

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int main(int argc, char* argv[])
{

  //initialize the whole thing
  if( initialize_sdl() < 0) return -1;

  bool running = true;
  while(running)
  {

    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          running = false;
          break;
          
      }
    }//events end

    SDL_SetRenderDrawColor(gRenderer, 125, 125, 125, 125);
    SDL_RenderClear(gRenderer);

    SDL_RenderPresent(gRenderer);

  }
  

  //free the whole thing
  free_sdl();

  return 0;
}

int initialize_sdl()
{

  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    printf("Failed to initialize sdl!\n %s", SDL_GetError());
    return -1;
  }

  gWindow = SDL_CreateWindow("Audio Visualizer",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             800,
                             600,
                             SDL_WINDOW_SHOWN);
  if(!gWindow)
  {
    printf("Failed on Window Creation!\n %s", SDL_GetError());
    return -1;
  }

  gRenderer = SDL_CreateRenderer(gWindow,
                                 -1,
                                 SDL_RENDERER_ACCELERATED);
  if(!gRenderer)
  {
    printf("Failed on Renderer Creation!\n %s", SDL_GetError());
    return -1;
  }

  if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    printf("Failed to initialize SDL_image!");
    return -1;
  }

  if(TTF_Init() < 0)
  {
    printf("Failed to initialize SDL_ttf!");
    return -1;
  }

  return 0;
}

void free_sdl()
{
  
}





