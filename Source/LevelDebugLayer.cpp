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

#include <core/platform/PlatformConfig.h>
#include "LevelDebugLayer.h"
#include "2d/Transition.h"
#include "CocosExplorer.h"
#include "EffectGameObject.h"
#include "GJGameLevel.h"
#include "GameToolbox/conv.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/log.h"
#include "GameToolbox/network.h"
#include "GameToolbox/nodes.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "LevelSearchLayer.h"
#include "LevelSelectLayer.h"
#include "MenuItemSpriteExtra.h"
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
#include "ShlObj_core.h"
#else
#define HRESULT unsigned int
#endif
#include "format.h"
#include <AudioEngine.h>
#include <Macros.h>

USING_NS_AX;
USING_NS_AX_EXT;
using namespace ax::network;

int audioId;

LevelDebugLayer* LevelDebugLayer::create(GJGameLevel* level)
{
	LevelDebugLayer* ret = new LevelDebugLayer();
	if (ret->init(level))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}
}

ax::Scene* LevelDebugLayer::scene(GJGameLevel* level)
{
	auto scene = Scene::create();
	scene->addChild(LevelDebugLayer::create(level));
	return scene;
}

float offset;
int songid;

HRESULT GetFolderLocation(int csidl, char* buffer)
{
	#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
	LPITEMIDLIST pidl = 0;
	HRESULT result = SHGetSpecialFolderLocation(NULL, csidl, &pidl);
	*buffer = 0;

	if (result == 0)
	{
		SHGetPathFromIDListA(pidl, buffer);
		CoTaskMemFree(pidl);
	}

	return result;

	#endif

	return 0;
}

void LevelDebugLayer::playMusic(float dt)
{
	#if (TARGET_PLATFORM == AX_PLATFORM_WIN32)
	char appdata[256];
	GetFolderLocation(CSIDL_LOCAL_APPDATA, appdata);
	auto path = fmt::format("{}/GeometryDash/{}.mp3", appdata, songid);
	audioId = AudioEngine::play2d(path, true, 0.2f);
	scheduleOnce([&](float dt) { AudioEngine::setCurrentTime(audioId, offset); }, 1, "time");
	#endif
}

bool LevelDebugLayer::init(GJGameLevel* level)
{
	if (!BaseGameLayer::init(level))
		return false;

	auto dir = Director::getInstance();
	auto winSize = dir->getWinSize();

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [this](Node const* btn) { exit(); });
	auto backMenu = Menu::create(backBtn, nullptr);
	backMenu->setPosition({25.0f, winSize.height - 22.0f});
	addChild(backMenu);

	_BG = Sprite::create(GameToolbox::getTextureString(fmt::format("game_bg_{:02}_001.png", _levelSettings._bgID)));
	if (!_BG)
	{
		_BG = Sprite::create(GameToolbox::getTextureString(fmt::format("game_bg_{:02}_001.png", 1)));
	}
	_BG->setStretchEnabled(false);
	const Texture2D::TexParams texParams = {backend::SamplerFilter::LINEAR, backend::SamplerFilter::LINEAR,
											backend::SamplerAddressMode::REPEAT, backend::SamplerAddressMode::REPEAT};
	_BG->getTexture()->setTexParameters(texParams);
	_BG->setTextureRect(Rect(0, 0, 1024 * 5, 1024));
	_BG->setPosition(winSize.x / 2, winSize.y / 4);
	addChild(_BG, -100);

	if (_colorChannels.contains(1000))
		_BG->setColor(_colorChannels.at(1000)._color);

	offset = this->_levelSettings.songOffset;
	songid = level->_songID;

	scheduleOnce(AX_SCHEDULE_SELECTOR(LevelDebugLayer::playMusic), 0);
	scheduleUpdate();

	return true;
}

void LevelDebugLayer::onEnter()
{
	Layer::onEnter();
	auto listener = EventListenerKeyboard::create();
	auto dir = Director::getInstance();
	listener->onKeyPressed = AX_CALLBACK_2(LevelDebugLayer::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(LevelDebugLayer::onKeyReleased, this);

	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	ImGuiPresenter::getInstance()->addRenderLoop("#playlayer", AX_CALLBACK_0(LevelDebugLayer::onDrawImgui, this),
												 dir->getRunningScene());
}

void LevelDebugLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	ImGuiPresenter::getInstance()->removeRenderLoop("#playlayer");
	this->unscheduleUpdate();

	Layer::onExit();
}

void LevelDebugLayer::onDrawImgui()
{
	if (_showDebugMenu)
		return;

	static int monitorN = 0;
	static bool fullscreen;
	static float gameSpeed, fps;
	ImGui::SetNextWindowPos({1000.0f, 200.0f}, ImGuiCond_FirstUseEver);

	ImGui::Begin("LeveDebugLayer Debug");

#ifdef AX_PLATFORM_PC
	if (ImGui::Checkbox("Fullscreen", &fullscreen))
	{
		int a;
		auto monitor = glfwGetMonitors(&a)[monitorN];
		auto mode = glfwGetVideoMode(monitor);

		if (fullscreen)
			glfwSetWindowMonitor(static_cast<GLViewImpl*>(ax::Director::getInstance()->getOpenGLView())->getWindow(),
								 monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else
		{
			glfwSetWindowMonitor(static_cast<GLViewImpl*>(ax::Director::getInstance()->getOpenGLView())->getWindow(),
								 NULL, 0, 0, 1280, 720, 0);
			glfwWindowHint(GLFW_DECORATED, true);
		}
	}
#endif

	ImGui::SameLine();

	if (ImGui::ArrowButton("full", ImGuiDir_Right))
		ImGui::OpenPopup("Fullscreen Settings");

	if (ImGui::BeginPopupModal("Fullscreen Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputInt("Monitor", &monitorN);
		if (ImGui::Button("Close"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	if (ImGui::Button("Exit"))
	{
		this->exit();
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
				ImGui::GetIO().Framerate);

	if (ImGui::InputFloat("Speed", &gameSpeed))
		Director::getInstance()->getScheduler()->setTimeScale(gameSpeed);

	if (ImGui::InputFloat("FPS", &fps))
		Director::getInstance()->setAnimationInterval(1.0f / fps);

	ImGui::InputFloat("Camera Speed", &_camSpeed);

	static int channelID = -1;

	ImGui::InputInt("Color Channel", &channelID);

	if (_colorChannels.contains(channelID))
	{
		auto channel = _colorChannels[channelID];
		int color[3] = {channel._color.r, channel._color.g, channel._color.b};
		ImGui::InputInt3("Color", color);
		ImGui::Checkbox("Blending", &channel._blending);
		ImGui::InputFloat("Opacity", &channel._opacity);
		ImGui::InputInt("Copied Color ID", &channel._copyingColorID);
	}

	static int groupID = -1;

	ImGui::InputInt("Group", &groupID);

	if (_colorChannels.contains(groupID))
	{
		auto group = _groups[groupID];
		ImGui::InputFloat("Alpha", &group._alpha);
		int size = group._objects.size();
		ImGui::InputInt("Obj Count", &size);
	}

	ImGui::End();
}

void LevelDebugLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A: {
		if (_camInput.x > -1.f)
			_camInput.x -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_D: {
		if (_camInput.x < 1.f)
			_camInput.x += 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_W: {
		if (_camInput.y < 1.f)
			_camInput.y += 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_S: {
		if (_camInput.y > -1.f)
			_camInput.y -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_ESCAPE: {
		exit();
	}
	case EventKeyboard::KeyCode::KEY_F: {
		_showDebugMenu = !_showDebugMenu;
		if (_showDebugMenu)
			CocosExplorer::close();
		else
			CocosExplorer::open();
	}
	break;
	default:
		break;
	}
}

void LevelDebugLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A: {
		if (_camInput.x < 1.f)
			_camInput.x += 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_D: {
		if (_camInput.x > -1.f)
			_camInput.x -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_W: {
		if (_camInput.y > -1.f)
			_camInput.y -= 1.f;
	}
	break;
	case EventKeyboard::KeyCode::KEY_S: {
		if (_camInput.y < 1.f)
			_camInput.y += 1.f;
	}
	break;
	default:
		break;
	}
}

void LevelDebugLayer::update(float delta)
{
	auto dir = Director::getInstance();
	auto winSize = dir->getWinSize();
	auto cam = Camera::getDefaultCamera();
	cam->setPosition(cam->getPosition() + (_camInput * _camSpeed * delta));
	_BG->setPosition({cam->getPosition().x, cam->getPosition().y - winSize.y / 2});

	_colorChannels[1007]._color = getLightBG(_colorChannels[1000]._color, Color3B::MAGENTA);

	const double delta60 = delta * 60.0;
	const double delta240 = delta60 * 4.0;
	const float newStepCount = delta240 < 0.0 ? ceil(delta240 - 0.5) : floor(delta240 + 0.5);
	int subStepCount = 4;
	if (newStepCount >= 4.0)
	{
		subStepCount = static_cast<int>(newStepCount);
	}

	const float stepDelta60 = delta60 / (float)subStepCount;
	const double stepDelta = delta / (float)subStepCount;

	if (this->_colorChannels.contains(1000))
	{
		this->_BG->setColor(this->_colorChannels.at(1000)._color);
	}

	for (int curStep = 0; curStep < subStepCount; curStep++)
	{
		const bool isLastSubStep = curStep == (subStepCount - 1);
		_effectManager->prepareMoveActions(stepDelta, !isLastSubStep);
		this->processMoveActionsStep(stepDelta);
	}

	updateVisibility();
	updateTriggers(delta);
}

void LevelDebugLayer::updateTriggers(float dt)
{
	int current_section = sectionForPos(Camera::getDefaultCamera()->getPositionX());

	for (int i = current_section - 3; i < current_section + 1; i++)
	{
		if (i < _sectionObjects.size() && i >= 0)
		{
			std::vector<GameObject*> section = _sectionObjects[i];

			for (int j = 0; j < section.size(); j++)
			{
				GameObject* obj = section[j];
				if (obj->isActive() && obj->_isTrigger)
				{
					auto trigger = dynamic_cast<EffectGameObject*>(obj);
					if (!trigger->_spawnTriggered &&
						trigger->getPositionX() <= Camera::getDefaultCamera()->getPositionX())
					{
						trigger->triggerActivated(dt);
					}
				}
			}
		}
	}
}

void LevelDebugLayer::reorderLayering(GameObject* parentObj, ax::Sprite* child)
{
	ax::Sprite* obj;

	if (child == nullptr)
		obj = parentObj;
	else
	{
		obj = child;
		if (child->getParent() != nullptr)
		{
			AX_SAFE_RETAIN(child);
			child->removeFromParentAndCleanup(true);
		}
		/*if (child->getBlendFunc() == parentObj->getBlendFunc())
		{
			GameToolbox::log("opi[jfeawsjpiosfzdihjopsdzipojds");
			parentObj->addChild(child);
			AX_SAFE_RELEASE(obj);
			return;
		}*/
	}

	if (obj->getBlendFunc() == GameToolbox::getBlending())
	{
		switch (parentObj->_zLayer)
		{
		case -3:
			_blendingBatchNodeB4->addChild(obj);
			break;
		case -1:
			_blendingBatchNodeB3->addChild(obj);
			break;
		case 1:
			_blendingBatchNodeB2->addChild(obj);
			break;
		case 3:
			_blendingBatchNodeB1->addChild(obj);
			break;
		default:
		case 5:
			_blendingBatchNodeT1->addChild(obj);
			break;
		case 7:
			_blendingBatchNodeT2->addChild(obj);
			break;
		case 9:
			_blendingBatchNodeT3->addChild(obj);
			break;
		}
	}
	else
	{
		if (parentObj->_texturePath == _mainBatchNodeTexture)
		{
			switch (parentObj->_zLayer)
			{
			case -3:
				_mainBatchNodeB4->addChild(obj);
				break;
			case -1:
				_mainBatchNodeB3->addChild(obj);
				break;
			case 1:
				_mainBatchNodeB2->addChild(obj);
				break;
			case 3:
				_mainBatchNodeB1->addChild(obj);
				break;
			default:
			case 5:
				_mainBatchNodeT1->addChild(obj);
				break;
			case 7:
				_mainBatchNodeT2->addChild(obj);
				break;
			case 9:
				_mainBatchNodeT3->addChild(obj);
				break;
			}
		}
		else if (parentObj->_texturePath == _main2BatchNodeTexture)
			_main2BatchNode->addChild(obj);

		AX_SAFE_RELEASE(obj);
	}
}

void LevelDebugLayer::updateVisibility()
{
	auto winSize = ax::Director::getInstance()->getWinSize();
	auto camPos = Camera::getDefaultCamera()->getPosition();

	float unk = 70.0f;

	int prevSection = floorf((camPos.x - (winSize.width / 2)) / 100) - 1;
	int nextSection = ceilf((camPos.x) / 100) + 2.0f;

	for (int i = prevSection; i < nextSection; i++)
	{
		if (i >= 0)
		{
			if (i < _sectionObjects.size())
			{
				auto section = _sectionObjects[i];
				for (size_t j = 0; j < section.size(); j++)
				{
					GameObject* obj = section[j];
					if (!obj)
						continue;

					if (obj->getParent() == nullptr && obj->_toggledOn)
					{
						if (obj->_particle)
						{
							addChild(obj->_particle);
							AX_SAFE_RELEASE(obj->_particle);
						}
						if (obj->_glowSprite)
						{
							_glowBatchNode->addChild(obj->_glowSprite);
							AX_SAFE_RELEASE(obj->_glowSprite);
						}

						reorderLayering(obj, nullptr);

						for (ax::Sprite* child : obj->_childSprites)
						{
							reorderLayering(obj, child);
						}
					}

					obj->setActive(true);
					obj->setVisible(true);
					obj->update();
				}
			}
		}
	}

	if (_prevSection - 1 >= 0 && _sectionObjects.size() != 0 && _prevSection <= _sectionObjects.size())
	{
		auto section = _sectionObjects[_prevSection - 1];
		for (size_t j = 0; j < section.size(); j++)
		{
			if (section[j]->getParent() != nullptr)
			{
				if (section[j]->_isTrigger)
				{
					EffectGameObject* trigger = static_cast<EffectGameObject*>(section[j]);
					if (trigger && trigger->_spawnTriggered && trigger->_wasTriggerActivated &&
						trigger->_multiTriggered)
					{
						trigger->setVisible(false);
						continue;
					}
				}
				section[j]->removeFromGameLayer();
			}
		}
	}

	if (_nextSection >= 0 && _sectionObjects.size() != 0 && _nextSection < _sectionObjects.size())
	{
		auto section = _sectionObjects[_nextSection];
		for (size_t j = 0; j < section.size(); j++)
		{
			if (section[j]->getParent() != nullptr)
			{
				EffectGameObject* trigger = static_cast<EffectGameObject*>(section[j]);
				if (trigger && trigger->_spawnTriggered)
				{
					trigger->setVisible(false);
					continue;
				}
				section[j]->removeFromGameLayer();
			}
		}
	}

	_prevSection = prevSection;
	_nextSection = nextSection;
}

void LevelDebugLayer::exit()
{

	AudioEngine::stopAll();
	AudioEngine::play2d("quitSound_01.ogg", false, 0.1f);
	AudioEngine::play2d("menuLoop.mp3", true, 0.2f);

	unscheduleUpdate();

	int size = _allObjects.size();
	for (int i = 0; i < size; i++)
	{
		GameObject* obj = _allObjects.at(i);
		if (obj && !obj->getParent())
		{
			for (auto sprite : obj->_childSprites)
			{
				if (!sprite->getParent())
				{
					sprite->onExit();
					AX_SAFE_RELEASE_NULL(sprite);
				}
			}
			if (obj->_particle)
			{
				obj->_particle->onExit();
				AX_SAFE_RELEASE_NULL(obj->_particle);
			}
			if (obj->_glowSprite)
			{
				obj->_glowSprite->onExit();
				AX_SAFE_RELEASE_NULL(obj->_glowSprite);
			}
			obj->unscheduleAllCallbacks();
			obj->onExit();
			obj->setActive(false);
			AX_SAFE_RELEASE(obj);
		}
	}

	int id = _level->_levelID;
	if (id <= 0 || id > 22)
		return GameToolbox::popSceneWithTransition(0.5f);

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelSelectLayer::scene(id - 1)));
}