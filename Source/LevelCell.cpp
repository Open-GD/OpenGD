#include "LevelCell.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ui/UIScale9Sprite.h"
#include "LevelInfoLayer.h"
#include "ButtonSprite.h"

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

void LevelCell::updateBGColor(int num) {
	BGColor = ax::LayerColor::create();
	if (num % 2 == 0) BGColor->initWithColor({ 161, 88, 44, 255 });
	else BGColor->initWithColor({ 191, 114, 62, 255 });
	BGColor->setContentSize(this->getContentSize());
	BGColor->setPositionY(5);
	layer->addChild(BGColor, -1);
}

bool LevelCell::init(GJGameLevel* level) {
	setContentSize({396, 90});
	layer = ax::Layer::create();
	addChild(layer);

	auto bigFontStr = GameToolbox::getTextureString("bigFont.fnt");
	
	if (level->_stars > 0) {
		auto starSprite = ax::Sprite::createWithSpriteFrameName("star_small01_001.png");
		starSprite->setPosition({34, 29});
		layer->addChild(starSprite);

		auto starText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_stars));
		starText->setScale(0.4f);
		starText->setPosition({26, 29});
		starText->setAnchorPoint({1, 0.5});
		layer->addChild(starText);
	}

	auto nameText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_levelName));
	nameText->setScale(0.8f);
	if(nameText->getContentSize().width > 180) nameText->setScale(nameText->getScale() * (180 / nameText->getContentSize().width));
	nameText->setPosition({50, 75});
	nameText->setAnchorPoint({0, 0.5});
	layer->addChild(nameText);

	ax::Label* songText = nullptr;

	if (level->_songID == 0)
	{
		songText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", LevelTools::getAudioTitle(level->_officialSongID)));
		songText->setColor(ax::Color3B(39, 206, 250));
	}
	else
	{
		songText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_songName));
		songText->setColor(ax::Color3B(255, 132, 211));
	}

	songText->setScale(0.5f, 0.5f);
	songText->setPosition({52, 33});
	songText->setAnchorPoint({0, 0.5});
	layer->addChild(songText);

	ax::Vec2 diffPos = {26, 59};

	if (level->_epic == 1)
	{
		auto epicSprite = ax::Sprite::createWithSpriteFrameName("GJ_epicCoin_001.png");
		epicSprite->setPosition(diffPos);
		layer->addChild(epicSprite, 2);
	}
	else if (level->_featureScore > 0)
	{
		auto featureScore = ax::Sprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
		featureScore->setPosition(diffPos);
		layer->addChild(featureScore, 2);
	}

	auto diffSprite = ax::Sprite::createWithSpriteFrameName(GJGameLevel::getDifficultySprite(level));
	diffSprite->setPosition(diffPos);
	layer->addChild(diffSprite, 2);

	auto lenSprite = ax::Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
	lenSprite->setPosition({59, 14});
	lenSprite->setScale(0.6f);
	layer->addChild(lenSprite, 2);

	auto lenText = ax::Label::createWithBMFont(bigFontStr, GameToolbox::lengthString(level->_length));
	lenText->setScale(0.4f);
	if (lenText->getContentSize().width > 120) lenText->setScale(lenText->getScale() * (120 / lenText->getContentSize().width));
	lenText->setPosition({ 69, 14 });
	lenText->setAnchorPoint({ 0, 0.5 });
	layer->addChild(lenText);

	auto dwnSprite = ax::Sprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	dwnSprite->setPosition({136.4, 14});
	dwnSprite->setScale(0.6f);
	layer->addChild(dwnSprite, 2);

	auto dwnText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_downloads));
	dwnText->setScale(0.4f);
	if (dwnText->getContentSize().width > 120) dwnText->setScale(dwnText->getScale() * (120 / dwnText->getContentSize().width));
	dwnText->setPosition({145.4, 14});
	dwnText->setAnchorPoint({0, 0.5});
	layer->addChild(dwnText);

	auto likeSprite = ax::Sprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
	likeSprite->setPosition({212.8, 14});
	likeSprite->setScale(0.6f);
	layer->addChild(likeSprite, 2);

	auto likeText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_likes));
	likeText->setScale(0.4f);
	if (likeText->getContentSize().width > 120) likeText->setScale(likeText->getScale() * (120 / likeText->getContentSize().width));
	likeText->setPosition({222.8, 14});
	likeText->setAnchorPoint({0, 0.5});
	layer->addChild(likeText);

	auto menu = ax::Menu::create();
	menu->setPosition({315.45, 45});
	layer->addChild(menu);

	auto creatorText = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", level->_levelCreator));
	creatorText->setScale(0.6f);
	if (level->_levelCreator == "-") creatorText->setColor(ax::Color3B(90, 255, 255)); // thanks gd colon
	if (creatorText->getContentSize().width > 200) creatorText->setScale(creatorText->getScale() * (200 / creatorText->getContentSize().width));
	creatorText->setPosition({-263.525, 8});
	creatorText->setAnchorPoint({0, 0.5});
	menu->addChild(creatorText);

	auto levelBadgePosition = creatorText->getContentSize().width * creatorText->getScale() + 65;
	
	if (level->_copiedID > 0)
	{
		auto collabSprite = ax::Sprite::createWithSpriteFrameName("collaborationIcon_001.png");
		collabSprite->setPosition(levelBadgePosition, 52);
		layer->addChild(collabSprite);
	}
	if (level->_objects > 40000)
	{
		auto highObjectSprite = ax::Sprite::createWithSpriteFrameName("highObjectIcon_001.png");
		highObjectSprite->setPosition(levelBadgePosition + (level->_copiedID > 0 ? 18 : 0), 52);
		layer->addChild(highObjectSprite);
	}

	auto button = MenuItemSpriteExtra::create(ButtonSprite::create("View", 0x32, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30), [&, level](Node* btn)
		{
			ax::Director::getInstance()->pushScene((ax::TransitionFade::create(0.5f, LevelInfoLayer::scene(level))));
		});
	menu->addChild(button);

	return true;
}