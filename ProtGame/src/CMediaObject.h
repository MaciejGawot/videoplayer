#pragma once
#include "../stdafx.h"

class CMediaObject
{
public:
	CMediaObject(std::string, libvlc_instance_t*);
	~CMediaObject();

	libvlc_media_t* operator()() { return mMedia; }


private:
	std::string mPath;

	//libvlc dependencies
	libvlc_media_t *mMedia;

};

