#include "GarageLayer.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "MenuLayer.h"
#include "SimplePlayer.h"
#include "GameManager.h"

USING_NS_AX;

Scene* GarageLayer::scene()
{
	auto s = Scene::create();
	s->addChild(GarageLayer::create());
	return s;
}

GarageLayer* GarageLayer::create()
{
	auto r = new GarageLayer();
	if (r && r->init())
		r->autorelease();
	else
	{
		delete r;
		r = nullptr;
	}
	return r;
}

bool GarageLayer::init()
{
	if (!Scene::init())
		return false;

	GameManager::getInstance()->_openedGarage = true;
	
	auto director = Director::getInstance();
	auto size  = director->getWinSize();

	GameToolbox::createBG(this, { 150, 150, 150 });
	GameToolbox::createCorners(this, true, false, true, true);

	_userNameField = ui::TextField::create("Username", GameToolbox::getTextureString("bigFont.fnt"), 20);
	_userNameField->setPlaceHolderColor({120, 170, 240});
	_userNameField->setMaxLength(10);
	_userNameField->setMaxLengthEnabled(true);
	_userNameField->setCursorEnabled(true);
	_userNameField->setString("Player");
	_userNameField->setPosition({ size.width / 2, size.height - 34 });
	this->addChild(_userNameField);

	auto line = Sprite::createWithSpriteFrameName("floorLine_001.png");
	line->setBlendFunc(GameToolbox::getBlending());
	line->setPosition({ size.width / 2, size.height / 2 + 50 });
	this->addChild(line);

	_iconPrev = SimplePlayer::create(109); // 132
	_iconPrev->setPosition({line->getPositionX(), line->getPositionY() + 25});
	_iconPrev->setMainColor({125, 0, 255});
	_iconPrev->setSecondaryColor({0, 255, 255});
	_iconPrev->setGlow(true);
	_iconPrev->setGlowColor({0, 255, 0});
	_iconPrev->setScale(1.6f);
	this->addChild(_iconPrev);

	this->setupIconSelect();

	auto menu	= Menu::create();
	menu->setPosition({0, 0});

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [=](Node*) {
		director->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
	});
	backBtn->setPosition({24, size.height - 23});
	//backBtn->setSizeMult(1.6f);
	menu->addChild(backBtn);

	auto shop = MenuItemSpriteExtra::create("shopRope_001.png", [=](Node*) {
		_iconPrev->updateGamemode(35, kIconTypeUfo);
	});

	shop->setPosition({135, size.height - 25});
	shop->setDestination({0, -15});
	menu->addChild(shop);

	auto shard = MenuItemSpriteExtra::create("GJ_shardsBtn_001.png", [=](Node*) {

	});
	shard->setPosition({30, size.height - 80});
	menu->addChild(shard);

	auto paint = MenuItemSpriteExtra::create("GJ_shardsBtn_001.png", [=](Node*) {

	});
	paint->setPosition({30, size.height - 120});
	menu->addChild(paint);

	
	// estadisticas
	this->createStat("GJ_starsIcon_001.png", "6");
	this->createStat("GJ_coinsIcon_001.png", "8");
	this->createStat("GJ_coinsIcon2_001.png", "12");
	this->createStat("currencyOrbIcon_001.png", "14");
	this->createStat("GJ_diamondsIcon_001.png", "13");
	//this->createStat("GJ_demonIcon_001.png", "5");

	this->addChild(menu);

#ifdef AX_PLATFORM_PC
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed  = AX_CALLBACK_2(GarageLayer::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(GarageLayer::onKeyReleased, this);
	director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	return true;
}

void GarageLayer::createStat(const char* sprite, const char* statKey)
{
	auto size = Director::getInstance()->getWinSize();

	auto stat = Sprite::createWithSpriteFrameName(sprite);
	stat->setScale(.65);
	stat->setPosition({size.width - 20, size.height - 14 - 20 * stats});
	this->addChild(stat);

	auto statL = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "0"); // GameStatsManager::sharedState()->getStat(statKey)
	statL->setScale(.45);
	statL->setAnchorPoint({1, 0});
	statL->setPosition({stat->getPositionX() - 16, stat->getPositionY() - 7});
	this->addChild(statL);

	stats++;
}

void GarageLayer::setupIconSelect()
{
	auto size = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("square02_001.png"));
	bg->setContentSize({385, 100});
	bg->setOpacity(75);
	bg->setPosition({size.width / 2, size.height / 2 - 65});
	this->addChild(bg);

	auto unlock = Sprite::createWithSpriteFrameName("GJ_unlockTxt_001.png");
	unlock->setPosition({size.width / 2, bg->getPositionY() + bg->getContentSize().height / 2 + 12});
	this->addChild(unlock);

	auto menu = Menu::create();

	for (int i = 0; i < 9; i++)
	{
		auto s1 = Sprite::createWithSpriteFrameName(this->getSpriteName(i, false));
		s1->setScale(.9f);
		auto s2 = Sprite::createWithSpriteFrameName(this->getSpriteName(i, true));
		s2->setScale(s1->getScale());

		auto i1 = MenuItemSpriteExtra::create(s1, [=](Node* a)
		{
			int tag = a->getTag();
			if (tag == 7)
				tag = kIconTypeSpecial;
			if (tag == 8)
				tag = kIconTypeDeathEffect;

			this->setupPage((IconType)tag);
		});
		i1->setTag(i);
		menu->addChild(i1);

		//auto i1 = MenuItemToggler::create(s1, s2, this, menu_selector(SaiGarageLayer::onSelectTab));
		//i1->setSizeMult(1.2f);
		//i1->setTag(i);
		//i1->setClickable(false);
		//menu->addChild(i1);
	}

	menu->alignItemsHorizontallyWithPadding(0);
	menu->setPosition({size.width / 2, unlock->getPositionY() + 30 + 3});
	this->addChild(menu);

	auto menuArr = Menu::create();
	menuArr->setPosition({0, 0});

	// Robtop aqui hace otra peruanada de usar "GJ_arrow_%02d_001.png" para las flechas etc...

	auto arrow1 = Sprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	arrow1->setScale(.8f);
	auto arrBtn1 = MenuItemSpriteExtra::create(arrow1, [=](Node*)
	{

	});
	//arrBtn1->setSizeMult(2.2f);
	arrBtn1->setPosition({bg->getPositionX() - 220, bg->getPositionY()});
	menuArr->addChild(arrBtn1);

	auto arrow2 = Sprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	arrow2->setScale(.8f);
	arrow2->setFlippedX(true);
	auto arrBtn2 = MenuItemSpriteExtra::create(arrow2, [=](Node*)
	{

	});
	//arrBtn2->setSizeMult(2.2f);
	arrBtn2->setPosition({bg->getPositionX() + 220, bg->getPositionY()});
	menuArr->addChild(arrBtn2);

	this->addChild(menuArr);

	m_pSelect = Sprite::createWithSpriteFrameName("GJ_select_001.png");
	m_pSelect->setScale(.9f);
	this->addChild(m_pSelect);

	this->setupPage(kIconTypeCube);
}

const char* GarageLayer::getSpriteName(int id, bool actived)
{
	switch (id)
	{
		case 0: return actived ? "gj_iconBtn_on_001.png" : "gj_iconBtn_off_001.png";
		case 1:	return actived ? "gj_shipBtn_on_001.png" : "gj_shipBtn_off_001.png";
		case 2: return actived ? "gj_ballBtn_on_001.png" : "gj_ballBtn_off_001.png";
		case 3: return actived ? "gj_birdBtn_on_001.png" : "gj_birdBtn_off_001.png";
		case 4: return actived ? "gj_dartBtn_on_001.png" : "gj_dartBtn_off_001.png";
		case 5: return actived ? "gj_robotBtn_on_001.png" : "gj_robotBtn_off_001.png";
		case 6: return actived ? "gj_spiderBtn_on_001.png" : "gj_spiderBtn_off_001.png";
		case 7: return actived ? "gj_streakBtn_on_001.png" : "gj_streakBtn_off_001.png";
		case 8: return actived ? "gj_explosionBtn_on_001.png" : "gj_explosionBtn_off_001.png";
	}
}

void GarageLayer::setupPage(IconType type)
{
		selectedMode = type;
		// aqui robtop hace cosas con funciones del gamemanager, ni idea
		auto size = Director::getInstance()->getWinSize();

		if (menuIcons) this->removeChild(menuIcons);

		menuIcons = Menu::create();
		menuIcons->setPosition(0, 0);

		float paddingX = 0, paddingY = 0;
		for (int i = 1; i <= 36; i++)
		{
			if (i > GameToolbox::getValueForGamemode(type))
				break;
			
			auto browserItem = Sprite::createWithSpriteFrameName("playerSquare_001.png");
			browserItem->setOpacity(0);
	
			if (type == kIconTypeSpecial)
			{
				auto icono = Sprite::createWithSpriteFrameName(StringUtils::format("player_special_%02d_001.png", i));
				icono->setPosition(browserItem->getContentSize() / 2);
				icono->setScale(27.0f / icono->getContentSize().width);
				browserItem->addChild(icono);
			}
			else if (type == kIconTypeDeathEffect)
			{
				auto icono = Sprite::createWithSpriteFrameName(StringUtils::format("explosionIcon_%02d_001.png", i));
				icono->setPosition(browserItem->getContentSize() / 2);
				icono->setScale(0.9f);
				browserItem->addChild(icono);
			} 
			else 
			{
				auto icono = SimplePlayer::create(0);
				icono->updateGamemode(i, type);
				icono->setMainColor({175, 175, 175});
				icono->setPosition(browserItem->getContentSize() / 2);
				if (type == kIconTypeUfo)
				{
					icono->setPositionY(icono->getPositionY() + 5);
					icono->m_pDomeSprite->setVisible(false);
				}
				icono->setScale(27.0f / icono->m_pMainSprite->getContentSize().width);
				browserItem->addChild(icono);
			}
			
			// if (!gm->isIconUnlocked(i, type))
			// {
				// auto icon = reinterpret_cast<Sprite*>(browserItem->getChildren()->objectAtIndex(0));
				// icon->setOpacity(75);
	
				// const char* name = "GJ_lock_001.png";
				// float scale	  = .75f;  // 2147483648, 4294967295
	
				// if (GameStatsManager::sharedState()->getStoreItem(
						// i, AchievementManager::sharedState()->convertIconTypeToUnlockType(type))) {
					// name  = "storeItemIcon_001.png";
					// scale = .9f;
				// }
	
				// auto lock = Sprite::createWithSpriteFrameName(name);
				// lock->setScale(scale);
				// lock->setPosition(browserItem->getContentSize() / 2);
				// browserItem->addChild(lock);
			// }
	
			auto btn = MenuItemSpriteExtra::create(browserItem, [=](Node* a)
			{ 
				m_pSelect->setPosition(a->getPosition());
				_iconPrev->updateGamemode(a->getTag(), selectedMode);
			});
			btn->setTag(i);
			btn->setPosition({size.width / 2 - 165 + paddingX, size.height / 2 - 65 + 30 - paddingY});
			menuIcons->addChild(btn);
	
			if (i % numPerRow == 0)
			{
				paddingX = 0;
				paddingY += 30;
			}
			else
				paddingX += 30;
		}

		this->addChild(menuIcons);
}

void GarageLayer::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event)
{
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_BACK:
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
	}
}

void GarageLayer::onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
}
