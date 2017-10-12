#include "stdafx.h"
#include "CMediaObject.h"

CMediaObject::CMediaObject(std::string path, libvlc_instance_t *libVLCInstance) : mPath(path)
{
	mMedia = libvlc_media_new_path(libVLCInstance, path.c_str());
}

CMediaObject::~CMediaObject()
{

}