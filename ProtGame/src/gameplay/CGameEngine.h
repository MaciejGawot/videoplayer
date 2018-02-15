#pragma once
#include "../CVideoEngine.h"
#include "../CVideoPlayer.h"

class CGameEngine
{
public:
	CGameEngine();
	~CGameEngine();

	void mainLoop();

protected:
	std::shared_ptr<CVideoEngine> mVideoEngine;
	std::shared_ptr<CVideoPlayer> mVideoPlayer;

};

