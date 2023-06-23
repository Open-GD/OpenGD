/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

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
