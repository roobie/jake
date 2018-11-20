#include <stdio.h>
#include <SDL.h>
#include "lib.h"

int
main(int argc, char* argv[])
{
	printf("result! %d\n", add(1, 2));

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Quit();
	return 0;
}
