#include "MoreGamesLayer.h"
#include "DropDownLayer.h"
#include "2d/CCMenu.h"
#include "2d/CCLabel.h"
#include "CCApplication.h"
#include "GameToolbox.h"

MoreGamesLayer* MoreGamesLayer::create(){
	auto pRet = new(std::nothrow) MoreGamesLayer();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool MoreGamesLayer::init()
{
	auto layer = ax::Layer::create();

	std::string bigFontStr = GameToolbox::getTextureString("bigFont.fnt");
	const auto& winSize = ax::Director::getInstance()->getWinSize();

	auto label = ax::Label::createWithBMFont(bigFontStr, "Nothing here yet... sorry :(");
	label->setScale(0.6f);
	label->setPosition(0, 0);
	layer->addChild(label);

	auto dropdownlayer = DropDownLayer::create(layer, "RobTop Games");
	dropdownlayer->showLayer(true, false);

	return true;
}