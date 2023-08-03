#include<stdio.h>
#include<stdbool.h>

#include"SDL2/SDL.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

//
//A big chunk of this code is heavily based on sdl_tone_generator by grieverheart
//https://github.com/Grieverheart/sdl_tone_generator/tree/main
//

double sine_wave(float frequency, double time){
  return sin(frequency * 2.0 * M_PI * time);
}

double square_wave(float frequency, double time){
  double x = sine_wave(frequency, time);
  x = (x < 0.0) ? sin(-1.0) : sin(1.0);
  
  return x;
}

double sawtooth_wave(float frequency, double time){
  //TODO:: The whole function...
}

void audio_callback(void* userdata, uint8_t* stream, int len){

    uint64_t* samples_played = (uint64_t*)userdata;
    float* fstream = (float*)(stream);
    static const float volume = 0.2;
    static const float frequency = 200.0;

    for(int sid = 0; sid < (len / 8); ++sid)
    {
        double time = (*samples_played + sid) / 44100.0;

        //TODO:: square is louder than sine
        //that's probably because it's hardcoded to be 1.0 or -1.0
        //guess a way to fix that...
        
        
        //fstream[2 * sid + 0] = volume * square_wave(frequency, time);
        //fstream[2 * sid + 1] = volume * square_wave(frequency, time);

        fstream[2 * sid + 0] = volume * sine_wave(frequency, time);
        fstream[2 * sid + 1] = volume * sine_wave(frequency, time);

    }
    
    *samples_played += (len / 8);
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
  
  uint64_t samples_played = 0;
  SDL_AudioSpec audio_spec_want, audio_spec;
  SDL_memset(&audio_spec_want, 0, sizeof(audio_spec_want));

  audio_spec_want.freq     = 44100;
  audio_spec_want.format   = AUDIO_F32;
  audio_spec_want.channels = 2;
  audio_spec_want.samples  = 1024;
  audio_spec_want.callback = audio_callback;
  audio_spec_want.userdata = (void*)&samples_played;

  SDL_AudioDeviceID audio_device_id = SDL_OpenAudioDevice(
      NULL, 0,
      &audio_spec_want, &audio_spec,
      SDL_AUDIO_ALLOW_FORMAT_CHANGE);

  if(!audio_device_id){
    printf( "Error creating SDL audio device: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

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

  SDL_DestroyWindow(window);
  SDL_CloseAudioDevice(audio_device_id);
  SDL_Quit();
    
  return 0;
}

