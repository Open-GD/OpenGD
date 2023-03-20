#include "MoreGamesLayer.h"

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

bool MoreGamesLayer::init(){
    auto scrollLayer = ax::Layer::create();
    auto menu = ax::Menu::create();

	auto promo1 = PromoItemSprite::create("promo_mu.png", [&](ax::Node* btn){
        ax::Application::getInstance()->openURL("http://www.robtopgames.com/dowmload/mu0");
    });
	promo1->setPosition(menu->convertToNodeSpace({0, -promo1->getContentSize().height + 1}));
	promo1->setAnchorPoint({0.5, 0.0});
    promo1->setScale(2);

	auto promo2 = PromoItemSprite::create("promo_mm.png", [&](ax::Node* btn){
        ax::Application::getInstance()->openURL("http://www.robtopgames.com/dowmload/mm0");
    });
	promo2->setPosition(menu->convertToNodeSpace({0, -promo1->getContentSize().height * 3 + 1}));
	promo2->setAnchorPoint({0.5, 0.0});
    promo2->setScale(2);

	menu->addChild(promo1);
	menu->addChild(promo2);

    scrollLayer->addChild(menu);
    auto dropdownlayer = DropDownLayer::create(scrollLayer, "RobTop Games");
    dropdownlayer->showLayer();

    return true;
}