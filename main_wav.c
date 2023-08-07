#include<stdio.h>
#include<stdbool.h>

#include"SDL2/SDL.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

Uint8* wav_buff = NULL;
Uint8* wav_pos = NULL;
Uint32 wav_len = 0;

void audio_callback(void* userdata, uint8_t* stream, int len){
  /* audio data here is interpreted as 16 bits signed integers.
     This is because that what my hardware * surprisingly * manages.
     You may have to change this to work on yours to any of the supported
     audio format by sdl
     https://wiki.libsdl.org/SDL2/SDL_AudioFormat
   */
  
  Sint16* istream = (Sint16*)stream;

  SDL_memset(stream, 0, len);
  SDL_memcpy(stream, wav_buff, len);

  for(int i = 0; i < (len / 4); ++i)
  {
    istream[2 * i + 0] *= 0.1;
    istream[2 * i + 1] *= 0.1;
  }
  
  wav_pos += len / 4;
  wav_len -= len;
}

int main(int argc, char* argv[]){
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Window* window = NULL;
  window = SDL_CreateWindow(
      "SDL Tone Generator",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  
  if(window == NULL){
    printf("Error creating SDL window: %s\n",SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_Renderer* renderer = NULL;
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL){
    printf("Error creating SDL window: %s\n",SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_AudioSpec audio_spec_want;
  SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

  audio_spec_want.freq     = 44100;
  audio_spec_want.format   = AUDIO_F32SYS;
  audio_spec_want.channels = 2;
  audio_spec_want.samples  = 1024;
  audio_spec_want.callback = audio_callback;
  audio_spec_want.userdata = NULL;

  SDL_LoadWAV("audio.wav", &audio_spec_want, &wav_buff, &wav_len);

  if(!wav_buff){
    printf("Error when loading audio}\n");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  SDL_AudioDeviceID audio_device_id = SDL_OpenAudioDevice(
      NULL, 0,
      &audio_spec_want, NULL,
      SDL_AUDIO_ALLOW_FORMAT_CHANGE);

  if(audio_spec_want.format == AUDIO_S32MSB){
    printf("Audio format audio\n");
  }
  
  if(audio_device_id < 0){
    printf( "Error creating SDL audio device: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_PauseAudioDevice(audio_device_id, 0);

  printf("Audio bit size: %d\n", SDL_AUDIO_BITSIZE(audio_spec_want.format));
  
  (SDL_AUDIO_ISFLOAT(audio_spec_want.format)) ? printf("Audio format is float\n") : printf("Audio format is int\n");

  (SDL_AUDIO_ISSIGNED(audio_spec_want.format)) ? printf("Audio format is signed\n") : printf("Audio format is unsigned\n");

  bool running = true;
  while(running){

    SDL_Event event;
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT){
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_CloseAudioDevice(audio_device_id);
  SDL_Quit();
    
  return 0;
}

