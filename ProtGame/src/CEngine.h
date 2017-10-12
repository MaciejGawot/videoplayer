#pragma once

#include "../stdafx.h"

class CEngine
{
public:
	CEngine();
	~CEngine();

protected:
	int mScreenWidth;
	int mScreenHeight;

	SDL_Window* mWindow = nullptr; //The window we'll be rendering to
	SDL_Renderer* mRenderer = nullptr; //The surface contained by the window

public:
	SDL_Window* getWindow() const { return mWindow; }
	SDL_Renderer* getRenderer() const { return mRenderer; }
	int getScreenWidth() const { return mScreenWidth; }
	int getScreenHeight() const { return mScreenHeight; }
};

