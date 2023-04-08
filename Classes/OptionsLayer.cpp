#include "OptionsLayer.h"
#include "TextButton.h"
#include "GameManager.h"

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
	
	auto mediumBtn = TextButton::create("Medium", [this](TextButton* btn){
		GameManager::getInstance()->setQualityMedium();
	});
	mediumBtn->setPositionX(-75);
	
	auto highBtn = TextButton::create("High", [this](TextButton* btn){
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