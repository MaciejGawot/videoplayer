#pragma once
#include "../stdafx.h"
#include "CEngine.h"
#include "CMediaObject.h"

class CVideoPlayer
{
public:
	CVideoPlayer(CEngine&);
	~CVideoPlayer();

	void loadMedia(CMediaObject&);
	bool isPlaying();
	void play();
	void stop();

	CMediaObject createMediaObject(std::string);

private:
	struct context {
		SDL_Renderer *renderer;
		SDL_Texture *texture;
		SDL_mutex *mutex;

		//media parameters
		int videoWidth;
		int videoHeight;
	} mContext;

	static void* lock(void *data, void **p_pixels);
	static void unlock(void *data, void *id, void *const *p_pixels);
	static void display(void *data, void *id);
	
	//libvlc dependencies 
	libvlc_instance_t *mLibVLCInstance;
	libvlc_media_player_t *mMediaPlayer;
};

