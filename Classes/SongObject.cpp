#include "SongObject.h"

SongObject* SongObject::create(int id)
{
	auto pRet = new SongObject();

	if (pRet && pRet->init(id))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool SongObject::init(int id)
{
	_songID = id;

	return true;
}
