#include "stdafx.h"

#include "src/CEngine.h"
#include "src/CVideoPlayer.h"

#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) 
{
	SDL_Event event;
	int done = 0, action = 0, pause = 0, n = 0;

	std::shared_ptr<CEngine> engine(new CEngine);
	std::shared_ptr<CVideoPlayer> player(new CVideoPlayer(*engine.get()));
	
	player->loadMedia(player->createMediaObject("Wildlife.wmv"));

	player->play();

	// Main loop.
	while (!done) {

		action = 0;

		// Keys: enter (fullscreen), space (pause), escape (quit).
		while (SDL_PollEvent(&event)) {

			switch (event.type) {
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				action = event.key.keysym.sym;
				break;
			}
		}

		switch (action) {
		case SDLK_ESCAPE:
		case SDLK_q:
			done = 1;
			break;
		case SDLK_p:
			printf("Pause toggle.\n");
			if (player->isPlaying())
			{ 
				player->pause();
			}
			else
			{
				player->play();
			}
			
			break;
		}

		SDL_Delay(1000 / 10);
	}
	

	return 0;
}