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

#include <string_view>
#include <vector>
#include <unordered_map>
#include <map>

#include "2d/Sprite.h"
#include "2d/ActionTween.h"
#include "math/Rect.h"
#include "math/Vec2.h"
#include "SpriteColor.h"
#include "external/json.hpp"
#include "GDHSV.h"
#include "GameToolbox/conv.h"

class PlayerObject;
namespace ax 
{ 
	class ParticleSystemQuad; 
}

enum GameObjectType
{
	kGameObjectTypeSolid = 0,
	kGameObjectTypeHazard = 2,
	kGameObjectTypeInverseGravityPortal = 3,
	kGameObjectTypeNormalGravityPortal = 4,
	kGameObjectTypeShipPortal = 5,
	kGameObjectTypeCubePortal = 6,
	kGameObjectTypeDecoration = 7,
	kGameObjectTypeYellowJumpPad = 8,
	kGameObjectTypePinkJumpPad = 9,
	kGameObjectTypeGravityPad = 10,
	kGameObjectTypeYellowJumpRing = 11,
	kGameObjectTypePinkJumpRing = 12,
	kGameObjectTypeGravityRing = 13,
	kGameObjectTypeInverseMirrorPortal = 14,
	kGameObjectTypeNormalMirrorPortal = 15,
	kGameObjectTypeBallPortal = 16,
	kGameObjectTypeRegularSizePortal = 17,
	kGameObjectTypeMiniSizePortal = 18,
	kGameObjectTypeUfoPortal = 19,
	kGameObjectTypeModifier = 20,
	kGameObjectTypeSecretCoin = 22,
	kGameObjectTypeDualPortal = 23,
	kGameObjectTypeSoloPortal = 24,
	kGameObjectTypeSlope = 25,
	kGameObjectTypeWavePortal = 26,
	kGameObjectTypeRobotPortal = 27,
	kGameObjectTypeTeleportPortal = 28,
	kGameObjectTypeGreenRing = 29,
	kGameObjectTypeCollectible = 30,
	kGameObjectTypeUserCoin = 31,
	kGameObjectTypeDropRing = 32,
	kGameObjectTypeSpiderPortal = 33,
	kGameObjectTypeRedJumpPad = 34,
	kGameObjectTypeRedJumpRing = 35,
	kGameObjectTypeCustomRing = 36,
	kGameObjectTypeDashRing = 37,
	kGameObjectTypeGravityDashRing = 38,
	kGameObjectTypeCollisionObject = 39,
	kGameObjectTypeSpecial = 40,
};

struct Hitbox
{
	float h, w, x, y;
};

class GameObject : public ax::Sprite, public ax::ActionTweenDelegate
{
  private:
	ax::Rect _pOuterBounds;
	ax::Rect _pInnerBounds;

	int m_pId;

	bool m_bActive;

	bool _primaryInvisible; //needed because robtop

	GameObjectType _pObjectType;

	int _pColorRed;
	int _pColorGreen;
	int _pColorBlue;
	float _pDuration;
	virtual void updateTweenAction(float value, std::string_view key) override;

  public:
	std::vector<ax::Sprite*> _childSprites;
	std::vector<int8_t> _childSpritesChannel;
	std::vector<ax::Vec2> _childSpritesScaling;
	std::string _texturePath;

	ax::Vec2 _startPosition, _firstPosition, _startPosOffset;

	float _effectOpacityMultipler = 1.f;

	bool _unkbool;

	std::vector<int> _groups;

	virtual void setPosition(const ax::Vec2& pos) override;
	virtual void setRotation(float rotation) override;
	virtual void setScaleX(float scalex) override;
	virtual void setScaleY(float scaley) override;
	virtual void setOpacity(uint8_t opacity) override;
	

	void setStartPosition(ax::Vec2 pos) { _startPosition = pos; }

	void setStartPositionX(float x) { _startPosition.x = x; }

	void setStartPositionY(float y) { _startPosition.y = y; }

	ax::Vec2 getStartPosition() { return _startPosition; }

	float getStartPositionX() { return _startPosition.x; }

	float getStartPositionY() { return _startPosition.y; }

	ax::Vec2 _startScale = {1.f, 1.f};

	void setStartScale(ax::Vec2 scale) { _startScale = scale; }

	void setStartScaleX(float x) { _startScale.x = x; }

	void setStartScaleY(float y) { _startScale.y = y; }

	ax::Vec2 getStartScale() { return _startScale; }

	float getStartScaleX() { return _startScale.x; }

	float getStartScaleY() { return _startScale.y; }

	ax::Sprite* _glowSprite;

	int _enterEffectID = 0;

	bool _dontTransform = false;

	void setEnterEffectID(int id) { _enterEffectID = id; }

	int getEnterEffectID() { return _enterEffectID; }

	bool _hasBeenActivatedP1, _hasBeenActivatedP2;
	int _mainColorChannel = -1, _secColorChannel = -1;

	SpriteColor* _mainColor, * _secColor;

	bool _hasGlow, _hasParticle;
	bool _isTrigger;

	bool _toggledOn = true;

	bool _isOnlyDetail = false;

	bool _mainHSVEnabled, _secondaryHSVEnabled;
	GDHSV _mainHSV, _secondaryHSV;

	int _zLayer = 0, _editorLayer = -1;

	int _uniqueID = -1;
	int _section = -1;

	float _radius = -1;

	ax::Mat4 _parentMatrix = ax::Mat4::IDENTITY;

	ax::ParticleSystemQuad* _particle;

	static const std::unordered_map<int, Hitbox, my_string_hash> _pHitboxes;
	static const std::unordered_map<int, float, my_string_hash> _pHitboxRadius;
	// from https://gist.github.com/absoIute/c8fa23c9b2cb39252755465345bc6e35
	static const std::unordered_map<int, const char*, my_string_hash> _pBlocks;

	static const std::vector<int> _pSolids;
	static const std::vector<int> _pTriggers;

	static GameObject* create(std::string_view frame, std::string_view glowFrame = "");
	static GameObject* createObject(std::string_view frame, std::string_view glowFrame = "");
	static GameObject* createFromString(std::string_view data);
	bool init(std::string_view frame, std::string_view glowFrame = "");

	void customSetup();
	void addCustomSprites(nlohmann::json j, ax::Sprite* parent);
	void applyColorChannel(ax::Sprite* sprite, int channelType, float opacityMultiplier, SpriteColor *col);
	ax::Color3B getChannelColor(SpriteColor* colorChannel);

	static std::string keyToFrame(int key);
	static std::map<std::string, std::string> stringSetupToDict(std::string);

	float getRadius() { return _radius * getScale(); }

	void setOuterBounds(const ax::Rect& value) { _pOuterBounds = value; }

	ax::Rect getOuterBounds() { return _pOuterBounds; }

	ax::Rect getOuterBounds(float a, float b);

	void setInnerBounds(const ax::Rect& value) { _pInnerBounds = value; }

	ax::Rect getInnerBounds() { return _pInnerBounds; }

	GameObjectType getGameObjectType() { return _pObjectType; }

	void setGameObjectType(GameObjectType value) { _pObjectType = value; }

	int getID() { return m_pId; }

	void setID(int id) { m_pId = id; }

	void setActive(bool active) { m_bActive = active; }

	bool isActive() { return m_bActive; }

	void triggerActivated(PlayerObject* player);
	bool hasBeenActiavedByPlayer(PlayerObject* player);

	void update();

	int getColorRed() { return _pColorRed; }

	void setColorRed(int col) { _pColorRed = col; }

	int getColorGreen() { return _pColorGreen; }

	void setColorGreen(int col) { _pColorGreen = col; }

	int getColorBlue() { return _pColorBlue; }

	void setColorBlue(int col) { _pColorBlue = col; }

	float getDuration() { return _pDuration; }

	void setDuration(float dura) { _pDuration = dura; }

	void createAndAddParticle(const char* particle, int a3);

	bool getDontTransform() { return _dontTransform; }

	void setDontTransform(bool v) { _dontTransform = v; }

	void removeFromGameLayer();
	
	static std::string_view getGlowFrame(int objectID);

	// AX_SYNTESIZE(int, _pColorRed, ColorRed); // does not work
	// AX_SYNTESIZE(int, _pColorGreen, ColorGreen);
	// AX_SYNTESIZE(int, _pColorBlue, ColorBlue);
};