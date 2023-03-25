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
	
	if (level->_Stars > 0) {
		auto starSprite = ax::Sprite::createWithSpriteFrameName("star_small01_001.png");
		starSprite->setPosition({34, 29});
		layer->addChild(starSprite);

		auto starText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_Stars));
		starText->setScale(0.4f);
		starText->setPosition({26, 29});
		starText->setAnchorPoint({1, 0.5});
		layer->addChild(starText);
	}

	auto nameText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_LevelName));
	nameText->setScale(0.8f);
	if(nameText->getContentSize().width > 180) nameText->setScale(nameText->getScale() * (180 / nameText->getContentSize().width));
	nameText->setPosition({50, 75});
	nameText->setAnchorPoint({0, 0.5});
	layer->addChild(nameText);

	ax::Label* songText = nullptr;

	if (level->_SongID == 0)
	{
		songText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", LevelTools::getAudioTitle(level->_OfficialSongID)));
		songText->setColor(ax::Color3B(39, 206, 250));
	}
	else
	{
		songText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_SongName));
		songText->setColor(ax::Color3B(255, 132, 211));
	}

	songText->setScale(0.5f, 0.5f);
	songText->setPosition({52, 33});
	songText->setAnchorPoint({0, 0.5});
	layer->addChild(songText);

	ax::Vec2 diffPos = {26, 59};

	if (level->_Epic == 1)
	{
		auto epicSprite = ax::Sprite::createWithSpriteFrameName("GJ_epicCoin_001.png");
		epicSprite->setPosition(diffPos);
		layer->addChild(epicSprite, 2);
	}
	else if (level->_FeatureScore > 0)
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

	auto lenText = ax::Label::createWithBMFont(bigFontStr, GameToolbox::lengthString(level->_Length));
	lenText->setScale(0.4f);
	if (lenText->getContentSize().width > 120) lenText->setScale(lenText->getScale() * (120 / lenText->getContentSize().width));
	lenText->setPosition({ 69, 14 });
	lenText->setAnchorPoint({ 0, 0.5 });
	layer->addChild(lenText);

	auto dwnSprite = ax::Sprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	dwnSprite->setPosition({136.4, 14});
	dwnSprite->setScale(0.6f);
	layer->addChild(dwnSprite, 2);

	auto dwnText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_Downloads));
	dwnText->setScale(0.4f);
	if(dwnText->getContentSize().width > 120) dwnText->setScale(dwnText->getScale() * (120 / dwnText->getContentSize().width));
	dwnText->setPosition({145.4, 14});
	dwnText->setAnchorPoint({0, 0.5});
	layer->addChild(dwnText);

	auto likeSprite = ax::Sprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
	likeSprite->setPosition({212.8, 14});
	likeSprite->setScale(0.6f);
	layer->addChild(likeSprite, 2);

	auto likeText = ax::Label::createWithBMFont(bigFontStr, fmt::format("{}", level->_Likes));
	likeText->setScale(0.4f);
	if(likeText->getContentSize().width > 120) likeText->setScale(likeText->getScale() * (120 / likeText->getContentSize().width));
	likeText->setPosition({222.8, 14});
	likeText->setAnchorPoint({0, 0.5});
	layer->addChild(likeText);

	auto menu = ax::Menu::create();
	menu->setPosition({315.45, 45});
	layer->addChild(menu);

	auto creatorText = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", level->_LevelCreator));
	creatorText->setScale(0.6f);
	if (level->_LevelCreator == "-") creatorText->setColor(ax::Color3B(90, 255, 255)); // thanks gd colon
	if (creatorText->getContentSize().width > 200) creatorText->setScale(creatorText->getScale() * (200 / creatorText->getContentSize().width));
	creatorText->setPosition({-263.525, 8});
	creatorText->setAnchorPoint({0, 0.5});
	menu->addChild(creatorText);

	auto levelBadgePosition = creatorText->getContentSize().width * creatorText->getScale() + 65;
	
	if (level->_CopiedID > 0)
	{
    auto collabSprite = ax::Sprite::createWithSpriteFrameName("collaborationIcon_001.png");
    collabSprite->setPosition(levelBadgePosition, 52);
    layer->addChild(collabSprite);
	}
	if (level->_Objects > 40000)
	{
    auto highObjectSprite = ax::Sprite::createWithSpriteFrameName("highObjectIcon_001.png");
    highObjectSprite->setPosition(levelBadgePosition + (level->_CopiedID > 0 ? 18 : 0), 52);
    layer->addChild(highObjectSprite);
	}

	auto scale9 = ax::ui::Scale9Sprite::create("GJ_button_01-uhd.png");
	scale9->setPosition({31.55, 15});
	scale9->setContentSize({63.1, 30});

	auto view = ax::Label::createWithBMFont(bigFontStr, "View");
	view->setPosition({ scale9->getContentSize().width / 2 - 2, scale9->getContentSize().height / 2 + 2 });
	view->setScale(0.6f);
	scale9->addChild(view);

	auto button = MenuItemSpriteExtra::create(scale9, [&, level](Node* btn) 
		{
			ax::Director::getInstance()->pushScene((ax::TransitionFade::create(0.5f, LevelInfoLayer::scene(level))));
		});
	button->setContentSize({88.340, 42});
	button->setPosition({10, 11});
	menu->addChild(button);

	return true;
}