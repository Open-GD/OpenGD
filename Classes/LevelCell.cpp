#include "LevelCell.h"
#include "GameToolbox.h"

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
    nameText->setPosition({50, 75});
    nameText->setAnchorPoint({0, 0.5});
    layer->addChild(nameText);

    auto songText = ax::Label::createWithBMFont("bigFont-uhd.fnt", fmt::format("{}", level->_SondURL));
    songText->setScale(0.5f, 0.5f);
    songText->setPosition({52, 33});
    songText->setAnchorPoint({0, 0.5});
    layer->addChild(songText);

    auto diffSprite = ax::Sprite::createWithSpriteFrameName(GJGameLevel::getDifficultySprite(level));
    diffSprite->setPosition({26, 59});
    addChild(diffSprite, 2);

    return true;
}