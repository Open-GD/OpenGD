#include "LoadingLayer.h"
#include "GameToolbox.h"
#include "MenuLayer.h"
#include "CocosExplorer.h"
#include "constants.h"

#include <array>

USING_NS_AX;
using GameToolbox::getTextureString, GameToolbox::log;

constexpr static auto splashes = std::to_array <const char*>({
	"Use practice mode to learn the layout of a level",
	"Back for more are ya?",
	"Customize your character's icon and color!",
	"If at first you don't succeed, try, try again...",
	"Listen to the music to help time your jumps",
	"Loading resources",
	"Here be dragons...",
	"Can you beat them all?",
	"Hold down to keep jumping",
	"Pro tip: Don't crash",
	"Spikes are not your friends, don't forget to jump",
	"You can download all songs from the level select page!",
	"Go online to play other players levels!",
	"Build your own levels using the level editor",
	"Does anyone even read this?",
	"Pro tip: Jump",
	"Waiting for planets to align",
	"Collecting scrap metal",
	"Looking for pixels",
	"The spikes whisper to me...",
	"What if the spikes are the good guys?",
	"Loading awesome soundtracks...",
	"Hiding secrets",
	"Calculating chance of success",
	"Programmer is sleeping, please wait",
	"Drawing pretty pictures",
	"Wandering around aimlessly",
	"Starting the flux capacitor",
	"Loading the progressbar",
	"Where did I put that coin...",
	"Loading Rage Cannon",
	"Fus Ro DASH!",
	"It's all in the timing",
	"Counting to 1337",
	"Play, Crash, Rage, Quit, Repeat",
	"RobTop is Love, RobTop is Life",
	"Such wow, very amaze.",
	"Only one button required to crash",
	"The Vault Keeper's name is 'Spooky'...",
	"This seems like a good place to hide a secret...",
	"Shhhh! You're gonna wake the big one!",
	"Hope the big guy doesn't wake up...",
	"Spikes... OF DOOM!",
	"Fake spikes are fake",
	"Loading will be finished... soon",
	"Why don't you go outside?",
	"Programming amazing AI",
	"It's Over 9000!",
	"Spooky doesn't get out much",
	"Hiding secret vault",
	"A wild RubRub appeared!",
	"I have been expecting you.",
	"Hiding rocket launcher",
	"So many secrets...",
	"Why u have to be mad?",
	"I don't know how this works...",
	"Unlock new icons and colors by completing achievements!",
	"It is only game...",
	"Warp Speed",
	"RubRub was here",
	"Hold on, reading the manual",
	"So, what's up?"
});

constexpr static auto pngs = std::to_array<const char*>({
	"GJ_GameSheetGlow.png", "GJ_GameSheet04.png", "GJ_GameSheet03.png", "GJ_GameSheet02.png", "GJ_GameSheet.png", "GJ_gradientBG.png", "edit_barBG_001.png", "GJ_button_01.png",
	"gravityOverlay.png", "goldFont.png", "bigFont.png", "chatFont.png", "CCControlColourPickerSpriteSheet.png"
});

constexpr static auto fonts = std::to_array<const char*>({
	"bigFont.fnt", "chatFont.fnt", "goldFont.fnt"
});

constexpr static auto plists = std::to_array<const char*>({
	"GJ_GameSheetGlow.plist", "GJ_GameSheet.plist", "CCControlColourPickerSpriteSheet.plist", "GJ_GameSheet02.plist", "GJ_GameSheet03.plist",
	"GJ_GameSheet04.plist"
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
	bgSpr->setScale(winSize.width / bgSpr->getContentSize().width);
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
	_pBar = SimpleProgressBar::create();
	_pBar->setPercentage(0.f);
	_pBar->setPosition({ winSize.width / 2, 100.0f });
	this->addChild(_pBar);
	
	this->runAction(Sequence::create(DelayTime::create(0), CallFunc::create([&]() { this->loadAssets(); }), nullptr));
	
#if SHOW_IMGUI == true
	CocosExplorer::openForever();
#endif
	
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
	GameToolbox::log("loading asset {} out of {}", (int)m_nAssetsLoaded, (int)m_nTotalAssets);
	_pBar->setPercentage((m_nAssetsLoaded / m_nTotalAssets)*100.f);

	if(m_nAssetsLoaded == m_nTotalAssets) {
		Director::getInstance()->replaceScene(MenuLayer::scene());
	}
}