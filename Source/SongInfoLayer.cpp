#include "SongInfoLayer.h"
#include <ui/UIScale9Sprite.h>
#include "GameToolbox.h"
#include "LevelTools.h"
#include "MenuItemSpriteExtra.h"
#include "TextButton.h"

USING_NS_AX;

SongInfoLayer* SongInfoLayer::create(int soundtrackID)
{
	auto artistID = LevelTools::getArtistForAudio(soundtrackID);

	return SongInfoLayer::create(
		LevelTools::getAudioTitle(soundtrackID), LevelTools::getNameForArtist(artistID),
		LevelTools::getURLForAudio(soundtrackID), LevelTools::getNgURLForArtist(artistID),
		LevelTools::getYtURLForArtist(artistID), LevelTools::getFbURLForArtist(artistID)
	);
}

SongInfoLayer* SongInfoLayer::create(std::string_view songName, std::string_view artistName, std::string_view urlLink,
	std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink)
{
	auto pRet = new SongInfoLayer();
	
	if (pRet && pRet->init(songName, artistName, urlLink, ngArtistLink, ytArtistLink, fbArtistLink))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

bool SongInfoLayer::init(std::string_view songName, std::string_view artistName, std::string_view urlLink,
	std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink)
{
	if (!PopupLayer::init())
		return false;
	
	auto winSize = Director::getInstance()->getWinSize();

	_downloadLink = urlLink;
	_ngArtistLink = ngArtistLink;
	_ytArtistLink = ytArtistLink;
	_fbArtistLink = fbArtistLink;


	auto bg = ax::ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_square01.png"), {0, 0, 80, 80});
	bg->setContentSize({ 420.0, 260.0 });
	bg->setPosition(winSize / 2);

	this->_mainLayer->addChild(bg);


	auto songLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), songName);
	songLabel->setPosition({ winSize.width / 2, (winSize.height / 2) + 90 });

	if (songLabel->getContentSize().width > 300.0f)
		songLabel->setScale(300 / songLabel->getContentSize().width);

	this->_mainLayer->addChild(songLabel);

	auto artistLabel = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", artistName));
	artistLabel->setPosition({ winSize.width / 2, (songLabel->getPosition().height + 4) - 30});

	if (artistLabel->getContentSize().width > 300.0f)
		artistLabel->setScale(300 / artistLabel->getContentSize().width);

	if (artistLabel->getScale() >= 0.8)
		artistLabel->setScale(0.8);

	this->_mainLayer->addChild(artistLabel);


	auto closeBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_closeBtn_001.png"), [&](Node*)
	{
		this->close();
	});
	auto closeBtnMenu = Menu::create();
	closeBtnMenu->addChild(closeBtn);

	closeBtnMenu->setPosition({ (winSize.width / 2) - 210 + 10, (winSize.height / 2) + 130 - 10 });
	
	this->_mainLayer->addChild(closeBtnMenu);

	
	auto dls = TextButton::create("Download Soundtrack", GameToolbox::getTextureString("bigFont.fnt"), 236.000, 30, [&](TextButton*)
	{
		Application::getInstance()->openURL(_downloadLink);
	});
	dls->setPosition(closeBtnMenu->convertToNodeSpace(artistLabel->getPosition()) + Vec2(0, -50));
	closeBtnMenu->addChild(dls);

	auto moreByLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), fmt::format("More By {}", artistName));
	moreByLabel->setPosition(artistLabel->getPosition() + Vec2(0, -110));
	moreByLabel->setScale(0.5);
	this->_mainLayer->addChild(moreByLabel);

	ax::Vector<Node*> socials;

	if (!ngArtistLink.empty())
	{
		auto ngButton = TextButton::create("Newgrounds", GameToolbox::getTextureString("bigFont.fnt"), 96.000, 30, [&](TextButton*)
		{
			Application::getInstance()->openURL(_ngArtistLink);
		});
		socials.pushBack(ngButton);
		closeBtnMenu->addChild(ngButton);
	}

	if (!ytArtistLink.empty())
	{
		auto ytButton = TextButton::create("Youtube", GameToolbox::getTextureString("bigFont.fnt"), 96.000, 30, [&](TextButton*)
		{
			Application::getInstance()->openURL(_ytArtistLink);
		});
		socials.pushBack(ytButton);
		closeBtnMenu->addChild(ytButton);
	}

	if (!fbArtistLink.empty())
	{
		auto ytButton = TextButton::create("Facebook", GameToolbox::getTextureString("bigFont.fnt"), 96.000, 30, [&](TextButton*)
		{
			Application::getInstance()->openURL(_fbArtistLink);
		});
		socials.pushBack(ytButton);
		closeBtnMenu->addChild(ytButton);
	}

	GameToolbox::alignItemsHorizontally(socials, 20, closeBtnMenu->convertToNodeSpace({ winSize.width / 2, ((winSize.height / 2) - 130.0f) + 40.0f }));

	return true;
}
