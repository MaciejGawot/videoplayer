#include "stdafx.h"
#include "CMediaObject.h"

CMediaObject::CMediaObject(std::string path, libvlc_instance_t *libVLCInstance) : mPath(path)
{
	mMedia = libvlc_media_new_path(libVLCInstance, path.c_str());

	if (NULL == mMedia)
	{
		std::cout << "LibVLC media creation failure." << std::endl;
		throw std::exception("Error on libvlc_media_new_path");
	}
}

CMediaObject::~CMediaObject()
{

}