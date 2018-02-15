#include "stdafx.h"
#include "CVideoPlayer.h"


CVideoPlayer::CVideoPlayer(CVideoEngine& engine)
{
	mContext.videoWidth = engine.getScreenWidth();
	mContext.videoHeight = engine.getScreenHeight();

	mContext.renderer = engine.getRenderer();

	mContext.texture = SDL_CreateTexture(
		mContext.renderer,
		SDL_PIXELFORMAT_BGR565, SDL_TEXTUREACCESS_STREAMING,
		mContext.videoWidth, mContext.videoHeight);

	if (NULL == mContext.texture) 
	{
		std::cout << "Texture could not be creeated: " << SDL_GetError() << std::endl;
		throw std::exception("Error on SDL_CreateTexture.");
	}

	mContext.mutex = SDL_CreateMutex();

	char const *vlc_argv[] = {
		//"--no-audio", // Don't play audio.
		"--no-xlib", // Don't use Xlib.
		"--video-filter", "sepia",
		//"--sepia-intensity=200"
	};

	//TODO: Set VLC_PLUGIN_PATH in fine way
	//printf("VLC_PLUGIN_PATH=%s\n", getenv("VLC_PLUGIN_PATH"));

	// Initialise libVLC.
	int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
	mLibVLCInstance = libvlc_new(vlc_argc, vlc_argv);

	if (NULL == mLibVLCInstance)
	{
		std::cout << "LibVLC initialization failure." << std::endl;
		throw std::exception("Error on libvlc_new.");
	}

	mMediaPlayer = libvlc_media_player_new(mLibVLCInstance);

	if (NULL == mMediaPlayer)
	{
		std::cout << "Media player could not be created" << std::endl;
		throw std::exception("Error on libvlc_media_player_new.");
	}
	libvlc_video_set_callbacks(mMediaPlayer, &CVideoPlayer::lock, &CVideoPlayer::unlock, &CVideoPlayer::display, &mContext);
	libvlc_video_set_format(mMediaPlayer, "RV16", mContext.videoWidth, mContext.videoHeight, mContext.videoWidth * 2);

	mEventManager = libvlc_media_player_event_manager(mMediaPlayer);
	registerEvents();
}


CVideoPlayer::~CVideoPlayer()
{
	// Clean up libVLC.
	libvlc_media_player_release(mMediaPlayer);
	libvlc_release(mLibVLCInstance);

	// Close window and clean up libSDL.
	SDL_DestroyMutex(mContext.mutex);
	SDL_DestroyRenderer(mContext.renderer);
}

void CVideoPlayer::loadMedia(CMediaObject& media)
{
	libvlc_media_player_set_media(mMediaPlayer, media());
	libvlc_media_release(media());
}

bool CVideoPlayer::isPlaying()
{
	return libvlc_media_player_is_playing(mMediaPlayer);
}

void CVideoPlayer::play()
{
	if (0 != libvlc_media_player_play(mMediaPlayer))
	{
		throw std::exception("Error on libvlc_media_player_play.");
	}
}

void CVideoPlayer::stop()
{
	libvlc_media_player_stop(mMediaPlayer);
}

void CVideoPlayer::pause()
{
	libvlc_media_player_pause(mMediaPlayer);
}

CMediaObject CVideoPlayer::createMediaObject(std::string path)
{
	return CMediaObject(path, mLibVLCInstance);
}


// VLC prepares to render a video frame.
void* CVideoPlayer::lock(void *data, void **p_pixels) {

	struct context *c = (context *)data;

	int pitch;
	SDL_LockMutex(c->mutex);
	SDL_LockTexture(c->texture, NULL, p_pixels, &pitch);

	return NULL; // Picture identifier, not needed here.
}

// VLC just rendered a video frame.
void CVideoPlayer::unlock(void *data, void *id, void *const *p_pixels)
{
	struct context *c = (context *)data;

	uint16_t *pixels = (uint16_t *)*p_pixels;

	SDL_UnlockTexture(c->texture);
	SDL_UnlockMutex(c->mutex);
}

// VLC wants to display a video frame.
void CVideoPlayer::display(void *data, void *id) 
{
	struct context *c = (context *)data;

	SDL_Rect rect;
	rect.w = c->videoWidth;
	rect.h = c->videoHeight;
	rect.x = 0;
	rect.y = 0;

	SDL_SetRenderDrawColor(c->renderer, 0, 80, 0, 255);
	SDL_RenderClear(c->renderer);
	SDL_RenderCopy(c->renderer, c->texture, NULL, &rect);
	SDL_RenderPresent(c->renderer);
}

void CVideoPlayer::registerEvents()
{
	if (0 != libvlc_event_attach(mEventManager, libvlc_MediaPlayerEndReached, callbacks, nullptr))
	{
		throw std::exception("Error on libvlc_event_attach.");
	}
}

void CVideoPlayer::callbacks(const libvlc_event_t* event, void* data)
{
	switch (event->type)
	{
	case libvlc_MediaPlayerEndReached:
		printf("Media ended");
		break;

		//Just in case ;)
		//libvlc_MediaPlayerMediaChanged
		//libvlc_MediaPlayerNothingSpecial
		//libvlc_MediaPlayerOpening
		//libvlc_MediaPlayerBuffering
		//libvlc_MediaPlayerPlaying
		//libvlc_MediaPlayerPaused
		//libvlc_MediaPlayerStopped
		//libvlc_MediaPlayerForward
		//libvlc_MediaPlayerBackward
	}
}