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

#include "LevelEditorLayer.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "AudioEngine.h"
#include "CreatorLayer.h"
#include "external/constants.h"
#include "CocosExplorer.h"
#include "GameToolbox/getTextureString.h"
#include "GroundLayer.h"
#include "GameToolbox/math.h"
#include "UILayer.h"
#include "SimpleProgressBar.h"

#include "fmt/format.h"

#include "ButtonSprite.h"
#include "MenuItemSpriteExtra.h"

#include "2d/Menu.h"

ax::Scene* LevelEditorLayer::scene(GJGameLevel* level) {
	auto scene = ax::Scene::create();
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

void LevelEditorLayer::updateCamera(float dt) {
	if (_inPlaybackMode) {
		PlayLayer::updateCamera(dt);

		this->_bottomGround->setPositionX(cameraFollow->getPositionX());

		_button_playback->setPositionX(cameraFollow->getPositionX() + 30);
		_button_playback->setPositionY(cameraFollow->getPositionY() + 30);

		return;
	}

	ax::Vec2 cam2 = m_camDelta;

	cam2.x *= dt;
	cam2.y *= dt;
	
	m_obCamPos += cam2;

	auto winSize = ax::Director::getInstance()->getWinSize();
	ax::Vec2 cam = m_obCamPos;

	cam.y = ax::clampf(cam.y, 0.0f, 1140.f - winSize.height);

	this->m_pBG->setPositionX(this->m_pBG->getPositionX() + (cam.x - m_obCamPos.x));

	if (!this->m_bMoveCameraX)
		m_obCamPos.x = cam.x;

	// if camera reset then do not lerp
	if (!this->m_bMoveCameraY && cam.x != 0)
	{
		m_obCamPos.y = GameToolbox::iLerp(m_obCamPos.y, cam.y, 0.1f, dt / 60.f);
	}
	else
	{
		m_obCamPos.y = cam.y;
	}

	ax::Camera::getDefaultCamera()->setPosition(this->m_obCamPos + winSize / 2);

	cameraFollow->setPosition(m_obCamPos);

	m_pHudLayer->setPosition(this->m_obCamPos);

	cam2.x *= 0.5;
	cam2.x *= 0.5;

	cam2.x = -cam2.x;

	this->m_pBG->setPositionX(this->m_pBG->getPositionX() - (cam2.x) * _bottomGround->getSpeed() * 0.1175f);

	_bottomGround->update(-cam2.x * 0.7);
	_ceiling->update(-cam2.x * 0.7);

	if (this->m_pBG->getPosition().x <= cam.x - 1024.f)
		this->m_pBG->setPositionX(this->m_pBG->getPositionX() + 1024.f);

	if (this->m_pBG->getPosition().x >= cam.x + 1024.f)
		this->m_pBG->setPositionX(this->m_pBG->getPositionX() - 1024.f);

	this->m_pBG->setPositionX(this->m_pBG->getPositionX() + (cam.x - m_obCamPos.x));

	this->m_pBG->setPositionY(m_obCamPos.y * 0.2f);

	this->_bottomGround->setPositionX(cameraFollow->getPositionX());

	_button_playback->setPositionX(cameraFollow->getPositionX() + 30);
	_button_playback->setPositionY(cameraFollow->getPositionY() + 30);
}

void LevelEditorLayer::showCompleteText() {
	return;
}

void LevelEditorLayer::onEnter() {
	Layer::onEnter();

	auto listener = ax::EventListenerKeyboard::create();
	auto dir = ax::Director::getInstance();

	listener->onKeyPressed = AX_CALLBACK_2(LevelEditorLayer::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(LevelEditorLayer::onKeyReleased, this);
	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
};

void LevelEditorLayer::destroyPlayer(PlayerObject* player) {
	printf("attempted to destroy player!\n");

	if (_inPlaybackMode) PlayLayer::destroyPlayer(player);
};

void LevelEditorLayer::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event)
{
	float speed = 7.f;

	switch (keyCode)
	{
	case ax::EventKeyboard::KeyCode::KEY_A: {
		m_camDelta.x = -speed;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_D: {
		m_camDelta.x = speed;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_W: {
		m_camDelta.y = speed;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_S: {
		m_camDelta.y = -speed;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_ESCAPE: {
		exit();
	}
	break;
	default:
		break;
	}
}

void LevelEditorLayer::onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event)
{
	switch (keyCode)
	{
	case ax::EventKeyboard::KeyCode::KEY_A: {
		m_camDelta.x = 0;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_D: {
		m_camDelta.x = 0;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_W: {
		m_camDelta.y = 0;
	}
	break;
	case ax::EventKeyboard::KeyCode::KEY_S: {
		m_camDelta.y = 0;
	}
	break;
	default:
		break;
	}
}

void LevelEditorLayer::update(float delta) {
	PlayLayer::update(delta);

	auto size = ax::Director::getInstance()->getWinSize();

	auto cam = m_obCamPos;

	cam.x += (size.x) / 2.f;
	cam.y = _bottomGround->getPositionY();

	if (!_inPlaybackMode) {
		_player1->setPosition(cam);
		_player2->setPosition(cam);
	}

	m_pBar->setPercentage(m_obCamPos.x / this->m_lastObjXPos * 100.f);
	float val = m_pBar->getPercentage();
	m_pPercentage->setString(ax::StringUtils::format("%.02f%%", val > 100 ? 100 : val < 0 ? 0 : val));

	if (this->_colorChannels.contains(1000))
	{
		this->m_pBG->setColor(this->_colorChannels.at(1000)._color);
	}
}

void LevelEditorLayer::resetLevel() {
	PlayLayer::resetLevel();

	if (!_inPlaybackMode) ax::AudioEngine::stopAll();
}

bool LevelEditorLayer::init(GJGameLevel* level) {
	if (!PlayLayer::init(level))
		return false;

	// DrawGridLayer::create();
	// LevelTools::getInstance()->getAudioString();
	// DrawGridLayer::getInstance()->loadTimeMarkers();

	auto grid = ax::DrawNode::create();
	addChild(grid, 10000000);

	int max_block_x = (m_lastObjXPos / 30.f) + 10;
	int max_block_y = 18;

	if (max_block_x <= 29) max_block_x = 256;

	int i = 0;

	auto calculate_pos = [&](int x, int y) {
		ax::Vec2 pos;

		pos.x = x * 30;
		pos.y = y * 30;

		pos.y += 90;

		return pos;
	};

	while (i < max_block_x) {
		int j = 0;

		while (j < max_block_y) {
			ax::Vec2 pos = calculate_pos(i, j);
			ax::Vec2 pos2 = pos;

			pos2.x += 30;
			pos2.y += 30;

			grid->drawRect(pos, pos2, ax::Color4F::BLACK);

			j++;
		}

		i++;
	}

	grid->setOpacity(128);

	_bottomGround->removeFromParentAndCleanup(false);

	addChild(_bottomGround, 101);

	m_pHudLayer->setLocalZOrder(grid->getLocalZOrder() + 2);
	_bottomGround->setLocalZOrder(grid->getLocalZOrder() + 1);
	_ceiling->setLocalZOrder(grid->getLocalZOrder() + 1);

	_player1->noclip = true;
	_player2->noclip = true;

	auto dir = ax::Director::getInstance();
	auto listener = ax::EventListenerTouchOneByOne::create();

	listener->setEnabled(true);
	listener->setSwallowTouches(true);

	// trigger when you start touch
	listener->onTouchBegan = AX_CALLBACK_2(LevelEditorLayer::onTouchBegan, this);
	listener->onTouchEnded = AX_CALLBACK_2(LevelEditorLayer::onTouchEnded, this);

	dir->getEventDispatcher()->removeEventListener(m_pHudLayer->_listener);

	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto menu = ax::Menu::create();

	auto buttonSprite = ButtonSprite::create("Playback", 0x32, 0, 0.6, false, GameToolbox::getTextureString("bigFont.fnt"), GameToolbox::getTextureString("GJ_button_01.png"), 30);
	MenuItemSpriteExtra* button = MenuItemSpriteExtra::create(buttonSprite, [this](Node* btn)
	{
		this->_inPlaybackMode = !this->_inPlaybackMode;
		this->m_obCamPos.x = 0;
		this->m_obCamPos.x = 0;
		resetLevel();
		//ax::Director::getInstance()->pushScene((ax::TransitionFade::create(0.5f, LevelInfoLayer::scene(level))));
	});
	button->setPosition(30, 30);

	menu->addChild(button);

	this->addChild(menu, grid->getLocalZOrder() + 3);

	_button_playback = menu;

	return true;
}

void LevelEditorLayer::addObject(GameObject* obj) {
	int section = sectionForPos(obj->getPositionX());

	obj->setDontTransform(false);

	printf("section: %d/%d\n", section, _sectionObjects.size());

	obj->_uniqueID = _pObjects.size();

	_allObjects.push_back(obj);
	_pObjects.push_back(obj);

	if ((_sectionObjects.size()) < (section + 1)) {
		int i = 0;
		int req_size = (section - _sectionObjects.size()) + 1;
		while (i < req_size) {
			_sectionObjects.push_back({});

			i++;
		}

		_sectionObjects.push_back({});
	}

	printf("new section: %d/%d\n", section, _sectionObjects.size());

	_sectionObjects[section].push_back(obj);
}

bool LevelEditorLayer::onTouchBegan(ax::Touch* touch, ax::Event* event) {
	
	if (!_inPlaybackMode) {
		auto pos = touch->getLocation();

		pos += m_obCamPos;

		pos.y -= 90;

		int mapped_section = pos.x /= 30;
		int mapped_y = pos.y /= 30;

		pos.x = mapped_section * 30 + 15;
		pos.y = mapped_y * 30 + 15;
		
		printf("touch! %f %f\n", pos.x, pos.y);

		// 1,1,2,15,3,15,21,3,24,7
		std::string object_string = fmt::format("1,1,2,{},3,{}", pos.x, pos.y);

		GameObject *obj = GameObject::createFromString(object_string);
		
		obj->setStretchEnabled(false);
		obj->setActive(true);

		addObject(obj);
	} else {
		_player1->pushButton();
		if (_isDualMode) _player2->pushButton();
	}

	return true;
}
void LevelEditorLayer::onTouchEnded(ax::Touch* touch, ax::Event* event) {
	if (_inPlaybackMode) {
		_player1->releaseButton();
		if (_isDualMode) _player2->releaseButton();
	}
}