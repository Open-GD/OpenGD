#include "LevelCell.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ui/UIScale9Sprite.h"
#include "LevelInfoLayer.h"

LevelCell* LevelCell::create(GJGameLevel* level) {
    LevelCell* ret = new LevelCell();
    if (ret->init(level)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool LevelCell::init(GJGameLevel* level) {
    setContentSize({396, 90});
    auto layer = ax::Layer::create();
    addChild(layer);

    if (level->_Stars > 0) {
        auto starSprite = ax::Sprite::createWithSpriteFrameName("star_small01_001.png");
        starSprite->setPosition({34, 29});
        layer->addChild(starSprite);

        auto starText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("{}", level->_Stars));
        starText->setScale(0.4f);
        starText->setPosition({26, 29});
        starText->setAnchorPoint({1, 0.5});
        layer->addChild(starText);
    }

    auto nameText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("{}", level->_LevelName));
    nameText->setScale(0.8f);
    if(nameText->getContentSize().width > 180) nameText->setScale(nameText->getScale() * (180 / nameText->getContentSize().width));
    nameText->setPosition({50, 75});
    nameText->setAnchorPoint({0, 0.5});
    layer->addChild(nameText);

    auto songText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("{}", level->_SongName));
    songText->setScale(0.5f, 0.5f);
    songText->setPosition({52, 33});
    songText->setAnchorPoint({0, 0.5});
    layer->addChild(songText);

    auto diffSprite = ax::Sprite::createWithSpriteFrameName(GJGameLevel::getDifficultySprite(level));
    diffSprite->setPosition({26, 59});
    layer->addChild(diffSprite, 2);

    auto lenSprite = ax::Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    lenSprite->setPosition({59, 14});
    lenSprite->setScale(0.6f);
    layer->addChild(lenSprite, 2);

    auto dwnSprite = ax::Sprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
    dwnSprite->setPosition({136.4, 14});
    dwnSprite->setScale(0.6f);
    layer->addChild(dwnSprite, 2);

    auto dwnText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("{}", level->_Downloads));
    dwnText->setScale(0.4f);
    if(dwnText->getContentSize().width > 120) dwnText->setScale(dwnText->getScale() * (120 / dwnText->getContentSize().width));
    dwnText->setPosition({145.4, 14});
    dwnText->setAnchorPoint({0, 0.5});
    layer->addChild(dwnText);

    auto likeSprite = ax::Sprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
    likeSprite->setPosition({212.8, 14});
    likeSprite->setScale(0.6f);
    layer->addChild(likeSprite, 2);

    auto likeText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("{}", level->_Likes));
    likeText->setScale(0.4f);
    if(likeText->getContentSize().width > 120) likeText->setScale(likeText->getScale() * (120 / likeText->getContentSize().width));
    likeText->setPosition({222.8, 14});
    likeText->setAnchorPoint({0, 0.5});
    layer->addChild(likeText);

    auto menu = ax::Menu::create();
    menu->setPosition({315.45, 45});
    layer->addChild(menu);

    auto creatorText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("By {}", level->_LevelCreator.empty() ? "-" : level->_LevelCreator));
    creatorText->setScale(0.6f);
    if(creatorText->getContentSize().width > 200) creatorText->setScale(creatorText->getScale() * (200 / creatorText->getContentSize().width));
    creatorText->setPosition({-260.525, 8});
    creatorText->setAnchorPoint({0, 0.5});
    menu->addChild(creatorText);

    auto scale9 = ax::ui::Scale9Sprite::create("GJ_button_01-uhd.png");
    scale9->setPosition({31.55, 15});
    scale9->setContentSize({63.1, 30});

    auto view = ax::Label::createWithBMFont("bigFont-uhd.fnt", "View");
    view->setPosition({30.55, 17});
    view->setScale(0.8);

    auto button = MenuItemSpriteExtra::create(scale9, [&, level](Node* btn) 
		{
			ax::Director::getInstance()->pushScene((ax::TransitionFade::create(0.5f, LevelInfoLayer::scene(level))));
		});
    button->setContentSize({88.340, 42});
    button->setPosition({10, 21});
    button->addChild(view);
    menu->addChild(button);

    return true;
}