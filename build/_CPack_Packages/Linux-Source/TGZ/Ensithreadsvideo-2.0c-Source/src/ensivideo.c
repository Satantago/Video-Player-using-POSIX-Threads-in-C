#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#include "oggstream.h"
#include "stream_common.h"

#include "synchro.h"

#include <pthread.h>


pthread_mutex_t mutexHash = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexFenetre = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condTailleFenetre = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexFenetreEtTexture = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condFenetreEtTexture = PTHREAD_COND_INITIALIZER;
bool fenetre_texture_prete = false;
bool fenetre_taille = false;

pthread_t theora2sdlthread;

pthread_mutex_t mutexTexture = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condTextureRead = PTHREAD_COND_INITIALIZER;
pthread_cond_t condTextureWrite = PTHREAD_COND_INITIALIZER;
int nb_texture_to_display = 0;

int main(int argc, char *argv[]) {
  
  int res;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s FILE", argv[0]);
    exit(EXIT_FAILURE);
  }
  assert(argc == 2);

  // Initialisation de la SDL
  res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
  atexit(SDL_Quit);
  assert(res == 0);

  // Your code HERE
  // start the two stream readers (theoraStreamReader and vorbisStreamReader)
  // each in a thread*
  pthread_t theoraThread, vorbisThread;
  pthread_create(&theoraThread, NULL, theoraStreamReader, (void*) argv[1]);
  pthread_create(&vorbisThread, NULL, vorbisStreamReader, (void*) argv[1]);
  // wait for vorbis thread
  pthread_join(vorbisThread, NULL);
  // 1 seconde of sound in advance, thus wait 1 seconde
  // before leaving
  sleep(1);

  // Wait for theora and theora2sdl threads
  pthread_cancel(theora2sdlthread);
  pthread_cancel(theoraThread);

  // TODO
  /* liberer des choses ? */
  pthread_join(theoraThread, NULL);
  pthread_join(theora2sdlthread, NULL);
  
  exit(EXIT_SUCCESS);
}
