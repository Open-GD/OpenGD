#include "OptionsLayer.h"
#include "ButtonSprite.h"
#include "GameManager.h"
#include "MenuItemSpriteExtra.h"
#include "2d/CCMenu.h"
#include "DropDownLayer.h"

USING_NS_AX;

OptionsLayer* OptionsLayer::create()
{
	auto pRet = new(std::nothrow) OptionsLayer();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

bool OptionsLayer::init()
{
	auto scrollLayer = Layer::create();
	
	auto mediumBtn = MenuItemSpriteExtra::create(ButtonSprite::create("Medium"), [](Node*) {
		GameManager::getInstance()->setQualityMedium();
	});
	mediumBtn->setPositionX(-75);
	
	auto highBtn = MenuItemSpriteExtra::create(ButtonSprite::create("High"), [](Node*) {
		GameManager::getInstance()->setQualityHigh();
	});
	highBtn->setPositionX(75);

	auto menu = Menu::create(mediumBtn, highBtn, nullptr);
	menu->setPosition(0, 0);
	scrollLayer->addChild(menu);
	auto dropdownlayer = DropDownLayer::create(scrollLayer, "Settings");
	dropdownlayer->showLayer(true, false);
	
	return true;
}