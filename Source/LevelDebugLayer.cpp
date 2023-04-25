#include "LevelDebugLayer.h"
#include "GameToolbox.h"
#include "LevelSearchLayer.h"
#include "LevelSelectLayer.h"
#include "MenuItemSpriteExtra.h"
#include "format.h"
#include <AudioEngine.h>
#include <ccMacros.h>
#include "EffectGameObject.h"
#include "ImGui/imgui/imgui.h"
#include "ImGui/ImGuiPresenter.h"
#include "CocosExplorer.h"

USING_NS_AX;
USING_NS_AX_EXT;
using namespace ax::network;

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

	ImGuiPresenter::getInstance()->addRenderLoop("#playlayer", AX_CALLBACK_0(LevelDebugLayer::onDrawImgui, this), dir->getRunningScene());
}

void LevelDebugLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	ImGuiPresenter::getInstance()->removeRenderLoop("#playlayer");
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

	ImGui::End();
}

void LevelDebugLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	GameToolbox::log("Key with keycode {} pressed", static_cast<int>(keyCode));
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
		if(_showDebugMenu)
			CocosExplorer::close();
		else CocosExplorer::open();
	}
	break;
	}
}

void LevelDebugLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	GameToolbox::log("Key with keycode {} released", static_cast<int>(keyCode));
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
	}
}

void LevelDebugLayer::update(float delta)
{
	auto dir = Director::getInstance();
	auto winSize = dir->getWinSize();
	auto cam = Camera::getDefaultCamera();
	cam->setPosition(cam->getPosition() + (_camInput * _camSpeed * delta));
	_BG->setPosition({cam->getPosition().x, cam->getPosition().y - winSize.y / 2});

	if (this->_colorChannels.contains(1000))
	{
		this->_BG->setColor(this->_colorChannels.at(1000)._color);
	}

	updateVisibility();
	updateTriggers(delta);
}

void LevelDebugLayer::updateTriggers(float dt)
{
	int current_section = sectionForPos(Camera::getDefaultCamera()->getPositionX());

	for (int i = current_section - 2; i < current_section + 1; i++)
	{
		if (i < _sectionObjects.size() && i >= 0)
		{
			std::vector<GameObject*> section = _sectionObjects[i];

			for (int j = 0; j < section.size(); j++)
			{
				GameObject* obj = section[j];

				if (obj->isActive())
				{
					if (obj->_isTrigger)
					{
						auto trigger = dynamic_cast<EffectGameObject*>(obj);
						if (!trigger->_wasTriggerActivated && trigger->getPositionX() <= Camera::getDefaultCamera()->getPositionX())
						{
							trigger->triggerActivated(dt);
						}
					}
				}
			}
		}
	}
}

void LevelDebugLayer::updateVisibility()
{
	auto winSize = ax::Director::getInstance()->getWinSize();
	auto camPos = Camera::getDefaultCamera()->getPosition();

	float unk = 70.0f;

	int prevSection = floorf((camPos.x - (winSize.width / 2)) / 100) - 1.0f;
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

					if (obj->getParent() == nullptr)
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

						if (isObjectBlending(obj))
						{
							switch (obj->_zLayer)
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
							if (obj->_texturePath == _mainBatchNodeTexture)
							{
								switch (obj->_zLayer)
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
							else if (obj->_texturePath == _main2BatchNodeTexture)
								_main2BatchNode->addChild(obj);
						}
						AX_SAFE_RELEASE(obj);
					}

					obj->setActive(true);
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
			section[j]->setActive(false);
			if (section[j]->getParent() != nullptr)
			{
				AX_SAFE_RETAIN(section[j]);
				if (section[j]->_particle)
				{
					AX_SAFE_RETAIN(section[j]->_particle);
					removeChild(section[j]->_particle, true);
				}
				if (section[j]->_glowSprite)
				{
					AX_SAFE_RETAIN(section[j]->_glowSprite);
					_glowBatchNode->removeChild(section[j]->_glowSprite, true);
				}
				//_mainBatchNode->removeChild(section[j], true);
				section[j]->removeFromParentAndCleanup(true);
			}
		}
	}

	if (_nextSection >= 0 && _sectionObjects.size() != 0 && _nextSection < _sectionObjects.size())
	{
		auto section = _sectionObjects[_nextSection];
		for (size_t j = 0; j < section.size(); j++)
		{
			section[j]->setActive(false);
			if (section[j]->getParent() != nullptr)
			{
				AX_SAFE_RETAIN(section[j]);
				if (section[j]->_particle)
				{
					AX_SAFE_RETAIN(section[j]->_particle);
					removeChild(section[j]->_particle, true);
				}
				if (section[j]->_glowSprite)
				{
					AX_SAFE_RETAIN(section[j]->_glowSprite);
					_glowBatchNode->removeChild(section[j]->_glowSprite, true);
				}
				//_mainBatchNode->removeChild(section[j], true);
				section[j]->removeFromParentAndCleanup(true);
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

	int id = _level->_levelID;
	if (id <= 0 || id > 22)
		return GameToolbox::popSceneWithTransition(0.5f);

	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, LevelSelectLayer::scene(id - 1)));
}