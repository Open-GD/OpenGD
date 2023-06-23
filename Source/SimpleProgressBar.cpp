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

#include "SimpleProgressBar.h"

#include "GameToolbox/getTextureString.h"

bool SimpleProgressBar::init() 
{
	_pGroove = ax::Sprite::create(GameToolbox::getTextureString("slidergroove_02.png"));
	_pBar = ax::Sprite::create(GameToolbox::getTextureString("sliderBar.png"));

	// config 
	_pGroove->setStretchEnabled(false);
	_pBarWidth = _pGroove->getContentSize().width - 4;
	_pBar->setStretchEnabled(false);
	_pBar->getTexture()->setTexParameters({ ax::backend::SamplerFilter::NEAREST, ax::backend::SamplerFilter::NEAREST, ax::backend::SamplerAddressMode::REPEAT, ax::backend::SamplerAddressMode::REPEAT });
	_pBar->setAnchorPoint({0, 0});
	_pBar->setPosition({2.f, 1.25f});

	// add everything
	_pGroove->addChild(_pBar, -1);
	addChild(_pGroove);

	scheduleUpdate();
	return true;
}
void SimpleProgressBar::update(float delta)
{
	if(_pPercentage > 100.f)
	{
		_pPercentage = 100.f;
	}
	if(_pPercentage < 0.f)
	{
		_pPercentage = 0.f;
	}
	_pBar->setTextureRect({0, 0, _pBarWidth * (this->_pPercentage / 100), _pBar->getContentSize().height});
	return;
}