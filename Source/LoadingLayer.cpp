/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "LoadingLayer.h"

#include "MenuLayer.h"
#include "CocosExplorer.h"
#include "GameManager.h"

#include "external/constants.h"
#include <array>
#include "2d/SpriteFrameCache.h"
#include "Director.h"
#include "renderer/TextureCache.h"
#include "2d/Label.h"
#include "SimpleProgressBar.h"
#include "2d/ActionInterval.h"
#include "2d/ActionInstant.h"
#include "GameToolbox/log.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/rand.h"

USING_NS_AX;


LoadingLayer* LoadingLayer::create() {
	LoadingLayer* pRet = new LoadingLayer();
	if (pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}


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
	"gravityOverlay.png", "goldFont.png", "bigFont.png", "chatFont.png", "CCControlColourPickerSpriteSheet.png", "GJ_ShopSheet.png", "SecretSheet.png"
});

constexpr static auto fonts = std::to_array<const char*>({
	"bigFont.fnt", "chatFont.fnt", "goldFont.fnt"
});

constexpr static auto plists = std::to_array<const char*>({
	"GJ_GameSheetGlow.plist", "GJ_GameSheet.plist", "CCControlColourPickerSpriteSheet.plist", "GJ_GameSheet02.plist", "GJ_GameSheet03.plist",
	"GJ_GameSheet04.plist", "GJ_ShopSheet.plist", "SecretSheet.plist",
});
	
	
const char* LoadingLayer::getSplash() {
	return splashes[GameToolbox::randomInt(0, splashes.size() - 1)];
}

Scene* LoadingLayer::scene() {
	auto scene = Scene::create();
	scene->addChild(LoadingLayer::create());
	return scene;
}

bool LoadingLayer::init() {
	if (!Layer::init()) return false;
	
	auto dir = Director::getInstance();

	_sprFrameCache = SpriteFrameCache::getInstance();
	_textureCache = dir->getTextureCache();
	
	dir->setContentScaleFactor(GameManager::getInstance()->isHigh() ? 4.0f : 2.0f);
	
	dir->purgeCachedData();
	_sprFrameCache->removeSpriteFrames();
	_textureCache->removeAllTextures();

	size_t totalAssets = fonts.size() + plists.size() + pngs.size();
	this->m_nTotalAssets = static_cast<int>(totalAssets);
	
	_textureCache->addImage(GameToolbox::getTextureString("GJ_LaunchSheet.png"));
	_sprFrameCache->addSpriteFramesWithFile(GameToolbox::getTextureString("GJ_LaunchSheet.plist"));

	auto winSize = dir->getWinSize();

	auto bgSpr = Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
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
	robLogoSpr->setPosition(logoSpr->getPosition() + Vec2(0, 80));
	this->addChild(robLogoSpr);

	auto splash = this->getSplash();
	auto splashText = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), splash);
	splashText->setPosition(winSize.width / 2, (winSize.height / 2) - 100);
	splashText->setScale(0.7f);

	this->addChild(splashText);
	_pBar = SimpleProgressBar::create();
	_pBar->setPercentage(0.f);
	_pBar->setPosition({ winSize.width / 2, splashText->getPosition().height + 40 });
	this->addChild(_pBar);
	
	this->runAction(Sequence::create(DelayTime::create(0), CallFunc::create([this]() { this->loadAssets(); }), nullptr));
	
#if SHOW_IMGUI == true
	CocosExplorer::openForever();
#endif
	

	GameToolbox::log("quality medium: {}, scale factor {}", GameManager::getInstance()->isMedium(), dir->getContentScaleFactor());
	
	return true;
}



void LoadingLayer::loadAssets() {
	
	for(auto image : pngs) {
		GameToolbox::log("image {}", image);
		_textureCache->addImageAsync(GameToolbox::getTextureString(image), AX_CALLBACK_1(LoadingLayer::assetLoaded, this));
	}
	
	for(auto plist : plists) {
		GameToolbox::log("plist {}", plist);
		_sprFrameCache->addSpriteFramesWithFile(GameToolbox::getTextureString(plist));
		this->assetLoaded(nullptr);
	}
	
	for(auto fnt : fonts) {
		GameToolbox::log("font {}", fnt);
		Label::createWithBMFont(GameToolbox::getTextureString(fnt), "someText");
		this->assetLoaded(nullptr);
	}
}

void LoadingLayer::assetLoaded(ax::Ref*)
{
	
	this->m_nAssetsLoaded++;
	GameToolbox::log("loading asset {} out of {}", (int)m_nAssetsLoaded, (int)m_nTotalAssets);
	_pBar->setPercentage((m_nAssetsLoaded / m_nTotalAssets)*100.f);

	if(m_nAssetsLoaded == m_nTotalAssets) {
		Director::getInstance()->replaceScene(MenuLayer::scene());
	}
}