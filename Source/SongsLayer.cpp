#include "SongsLayer.h"
#include "SongObject.h"
#include "SongCell.h"

SongsLayer* SongsLayer::create()
{
	auto pRet = new SongsLayer();

	if (pRet && pRet->init(pRet->_listView, "Soundtracks"))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void SongsLayer::customSetup()
{
	_listView->setAnchorPoint({ 0.5, 0.5 });
	_listView->setBounceEnabled(true);
	_listView->setScrollBarEnabled(false);

	for (size_t i = 0; i < 21; ++i)
	{
		auto cell = SongCell::create(SongObject::create(i));
		cell->updateBGColor(i);
		_listView->pushBackCustomItem(cell);
	}

	auto cell = SongCell::create(SongObject::create(-1));
	cell->updateBGColor(-1);
	_listView->pushBackCustomItem(cell);
}
