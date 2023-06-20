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