#include "AlertLayer.h"
#include <ui/CocosGUI.h>
#include "GameToolbox.h"

USING_NS_AX;

bool AlertLayer::init(std::string_view title, std::string_view desc, std::string_view btn1Str, std::string_view btn2Str, float width, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback)
{
	if (!PopupLayer::init()) 
		return false;

	auto winSize = Director::getInstance()->getWinSize();

	auto descLabel = Label::createWithBMFont(GameToolbox::getTextureString("chatFont.fnt"), desc, TextHAlignment::CENTER);
	width = descLabel->getContentSize().width + 32;
	// descLabel->setAnchorPoint({0.5, 1});
	descLabel->setDimensions(width, 0);
	descLabel->setPosition(Point(winSize / 2));
	this->_mainLayer->addChild(descLabel);
	auto descHeight = std::max(descLabel->getContentSize().height, 150.f);

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square01_001.png"));
	bg->setStretchEnabled(true);
	bg->setContentSize({width, descHeight});
	bg->setPosition(winSize / 2);
	this->_mainLayer->addChild(bg, -1);

	std::string goldFontStr = GameToolbox::getTextureString("goldFont.fnt");
	
	auto titleLabel = Label::createWithBMFont(goldFontStr, title);
	titleLabel->setAnchorPoint({0.5, 1.0});
	titleLabel->setPosition({winSize.width / 2, (winSize.height - descHeight) / 2 + descHeight - 15});
	titleLabel->setScale(0.9f);
	this->_mainLayer->addChild(titleLabel);

	auto menu = Menu::create();
	this->_mainLayer->addChild(menu);
	menu->setPositionY((winSize.height - descHeight) / 2 + 60);
	
	auto _btnCallback = btn1Callback ? btn1Callback : [this](TextButton*) {this->close(); };
	
	_btn1 = TextButton::create(btn1Str, goldFontStr, 0, 0, _btnCallback);
	menu->addChild(_btn1);

	if (!btn2Str.empty())
	{
		auto _btn2Callback = btn2Callback ? btn2Callback : [this](TextButton*) {this->close(); };
		_btn2 = TextButton::create(btn2Str, goldFontStr, 0, 0, _btn2Callback);
		menu->addChild(_btn2);

		menu->alignItemsHorizontallyWithPadding(MIN((width - (_btn1->getContentSize().width + _btn2->getContentSize().width)) / 2, 30));
	}
	menu->setPositionY(descHeight - 32);

	return true;
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, float width, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback)
{
	auto pRet = new(std::nothrow) AlertLayer();

	if (pRet && pRet->init(title, desc, btn1, btn2, width, btn1Callback, btn2Callback))
	{
		pRet->autorelease();
		return pRet;
	} 
	else 
	{
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback) {
	return AlertLayer::create(title, desc, btn1, btn2, 500, btn1Callback, btn2Callback);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, float width, std::function<void(TextButton*)> btn1Callback) {
	return AlertLayer::create(title, desc, btn1, "", width, btn1Callback, nullptr);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, std::string_view btn1, std::function<void(TextButton*)> btn1Callback) {
	return AlertLayer::create(title, desc, btn1, "", 500, btn1Callback, nullptr);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc, float width) {
	return AlertLayer::create(title, desc, "OK", "", width, nullptr, nullptr);
}

AlertLayer* AlertLayer::create(std::string_view title, std::string_view desc) {
	return AlertLayer::create(title, desc, "OK", "", 500, nullptr, nullptr);
}

void AlertLayer::setBtn1Callback(std::function<void(TextButton*)> callback) {
	this->_btn1->setCallback(callback);
}

void AlertLayer::setBtn2Callback(std::function<void(TextButton*)> callback) {
	this->_btn2->setCallback(callback);
}