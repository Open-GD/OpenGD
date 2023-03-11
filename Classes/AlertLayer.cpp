#include "AlertLayer.h"
#include <ui/CocosGUI.h>
#include "GameToolbox.h"

USING_NS_AX;

bool AlertLayer::init(std::string title, std::string desc, std::string btn1, std::string btn2, float width, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback) {
    if (!PopupLayer::init()) return false;

    auto winSize = Director::getInstance()->getWinSize();

    auto descLabel = Label::createWithBMFont(GameToolbox::getTextureString("chatFont.fnt"), desc, TextHAlignment::CENTER);
    // descLabel->setAnchorPoint({0.5, 1});
    descLabel->setDimensions(width - 120, 0);
    descLabel->setPosition(Point(winSize / 2) + Vec2(0, 10));
    this->m_pMainLayer->addChild(descLabel);
    auto descHeight = std::max(descLabel->getContentSize().height + 240, 280.f);

    auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square01_001.png"));
    bg->setStretchEnabled(false);
    bg->setContentSize({width, descHeight});
    bg->setPosition(winSize / 2);
    this->m_pMainLayer->addChild(bg, -1);

    auto titleLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), title);
    titleLabel->setAnchorPoint({0.5, 1.0});
    titleLabel->setPosition({winSize.width / 2, (winSize.height - descHeight) / 2 + descHeight - 30});
    titleLabel->setScale(0.9f);
    this->m_pMainLayer->addChild(titleLabel);

    auto menu = Menu::create();
    this->m_pMainLayer->addChild(menu);
    menu->setPositionY((winSize.height - descHeight) / 2 + 60);

    auto button1 = TextButton::create(btn1, GameToolbox::getTextureString("goldFont.fnt"), 180, 0, (btn1Callback == NULL) ? [=](TextButton*) {
        this->close();
    } : btn1Callback);
    menu->addChild(button1);
    this->m_pBtn1 = button1;

    if (btn2 != "") {
        auto button2 = TextButton::create(btn2, GameToolbox::getTextureString("goldFont.fnt"), 180, 0, (btn2Callback == NULL) ? [=](TextButton*) {
            this->close();
        } : btn2Callback);
        menu->addChild(button2);
        this->m_pBtn2 = button2;

        menu->alignItemsHorizontallyWithPadding(MIN((width - (button1->getContentSize().width + button2->getContentSize().width)) / 2, 30));
    }

    return true;
}

AlertLayer* AlertLayer::create(std::string title, std::string desc, std::string btn1, std::string btn2, float width, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback) {
    auto pRet = new(std::nothrow) AlertLayer();

    if (pRet && pRet->init(title, desc, btn1, btn2, width, btn1Callback, btn2Callback)) {
        pRet->autorelease();
        return pRet;
    } else {
        AX_SAFE_DELETE(pRet);
        return nullptr;
    }
}

AlertLayer* AlertLayer::create(std::string title, std::string desc, std::string btn1, std::string btn2, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback) {
    return AlertLayer::create(title, desc, btn1, btn2, 500, btn1Callback, btn2Callback);
}

AlertLayer* AlertLayer::create(std::string title, std::string desc, std::string btn1, float width, std::function<void(TextButton*)> btn1Callback) {
    return AlertLayer::create(title, desc, btn1, "", width, btn1Callback, NULL);
}

AlertLayer* AlertLayer::create(std::string title, std::string desc, std::string btn1, std::function<void(TextButton*)> btn1Callback) {
    return AlertLayer::create(title, desc, btn1, "", 500, btn1Callback, NULL);
}

AlertLayer* AlertLayer::create(std::string title, std::string desc, float width) {
    return AlertLayer::create(title, desc, "OK", "", width, NULL, NULL);
}

AlertLayer* AlertLayer::create(std::string title, std::string desc) {
    return AlertLayer::create(title, desc, "OK", "", 500, NULL, NULL);
}

void AlertLayer::setBtn1Callback(std::function<void(TextButton*)> callback) {
    this->m_pBtn1->setCallback(callback);
}

void AlertLayer::setBtn2Callback(std::function<void(TextButton*)> callback) {
    this->m_pBtn2->setCallback(callback);
}