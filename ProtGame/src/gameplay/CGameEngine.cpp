#include "stdafx.h"
#include "CGameEngine.h"


CGameEngine::CGameEngine() : mVideoEngine(new CVideoEngine), mVideoPlayer(new CVideoPlayer(mVideoEngine))
{
	mVideoPlayer->loadMedia(mVideoPlayer->createMediaObject("Wildlife.wmv"));
	mVideoPlayer->play();
}


CGameEngine::~CGameEngine()
{

}

void CGameEngine::mainLoop()
{
	SDL_Event event;

	int done = 0, action = 0, pause = 0, n = 0;

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
			if (mVideoPlayer->isPlaying())
			{
				mVideoPlayer->pause();
			}
			else
			{
				mVideoPlayer->play();
			}

			break;
		}

		SDL_Delay(1000 / 10);
	}
}