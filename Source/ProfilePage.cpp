#include "ProfilePage.h"

#include <MenuItemSpriteExtra.h>
#include "GameToolbox.h"
#include <ui/CocosGUI.h>
#include "ButtonSprite.h"
#include "2d/CCMenu.h"
#include "ccUTF8.h"
#include "base/CCDirector.h"
#include "LoadingCircle.h"
#include "SimplePlayer.h"
#include "GameManager.h"
#include "GJUserScore.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"

USING_NS_AX;

ProfilePage* ProfilePage::create(int accountID, bool mainMenuProfile)
{
	ProfilePage* pRet = new ProfilePage();

	if (pRet && pRet->init(accountID, mainMenuProfile))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void ProfilePage::loadPageFromUserInfo(GJUserScore* score) // replace with 'GJUserScore* score'
{
	const auto& winSize = Director::getInstance()->getWinSize();

	//GameToolbox::limitLabelWidth(playerName, 185.0f, 0.9f, 0.0f);

	auto playerStatsNode = Node::create();
	std::string statSpriteName;

	bool hasCP = (score->_creatorpoints > 0) ? true : false;

	int widthLimit = hasCP ? 50 : 60;
	int statValue;

	for (int i = 0; i < (hasCP ? 6 : 5); i++)
	{
		switch (i)
		{
			case 1:
				statSpriteName = "GJ_diamondsIcon_001.png";
				statValue = score->_diamonds;
				break;
			case 2:
				statSpriteName = "GJ_coinsIcon_001.png";
				statValue = score->_secretCoins;
				break;
			case 3:
				statSpriteName = "GJ_coinsIcon2_001.png";
				statValue = score->_usercoins;
				break;
			case 4:
				statSpriteName = "GJ_demonIcon_001.png";
				statValue = score->_demons;
				break;
			case 5:
				statSpriteName = "GJ_hammerIcon_001.png";
				statValue = score->_creatorpoints;
				break;
			default:
				statSpriteName = "GJ_starsIcon_001.png";
				statValue = score->_stars;
				break;
		}
		auto statValueLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), std::to_string(statValue));
		GameToolbox::limitLabelWidth(statValueLabel, widthLimit, 0.6, 0.0);

		playerStatsNode->addChild(statValueLabel);

		auto statSprite = Sprite::createWithSpriteFrameName(statSpriteName);

		playerStatsNode->addChild(statSprite);
		
	}
	
	float offset = 420.0;

	if (true) // if has more than 2 social medias linked
	{
		offset = 340.0;
	}

	playerStatsNode->setPosition(winSize.width / 2 - offset / 2, winSize.height / 2 + 85);
	this->_mainLayer->addChild(playerStatsNode);

	IconType gamemode;
	int iconID;

	for (int i = 0; i < 7; i++)
	{
		switch(i)
		{
			case 0:
				gamemode = kIconTypeCube;
				iconID = score->_accIcon;
				break;
			case 1:
				gamemode = kIconTypeShip;
				iconID = score->_accShip;
				break;
			case 2:
				gamemode = kIconTypeBall;
				iconID = score->_accBall;
				break;
			case 3:
				gamemode = kIconTypeUfo;
				iconID = score->_accBird;
				break;
			case 4:
				gamemode = kIconTypeWave;
				iconID = score->_accDart;
				break;
			case 5:
				gamemode = kIconTypeRobot;
				iconID = score->_accRobot;
				break;
			case 6:
				gamemode = kIconTypeSpider;
				iconID = score->_accSpider;
				break;
		}

		SimplePlayer* iconSprite = SimplePlayer::create(iconID);
		iconSprite->updateGamemode(iconID, gamemode);
		ax::Vec2 iconPos = {i * 48.0f - 144.0f, 0.0f};
		iconSprite->setPosition(iconPos.operator+({winSize.width / 2, winSize.height / 2 + 40.0f}));
		iconSprite->setMainColor(GameToolbox::colorForIdx(score->_playerColor));
		iconSprite->setSecondaryColor(GameToolbox::colorForIdx(score->_playerColor2));
		if (score->_accGlow){iconSprite->setGlow(true);iconSprite->setGlowColor(GameToolbox::colorForIdx(score->_playerColor2));}
		this->_mainLayer->addChild(iconSprite);
	}
}

bool ProfilePage::init(int accountID, bool mainMenuProfile)
{
	GameToolbox::log("{}",accountID);
	if (!PopupLayer::init()) return false;
	const auto& winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"));
	bg->setContentSize({ 440.0, 290.0 });
	bg->setPosition(winSize / 2);

	this->_mainLayer->addChild(bg);

	auto playerName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Hello World");
	playerName->setPosition({ winSize.width / 2, ((winSize.height / 2) + 145.0f) - 20.0f});
	GameToolbox::limitLabelWidth(playerName, 185.0f, 0.9f, 0.0f);

	this->_mainLayer->addChild(playerName);

	auto floorLine = Sprite::createWithSpriteFrameName("floorLine_001.png");
	floorLine->setOpacity(100);
	floorLine->setScaleX(0.8f);
	floorLine->setPosition({ winSize.width / 2, bg->getPositionY() + 100.0f});

	this->_mainLayer->addChild(floorLine);

	// TODO: GJCommentListLayer position: -170.0f,-22.5f;

	auto closeBtnSprite = Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
	auto closeBtn = MenuItemSpriteExtra::create(closeBtnSprite, [&](Node*) { this->close(); });

	auto menu = Menu::create();
	menu->setPosition((winSize.width / 2 - 220.0f) + 10.0f, ((winSize.y / 2) + 145.0f) - 10.0f );
	menu->addChild(closeBtn);

	_errorMsg = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), "Something went wrong...");
	_errorMsg->setScale(0.6f);
	_errorMsg->setPosition({ winSize.width / 2, winSize.height / 2 - 53.0f});
	_errorMsg->setVisible(false);

	this->_mainLayer->addChild(_errorMsg);

	if (true) // complete if statement here: if registered player id is not equal to ProfilePage player id?
	{
		const char* heartSprite = "gj_heartOn_001.png";
		
		if (true) // complete if statement here: if not following, FUN_004a4950 is the function that checks if the player is followed or not
		{
			heartSprite = "gj_heartOff_001.png";
		}
		auto heartObject = MenuItemSpriteExtra::create(heartSprite, [&](Node*) { /*Follow Function*/ });
		heartObject->setScaleMultiplier(1.1f);
		ax::Vec2 heartPosition = { (winSize.width / 2) - 198.0f, (winSize.height / 2) - 123.0f };
		heartObject->setPosition(menu->convertToNodeSpace(heartPosition));
		
		if (true) // complete if statement here: if not following
		{
			auto followTxtSprite = Sprite::createWithSpriteFrameName("GJ_followTxt_001.png");
			followTxtSprite->setPosition(heartPosition.operator+({48.0f,-2.0f}));
			
			this->_mainLayer->addChild(followTxtSprite);
		}

		menu->addChild(heartObject);
		
	}

	_loadingcircle = LoadingCircle::create();
	_loadingcircle->setPosition({ winSize.width / 2, winSize.height / 2 - 53.0f});
	this->_mainLayer->addChild(_loadingcircle);

	auto refreshBtnSprite = Sprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
	auto refreshBtn = MenuItemSpriteExtra::create(refreshBtnSprite, [&](Node*) { /*Refresh Function*/ });

	refreshBtn->setPosition(menu->convertToNodeSpace({(winSize.width / 2 - 220.0f) + 10.0f, (winSize.height / 2 - 145.0f) + 10.0f + 1.0f}));
	refreshBtn->setScaleMultiplier(1.5f);

	std::string postData = fmt::format("secret=Wmfd2893gb7&targetAccountID={}", accountID);
	
	auto _request = new ax::network::HttpRequest();
	_request->setUrl("http://www.boomlings.com/database/getGJUserInfo20.php");
	_request->setRequestType(ax::network::HttpRequest::Type::POST);
	_request->setHeaders(std::vector<std::string>{"user-agent: "});
	_request->setRequestData(postData.c_str(), postData.length());
	_request->setResponseCallback(AX_CALLBACK_2(ProfilePage::onHttpRequestCompleted, this));
	_request->setTag("valid");
	ax::network::HttpClient::getInstance()->send(_request);
	_request->release();

	menu->addChild(refreshBtn);

	this->_mainLayer->addChild(menu);

	return true;
}

void ProfilePage::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	if (auto str = GameToolbox::getResponse(response))
	{
		GameToolbox::log("{}", (*str));
		if (*str == "-1") return;
		auto score = GJUserScore::createWithResponse((*str));
		loadPageFromUserInfo(score);
		return;
	}
	_errorMsg->setVisible(true);
}
