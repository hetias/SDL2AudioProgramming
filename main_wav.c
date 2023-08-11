#include<stdio.h>
#include<stdbool.h>

#include"SDL2/SDL.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define AUDIO_FILE "audio.wav"

typedef struct{
  Uint8* data;
  Uint8* pos;
  Uint32 lenght;
  SDL_AudioSpec spec;
}audioInfo;

SDL_Window* window = NULL; 
SDL_Renderer* renderer = NULL;
SDL_AudioDeviceID audio_device_id = -1;

audioInfo file = {0};

/* audio data here is interpreted as 16 bits signed integers.
   This is because that what my hardware * surprisingly * manages.
   You may have to change this to work on yours to any of the supported
   audio format by sdl
   https://wiki.libsdl.org/SDL2/SDL_AudioFormat
   and more than probably, the audio that you use may be another factor
   to consider.

   TODO:: MAKE A DYNAMIC KIND OF CAST DEPENDING ON FILE.SPEC.FORMAT
*/
void audio_callback(void* userdata, uint8_t* stream, int buff_len){

  Sint16* istream = (Sint16*)stream;

  SDL_memset(stream, 0, buff_len);
  SDL_memcpy(stream, file.pos, buff_len);

  for(int i = 0; i < (buff_len / 4); ++i)
  {
    /* istream[2 * i + 0] *= 0.1; //channel 1 */
    /* istream[2 * i + 1] *= 0.1; //channel 2 */
  }
  
  file.pos += buff_len;
  file.lenght -= buff_len;
}

int initialize_sdl(){
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
    printf("Error initializing SDL: %s\n", SDL_GetError());
    return -1;
  }

  window = SDL_CreateWindow(
      "SDL Audio",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH, WINDOW_HEIGHT,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  
  if(window == NULL){
    printf("Error creating SDL window: %s\n",SDL_GetError());
    SDL_Quit();
    return -1;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(renderer == NULL){
    printf("Error creating SDL window: %s\n",SDL_GetError());
    SDL_Quit();
    return -1;
  }

  return 0;
}

void close_sdl(){
  SDL_DestroyWindow(window);
  SDL_CloseAudioDevice(audio_device_id);
  SDL_Quit();
}

int open_audio_file(){
  SDL_LoadWAV( AUDIO_FILE, &file.spec, &file.data, &file.lenght);

  if(!file.data){
    printf("Error when loading audio}\n");
    return -1;
  }

  file.pos = file.data;
  file.spec.callback = audio_callback;
  return 0;
}

void write_spec(){
  printf("frequency: %d\n", file.spec.freq);
  printf("format: %d\n", file.spec.format);
  printf("channels: %d\n", file.spec.channels);
  printf("samples: %d\n", file.spec.samples);

  printf("Audio bit size: %d\n", SDL_AUDIO_BITSIZE(file.spec.format));
  
  (SDL_AUDIO_ISFLOAT(file.spec.format)) ? printf("Audio format is float\n") : printf("Audio format is int\n");

  (SDL_AUDIO_ISSIGNED(file.spec.format)) ? printf("Audio format is signed\n") : printf("Audio format is unsigned\n");

}

int open_audio(){
  audio_device_id = SDL_OpenAudioDevice(
      NULL,
      0,
      &file.spec,
      NULL,
      SDL_AUDIO_ALLOW_FORMAT_CHANGE);

  if(audio_device_id < 0){
    printf( "Error creating SDL audio device: %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}

int main(int argc, char* argv[]){
  if(initialize_sdl() < 0)
    return -1;

  if(open_audio_file() < 0)
    return -1;

  if(open_audio() < 0)
    return -1;
    
  write_spec();

  SDL_PauseAudioDevice(audio_device_id, 0);
  
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

  close_sdl();    
  return 0;
}

