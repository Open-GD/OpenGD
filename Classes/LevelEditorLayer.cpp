#include "LevelEditorLayer.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "AudioEngine.h"
#include "CreatorLayer.h"
#include "constants.h"

ax::Scene* LevelEditorLayer::scene(GJGameLevel* level) {
	auto scene = new ax::Scene();
	scene->addChild(LevelEditorLayer::create(level));
	return scene;
}

LevelEditorLayer* LevelEditorLayer::create(GJGameLevel* level) {
	LevelEditorLayer* pRet = new LevelEditorLayer();
	if (pRet->init(level)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

// LevelEditorLayer* LevelEditorLayer::getInstance() {}

bool LevelEditorLayer::init(GJGameLevel* level) {
	if (!ax::Layer::init())
		return false;

	setLevel(level);

	ax::AudioEngine::stopAll();

	// auto bg = ax::Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
	// bg->setStretchEnabled(false);
	// bg->setPosition(winSize.x / 2, winSize.y / 2);
	// addChild(bg, -100);

	// DrawGridLayer::create();
	// LevelTools::getInstance()->getAudioString();
	// DrawGridLayer::getInstance()->loadTimeMarkers();

	return true;
}

void LevelEditorLayer::exit() {
	ax::AudioEngine::stopAll();
	ax::AudioEngine::play2d("menuLoop.mp3", true, 0.2f);
	ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, CreatorLayer::scene()));
}
