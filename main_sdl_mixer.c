#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char* argv[]){
  // Initialize SDL.
  if (SDL_Init(SDL_INIT_AUDIO) != 0){
    printf("FAILED INITIALIZING SDL");
    printf("%s\n", SDL_GetError() );
    return 1;
  }

  // Initialize SDL_Mixer
  if (!Mix_Init(MIX_INIT_OGG)){
    printf("FAILED INITIALIZING SDL_mixer");
    printf("%s\n", SDL_GetError() );
    return 1;
  }

  //open sound device
  if(Mix_OpenAudio(41000, AUDIO_F32SYS , 2, 2048) < 0){
    printf("FAILED OPENING AUDIO DEVICE");
    printf("%s\n", SDL_GetError() );
    return -1;
  }
  
  //load audio file
  Mix_Chunk* wav_file = NULL;
  wav_file = Mix_LoadWAV("audio.wav");
  if(wav_file == NULL){
     printf("FAILED OPENING WAV FILE\n");
     printf("%s\n", SDL_GetError() );
     return 1;
  }


  //Play Audio File
  Mix_Volume(1, MIX_MAX_VOLUME / 3);
  if(Mix_PlayChannel(1, wav_file, 0) < 0){
    printf("FAILED PLAYING CHUNK");
    printf("%s\n", SDL_GetError() );
    return -1;
  }

  SDL_Delay(1000);
  
  //free wav file
  Mix_FreeChunk(wav_file);

  //close audio device
  Mix_CloseAudio();

  Mix_Quit();
  SDL_Quit();
}
