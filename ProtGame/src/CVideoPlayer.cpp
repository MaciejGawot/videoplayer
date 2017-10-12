#include "stdafx.h"
#include "CVideoPlayer.h"


CVideoPlayer::CVideoPlayer(CEngine& engine)
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
		std::cout << "Texture could not be creeated: " << SDL_GetError() << std::endl;;
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
	}
}


CVideoPlayer::~CVideoPlayer()
{
	// Stop stream and clean up libVLC.
	libvlc_media_player_stop(mMediaPlayer);
	libvlc_media_player_release(mMediaPlayer);
	libvlc_release(mLibVLCInstance);

	// Close window and clean up libSDL.
	SDL_DestroyMutex(mContext.mutex);
	SDL_DestroyRenderer(mContext.renderer);
}

void CVideoPlayer::loadMedia(CMediaObject& media)
{
	mMediaPlayer = libvlc_media_player_new_from_media(media());
	libvlc_media_release(media());

	libvlc_video_set_callbacks(mMediaPlayer, &CVideoPlayer::lock, &CVideoPlayer::unlock,  &CVideoPlayer::display, &mContext);
	libvlc_video_set_format(mMediaPlayer, "RV16", mContext.videoWidth, mContext.videoHeight, mContext.videoWidth*2);

	//TODO move to dedicated method
	libvlc_media_player_play(mMediaPlayer);
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