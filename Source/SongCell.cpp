#include "SongCell.h"
#include "GameToolbox.h"
#include "TextButton.h"
#include "LevelTools.h"
#include "ui/UIScale9Sprite.h"
#include "MenuItemSpriteExtra.h"
#include "SongInfoLayer.h"

USING_NS_AX;

SongCell* SongCell::create(SongObject* songInfo)
{
	auto pRet = new SongCell();

	if (pRet && pRet->init(songInfo))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void SongCell::updateBGColor(int num)
{
	_bgColor = ax::LayerColor::create();
	if (num % 2 == 0) _bgColor->initWithColor({ 161, 88, 44, 255 });
	else _bgColor->initWithColor({ 191, 114, 62, 255 });
	_bgColor->setContentSize(this->getContentSize());
	_bgColor->setPosition(this->getPosition());
	this->addChild(_bgColor, -1);
}

bool SongCell::init(SongObject* songInfo)
{
	this->setContentSize({ 356, 60 });

	_layer = ax::Layer::create();
	_layer->setContentSize(this->getContentSize());
	this->addChild(_layer);

	auto contentSize = this->getContentSize();
	auto sID = songInfo->_songID;


	// Song info
	auto songName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), fmt::format("{}", LevelTools::getAudioTitle(sID)));
	songName->setAnchorPoint({ 0.0, 0.5 });
	songName->setScale(0.7);
	songName->setPosition({ 10, (contentSize.height / 2) + 10 });

	if (songName->getContentSize().width > 240)
		songName->setScale(240 / songName->getContentSize().width);
	
	if (songName->getScale() >= 0.7)
		songName->setScale(0.7);

	auto artistName = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), fmt::format("By {}", LevelTools::getNameForArtist(LevelTools::getArtistForAudio(sID))));
	artistName->setAnchorPoint({ 0.0, 0.5 });
	artistName->setScale(0.7);
	artistName->setPosition(songName->getPosition() + Vec2(2, -21));

	if (artistName->getContentSize().width > 140)
		artistName->setScale(140 / artistName->getContentSize().width);

	if (artistName->getScale() >= 0.7)
		artistName->setScale(0.7);

	_layer->addChild(artistName);
	_layer->addChild(songName);


	// View button
	auto scale9 = ax::ui::Scale9Sprite::create(GameToolbox::getTextureString("GJ_button_01.png"));
	scale9->setPosition({ 31.55, 15 });
	scale9->setContentSize({ 63.1, 30 });

	auto view = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "View");
	view->setPosition({ scale9->getContentSize().width / 2 - 2, scale9->getContentSize().height / 2 + 2 });
	view->setScale(0.6f);
	scale9->addChild(view);

	auto button = MenuItemSpriteExtra::create(scale9, [sID](Node* btn)
	{
			SongInfoLayer::create(sID)->show();
	});
	button->setContentSize({88.340, 42});
	button->setPosition({10, 30});
	auto viewBtnMenu = Menu::create();
	viewBtnMenu->setPosition({315.675, 5});
	viewBtnMenu->addChild(button);
	_layer->addChild(viewBtnMenu);


	// Outlines for the cell
	auto color = Color4B(0, 0, 0, 50);

	auto topLine = DrawNode::create();
	topLine->drawRect(Vec2(0, 0), Vec2(contentSize.width, 0), color);
	_layer->addChild(topLine, _layer->getLocalZOrder() + 1);

	auto bottomLine = DrawNode::create();
	bottomLine->drawRect(Vec2::ZERO, Vec2(contentSize.width, 0), color);
	bottomLine->setPosition(Vec2(0, contentSize.height));
	_layer->addChild(bottomLine, _layer->getLocalZOrder() + 1);


	return true;
}
