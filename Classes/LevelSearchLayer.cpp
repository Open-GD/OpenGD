#include "AlertLayer.h"
#include "CreatorLayer.h"
#include "GameToolbox.h"
#include <iostream>
#include "LevelSearchLayer.h"
#include "MenuItemSpriteExtra.h"
#include <ui/CocosGUI.h>

USING_NS_AX;

const Color3B SELECTED_COLOR = { 255, 255, 255 };
const Color3B UNSELECTED_COLOR = { 125, 125, 125 };

Menu* LevelSearchLayer::m_pDifficultyFilterMenu = nullptr;
Menu* LevelSearchLayer::m_pLengthFilterMenu = nullptr;
TextInputNode* LevelSearchLayer::m_pTextField = nullptr;

// these are supposed to be stored in GameManager, but we dont have that yet.
std::vector<int> LevelSearchLayer::m_dSelectedDifficulties = {};
std::vector<int> LevelSearchLayer::m_dSelectedTimes = {};


Sprite* searchButton(std::string texture, std::string text, float scale, std::string icon)
{
	auto sprite = Sprite::createWithSpriteFrameName(texture);
	auto menu = Menu::create();

	auto content_size = sprite->getContentSize();
	auto label = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), text);
	label->setScale(scale);
	menu->addChild(label);

	if (!icon.empty()) {
		auto icon_sprite = Sprite::createWithSpriteFrameName(icon);
		icon_sprite->setScale(1.1f);
		menu->addChild(icon_sprite);
	}

	menu->setPosition(content_size / 2);
	menu->alignItemsHorizontally();

	sprite->addChild(menu);

	return sprite;
}

Scene* LevelSearchLayer::scene()
{
	auto scene = Scene::create();
	scene->addChild(LevelSearchLayer::create());
	return scene;
}

void LevelSearchLayer::toggleTime(Ref* btn)
{
	auto tag = dynamic_cast<Node*>(btn)->getTag();

	auto selected = std::find(
		m_dSelectedTimes.begin(), m_dSelectedTimes.end(), tag
	) != m_dSelectedTimes.end();

	if (selected)
	{
		std::erase(m_dSelectedTimes, tag);
		m_pLengthFilterMenu->getChildByTag(tag)->setColor(UNSELECTED_COLOR);
	}
	else
	{
		m_pLengthFilterMenu->getChildByTag(tag)->setColor(SELECTED_COLOR);
		m_dSelectedTimes.push_back(tag);
	}
}

void LevelSearchLayer::toggleDifficulty(Node* btn)
{
	int tag = btn->getTag();

	auto naPos = std::find(m_dSelectedDifficulties.begin(), m_dSelectedDifficulties.end(), 0);
	auto isNaSelected = naPos != m_dSelectedDifficulties.end();

	if (isNaSelected && tag != 0)
	{
		m_dSelectedDifficulties.erase(naPos);
		m_pDifficultyFilterMenu->getChildByTag(0)->setColor(UNSELECTED_COLOR);
	}

	auto selected = std::find(
		m_dSelectedDifficulties.begin(), m_dSelectedDifficulties.end(), tag
	) != m_dSelectedDifficulties.end();

	if (selected)
	{
		std::erase(m_dSelectedDifficulties, tag);
		m_pDifficultyFilterMenu->getChildByTag(tag)->setColor(UNSELECTED_COLOR);
	}
	else
	{
		if (tag == 0) {
			for (int tag : m_dSelectedDifficulties)
				m_pDifficultyFilterMenu->getChildByTag(tag)->setColor(UNSELECTED_COLOR);
			m_dSelectedDifficulties.clear();
		}

		m_pDifficultyFilterMenu->getChildByTag(tag)->setColor(SELECTED_COLOR);
		m_dSelectedDifficulties.push_back(tag);
	}
}

bool LevelSearchLayer::init()
{
	if (!Layer::init()) return false;

	auto winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create(GameToolbox::getTextureString("GJ_gradientBG.png"));
	bg->setScaleX(winSize.width / bg->getContentSize().width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setAnchorPoint({ 0, 0 });
	bg->setColor({ 0, 102, 255 });
	this->addChild(bg);

	GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = Menu::create();
	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) {
		Director::getInstance()->replaceScene(TransitionFade::create(.5, CreatorLayer::scene()));
	});
	backBtnMenu->addChild(backBtn);
	backBtnMenu->setPosition({ 24.0, winSize.height - 23.0f });
	this->addChild(backBtnMenu);


	// Search bar, search button
	auto searchBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	searchBg->setOpacity(255);
	searchBg->setContentSize({ 365.0, 40.0 });
	searchBg->setPosition({ winSize.width / 2, (winSize.height / 2) + 120.0f });
	searchBg->setScale(1.0f);
	searchBg->setColor({ 8, 56, 141 });

	this->addChild(searchBg);

	auto menuSearch = Menu::create();
	auto searchBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn02_001.png", "Search", 0.6, ""), [&](Node*) {
	});
	menuSearch->addChild(searchBtn);
	menuSearch->setPosition({ (winSize.width / 2) + 128.0f, (winSize.height / 2) + 120.0f });

	this->addChild(menuSearch);

	auto field = TextInputNode::create(240.f, 30.f, GameToolbox::getTextureString("bigFont.fnt").c_str(), "Search for a level or id", 18);
	field->m_pTextField->setAnchorPoint({ 0.0, 0.5 });
	field->m_pTextField->setMaxLength(20);
	field->m_pTextField->setMaxLengthEnabled(true);

	field->m_pPlaceholder->setColor({ 120, 170, 240 });
	field->setPosition(winSize / 2);
	field->setPosition({ (winSize.width / 2) - 174.0f, (winSize.height / 2) + 105.f });
	m_pTextField = field;

	this->addChild(field);

	// Quick Search
	auto quickSearchLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Quick Search", TextHAlignment::CENTER);
	quickSearchLabel->setPosition({ winSize.width / 2, (winSize.height / 2) + 25.0f + 59.0f});
	quickSearchLabel->setScale(0.6f);
	this->addChild(quickSearchLabel);

	auto quickSearchBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	quickSearchBg->setOpacity(255);
	quickSearchBg->setContentSize({ 365.0, 90.0 });
	quickSearchBg->setPosition({ winSize.width / 2, (winSize.height / 2) + 25.0f });
	quickSearchBg->setScale(1.0f);
	quickSearchBg->setColor({ 0, 46, 117 });

	this->addChild(quickSearchBg);

	auto quickSearchMenu = Menu::create();
	quickSearchMenu->setPosition(winSize.width / 2, (winSize.height / 2) * 1.16);

	auto mostDownloadedBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn03_001.png", "Most Downloaded", 0.45, "GJ_sDownloadIcon_001.png"), [&](Node*) {});
	quickSearchMenu->addChild(mostDownloadedBtn);

	auto mostLikedBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn03_001.png", "Most Liked", 0.6, "GJ_sLikeIcon_001.png"), [&](Node*) {});
	quickSearchMenu->addChild(mostLikedBtn);

	auto trendingBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn03_001.png", "Trending", 0.6, "GJ_sTrendingIcon_001.png"), [&](Node*) {});
	quickSearchMenu->addChild(trendingBtn);
	
	auto mostRecentBtn = MenuItemSpriteExtra::create(searchButton("GJ_longBtn03_001.png", "Most Recent", 0.6, "GJ_sRecentIcon_001.png"), [&](Node*) {});
	quickSearchMenu->addChild(mostRecentBtn);

	quickSearchMenu->alignItemsVertically();

	mostDownloadedBtn->setPosition({ -90, 20 });
	mostLikedBtn->setPosition({ 90, 20 });
	trendingBtn->setPosition({ -90, -20 });
	mostRecentBtn->setPosition({ 90, -20 });

	this->addChild(quickSearchMenu);


	// Difficulty filters
	auto filterLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Filters", TextHAlignment::CENTER);
	filterLabel->setPosition({ winSize.width / 2, ((winSize.height / 2) - 70.0f) + 39.0f });
	filterLabel->setScale(0.6f);
	this->addChild(filterLabel);

	auto filterBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	filterBg->setOpacity(255);
	filterBg->setContentSize({ 365.0, 50.0 });
	filterBg->setPosition({ winSize.width / 2, (winSize.height / 2) - 70.0f });
	filterBg->setScale(1.0f);
	filterBg->setColor({ 0, 39, 98 });
	this->addChild(filterBg);

	m_pDifficultyFilterMenu = Menu::create();

	for (size_t i = 0; i < 6; i++)
	{
		auto texture_name = StringUtils::format("difficulty_%02d_btn_001.png", i);
		auto diff_sprite = Sprite::createWithSpriteFrameName(texture_name);

		auto diff_spriteextra = MenuItemSpriteExtra::create(diff_sprite, LevelSearchLayer::toggleDifficulty);
		diff_spriteextra->setColor(UNSELECTED_COLOR);
		diff_spriteextra->setTag(i);
		diff_spriteextra->setScale(1.0f);

		m_pDifficultyFilterMenu->addChild(diff_spriteextra);
	}

	m_pDifficultyFilterMenu->alignItemsHorizontallyWithPadding(25);
	m_pDifficultyFilterMenu->setPosition({ winSize.width / 2, (winSize.height / 2) - 70.0f });

	this->addChild(m_pDifficultyFilterMenu);

	for (size_t tag : m_dSelectedDifficulties)
	{
		m_pDifficultyFilterMenu->getChildByTag(tag)->setColor(SELECTED_COLOR);
	}


	// Length filter
	auto lengthFilterBg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02b_001.png"));
	lengthFilterBg->setOpacity(255);
	lengthFilterBg->setContentSize({ 365.0, 35.0 });
	lengthFilterBg->setPosition({ winSize.width / 2, (winSize.height / 2) - 125.0f });
	lengthFilterBg->setScale(1.0f);
	lengthFilterBg->setColor({ 0, 31, 79 });

	this->addChild(lengthFilterBg);

	m_pLengthFilterMenu = Menu::create();

	auto timeIcon = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png"), [&](Node*) {});
	timeIcon->setEnabled(false);

	m_pLengthFilterMenu->addChild(timeIcon);
	for (int i = 0; i < 4; ++i)
	{
		const char* length = [&](int i) {
			switch (i) {
			case 0:
				return "Tiny";
			case 1:
				return "Short";
			case 2:
				return "Medium";
			default:
				return "Long";
			}
		}(i);

		// MenuItemSpriteExtra breaks padding for some reason
		auto label = MenuItemLabel::create(Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt").c_str(), length, TextHAlignment::CENTER), LevelSearchLayer::toggleTime);
		label->setColor(UNSELECTED_COLOR);
		label->setScale(0.6f);
		label->setTag(i);

		m_pLengthFilterMenu->addChild(label);
	}

	m_pLengthFilterMenu->setPosition(lengthFilterBg->getPosition());
	m_pLengthFilterMenu->setContentSize({ 365.0f, 35.0f });
	m_pLengthFilterMenu->alignItemsHorizontallyWithPadding(20);

	this->addChild(m_pLengthFilterMenu);

	for (size_t tag : m_dSelectedTimes)
	{
		m_pLengthFilterMenu->getChildByTag(tag)->setColor(SELECTED_COLOR);
	}

	return true;
}
