#include "LoadingLayer.h"
#include "GameToolbox.h"
#include "MenuLayer.h"
#include "CocosExplorer.h"

#include <array>

USING_NS_AX;
using GameToolbox::getTextureString, GameToolbox::log;

constexpr static auto splashes = std::to_array <const char*>({
	"Listen to the music to help time your jumps", 
	"Back for more are ya?", 
	"Use practice mode to learn the layout of a level", 
	"Build your own levels using the level editor", 
	"Go online to play other players levels!", 
	"If at first you don't succeed, try, try again...",
	"Can you beat them all?",
	"Customize your character's icon and color!",
	"You can download all songs from the level select page!",
	"Spikes are not your friends, don't forget to jump",
	"Unlock new icons and colors by completing achievements!"
});

constexpr static auto pngs = std::to_array<const char*>({
	"GJ_GameSheet.png", "GJ_gradientBG.png", "edit_barBG_001.png", "GJ_button_01.png",
	"gravityOverlay.png", "goldFont.png", "bigFont.png", "chatFont.png", "CCControlColourPickerSpriteSheet.png"
});

constexpr static auto fonts = std::to_array<const char*>({
	"bigFont.fnt", "chatFont.fnt", "goldFont.fnt"
});
	
constexpr static auto plists = std::to_array<const char*>({
	"GJ_GameSheet.plist", "CCControlColourPickerSpriteSheet.plist",
});
	
	
const char* LoadingLayer::getSplash() {
	return splashes[rand() % (splashes.size()-1)];
}

Scene* LoadingLayer::scene() {
	auto scene = Scene::create();
	scene->addChild(LoadingLayer::create());
	return scene;
}

bool LoadingLayer::init() {
	if (!Layer::init()) return false;
	
	
	size_t totalAssets = fonts.size() + plists.size() + pngs.size();
	this->m_nTotalAssets = static_cast<int>(totalAssets);
	
	Director::getInstance()->getTextureCache()->addImage(getTextureString("GJ_LaunchSheet.png"));
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(getTextureString("GJ_LaunchSheet.plist"));

	auto winSize = Director::getInstance()->getWinSize();

	auto bgSpr = Sprite::create(getTextureString("game_bg_01_001.png"));
	bgSpr->setStretchEnabled(false);
	bgSpr->setPosition(winSize / 2);
	bgSpr->setScale(1.185f);
	bgSpr->setColor({ 0, 102, 255 });
	this->addChild(bgSpr);

	auto logoSpr = Sprite::createWithSpriteFrameName("GJ_logo_001.png");
	logoSpr->setStretchEnabled(false);
	logoSpr->setPosition(winSize / 2);
	this->addChild(logoSpr);
	
	auto robLogoSpr = Sprite::createWithSpriteFrameName("RobTopLogoBig_001.png");
	robLogoSpr->setStretchEnabled(false);
	robLogoSpr->setPosition({ 284.5f, 240.0f });
	this->addChild(robLogoSpr);

	auto splash = this->getSplash();
	auto splashText = Label::createWithBMFont(getTextureString("goldFont.fnt"), splash);
	splashText->setPosition({ winSize.width / 2, 60.0f});
	splashText->setScale(0.7f);

	this->addChild(splashText);
	m_pGrooveSprite = Sprite::create(getTextureString("slidergroove.png"));
	m_pGrooveSprite->setStretchEnabled(false);
	m_pGrooveSprite->setPosition({ winSize.width / 2, 100.0f });
	this->m_fTotalBarWidth = m_pGrooveSprite->getContentSize().width - 8;
	this->addChild(m_pGrooveSprite);

	m_pBarSprite = Sprite::create(getTextureString("sliderBar.png"));
	m_pBarSprite->setStretchEnabled(false);
	m_pBarSprite->getTexture()->setTexParameters({ backend::SamplerFilter::NEAREST, backend::SamplerFilter::NEAREST, backend::SamplerAddressMode::REPEAT, backend::SamplerAddressMode::REPEAT });
	m_pBarSprite->setAnchorPoint({0, 0});
	m_pBarSprite->setPosition({ 2.0f, 4.0f });
	m_pGrooveSprite->addChild(m_pBarSprite, -1);
	
	this->runAction(Sequence::create(DelayTime::create(0), CallFunc::create([&]() { this->loadAssets(); }), nullptr));
	
	CocosExplorer::openForever();
	
	return true;
}



void LoadingLayer::loadAssets() {
	
	auto textureCache = Director::getInstance()->getTextureCache();
	for(auto image : pngs) {
		GameToolbox::log("image {}", image);
		textureCache->addImageAsync(getTextureString(image), AX_CALLBACK_1(LoadingLayer::assetLoaded, this));
	}
	
	auto frameCache = SpriteFrameCache::getInstance();
	for(auto plist : plists) {
		GameToolbox::log("plist {}", plist);
		frameCache->addSpriteFramesWithFile(getTextureString(plist));
		this->assetLoaded(nullptr);
	}
	
	for(auto fnt : fonts) {
		GameToolbox::log("font {}", fnt);
		auto label = Label::createWithBMFont(getTextureString(fnt), "someText");
		this->assetLoaded(nullptr);
	}
}

void LoadingLayer::assetLoaded(ax::Ref*) {
	
	this->m_nAssetsLoaded++;
	GameToolbox::log("loading asset {}", m_nAssetsLoaded);
	m_pBarSprite->setTextureRect({0, 0, m_fTotalBarWidth * (this->m_nAssetsLoaded / this->m_nTotalAssets), m_pBarSprite->getContentSize().height});
	
	if(m_nAssetsLoaded == m_nTotalAssets) {
		Director::getInstance()->replaceScene(MenuLayer::scene());
	}
}