#include "stdafx.h"
#include "CVideoEngine.h"


CVideoEngine::CVideoEngine() : mScreenWidth(800), mScreenHeight(600)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;;
	}
	else
	{
		//Create window
		mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mScreenWidth, mScreenHeight, SDL_WINDOW_SHOWN);
		if (mWindow == NULL)
		{
			std::cout << "Window could not be creeated: " << SDL_GetError() << std::endl;;
		}

		//Create renderer
		mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
		if (mRenderer == NULL)
		{
			std::cout << "Renderer could not be creeated: " << SDL_GetError() << std::endl;;
		}
	}
}


CVideoEngine::~CVideoEngine()
{
	//Destroy window
	SDL_DestroyWindow(mWindow);

	//Quit SDL subsystems
	SDL_Quit();
}