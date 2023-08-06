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

#pragma once

#include "PlayLayer.h"
#include "2d/Menu.h"

class LevelEditorLayer : public PlayLayer {
private:
	bool init(GJGameLevel* level);

    ax::Vec2 m_camDelta;

    bool _inPlaybackMode = false;
    bool _inSwapMode = false;

    ax::Menu *_button_playback;

    std::map<std::string, GameObject *> _objectPositionCache;

    int _selectedObject = 1;
    GameObject *_selectedObjectReal = nullptr;

    bool _shiftPressed = false;
public:

	static ax::Scene* scene(GJGameLevel* level);
	static LevelEditorLayer* create(GJGameLevel* level);

    void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) override;
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) override;

    void addObject(GameObject* obj) override;

    void resetLevel() override;
    void updateCamera(float dt) override;

    void onEnter() override;
    void update(float delta) override;

    void destroyPlayer(PlayerObject* player) override;

    void showCompleteText() override;

    GameObject *findObject(float x, float y);

    bool onTouchBegan(ax::Touch* touch, ax::Event* event);
	void onTouchEnded(ax::Touch* touch, ax::Event* event);
    void onTouchMoved(ax::Touch* touch, ax::Event* event);
	// static LevelEditorLayer* getInstance();
};
