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

#include "GameObject.h"
#include "2d/ParticleSystemQuad.h"
#include "BaseGameLayer.h"
#include "EffectGameObject.h"
#include "GameToolbox/conv.h"
#include "GameToolbox/log.h"
#include "PlayLayer.h"
#include "PlayerObject.h"
#include "platform/FileUtils.h"
#include <fmt/format.h>
#include <fstream>

USING_NS_AX;

ax::Color3B _tempColor;

bool GameObject::init(std::string_view frame, std::string_view glowFrame)
{
	if (frame.find("player") != std::string::npos)
		return true;

	if (frame.empty())
		return false;

	if (!Sprite::initWithSpriteFrameName(fmt::format("{}.png", frame)))
		return false;

	_pOuterBounds = Rect();
	_pInnerBounds = Rect();

	_glowSprite = nullptr;

	if (!glowFrame.empty())
	{
		_glowSprite = Sprite::createWithSpriteFrameName(fmt::format("{}.png", glowFrame));
		if (_glowSprite)
		{
			_hasGlow = true;
			_glowSprite->setBlendFunc(GameToolbox::getBlending());
			_glowSprite->setStretchEnabled(false);
			_glowSprite->setLocalZOrder(-1);
			_glowSprite->retain();
		}
	}
	_texturePath = getTexture()->getPath();

	setCascadeColorEnabled(false);
	setCascadeOpacityEnabled(false);

	return true;
}

void GameObject::addCustomSprites(nlohmann::json j, ax::Sprite* parent)
{
	for (nlohmann::json jsonObj : j)
	{
		ax::Sprite* s = ax::Sprite::createWithSpriteFrameName(jsonObj["texture_name"].get<std::string>());
		if (!s)
			continue;
		parent->addChild(s);
		s->setStretchEnabled(false);
		s->setAnchorPoint({static_cast<float>(jsonObj["anchor_x"]), static_cast<float>(jsonObj["anchor_y"])});
		s->setFlippedX(static_cast<bool>(jsonObj["flip_x"]));
		s->setFlippedY(static_cast<bool>(jsonObj["flip_y"]));
		s->setPosition({static_cast<float>(jsonObj["x"]), static_cast<float>(jsonObj["y"])});
		s->setLocalZOrder(static_cast<int>(jsonObj["z"]));
		s->setRotation(static_cast<float>(jsonObj["rot"]));
		s->setScaleX(static_cast<float>(jsonObj["scale_x"]));
		s->setScaleY(static_cast<float>(jsonObj["scale_y"]));
		if (jsonObj.contains("content_x"))
			s->setContentSize({static_cast<float>(jsonObj["content_x"]), static_cast<float>(jsonObj["content_y"])});
		s->setCascadeColorEnabled(false);
		s->setCascadeOpacityEnabled(false);
		if (jsonObj.contains("color_channel"))
		{
			if (jsonObj["color_channel"] == "base")
				_childSpritesChannel.push_back(0);
			else if (jsonObj["color_channel"] == "detail")
				_childSpritesChannel.push_back(1);
			else if (jsonObj["color_channel"] == "black")
				_childSpritesChannel.push_back(2);
		}
		else
			_childSpritesChannel.push_back(404);

		_childSprites.push_back(s);

		if (jsonObj.contains("children"))
			addCustomSprites(jsonObj["children"], s);
	}
}

void GameObject::customSetup()
{
	static nlohmann::json childJson;

	if (childJson.empty())
	{
		childJson = nlohmann::json::parse(FileUtils::getInstance()->getStringFromFile("Custom/object.json"));
	}

	auto objJson = childJson[std::to_string(getID())];

	if (objJson.contains("default_z_order"))
		setGlobalZOrder((int)objJson["default_z_order"]);
	if (objJson.contains("default_z_layer"))
		_zLayer = (int)objJson["default_z_layer"];
	if (objJson.contains("default_primary_channel"))
		_mainColorChannel = (int)objJson["default_primary_channel"];
	if (objJson.contains("default_secondary_channel"))
		_secColorChannel = (int)objJson["default_secondary_channel"];

	if (objJson.contains("color_channel"))
	{
		if (objJson["color_channel"] == "base")
			_childSpritesChannel.push_back(0);
		else if (objJson["color_channel"] == "detail")
			_childSpritesChannel.push_back(1);
		else if (objJson["color_channel"] == "black")
			_childSpritesChannel.push_back(2);
	}
	else
		_childSpritesChannel.push_back(404);

	if (objJson.contains("object_type"))
		setGameObjectType((GameObjectType)objJson["object_type"]);

	if (objJson.contains("children"))
		addCustomSprites(objJson["children"], this);

	for (auto obj : _childSprites)
		obj->setAdditionalTransform(&_parentMatrix);

	_isOnlyDetail = true;

	for (size_t type : _childSpritesChannel)
	{
		if (type == 0)
			_isOnlyDetail = false;
	}

	_primaryInvisible = false;

	// robtop made 3d parts have lines by default but then decided to make them invisible instead of removing them for
	// whatever reason
	if (getID() >= 515 && getID() <= 640)
	{
		_primaryInvisible = true;
		setOpacity(0);
	}

	switch (getID())
	{
	case 10:
		createAndAddParticle("portalEffect01.plist", 3);
		break;
	case 11:
		createAndAddParticle("portalEffect02.plist", 3);
		break;
	case 12:
		createAndAddParticle("portalEffect03.plist", 3);
		break;
	case 13:
		createAndAddParticle("portalEffect04.plist", 3);
		break;
	case 35: // yellow pad
		createAndAddParticle("bumpEffect.plist", 0);
		_particle->setPositionY(getPositionY() - 4.f);
		break;
	case 67: // blue pad
		createAndAddParticle("bumpEffect.plist", 0);
		_particle->setStartColor({0, 255, 255, 255});
		_particle->setPositionY(getPositionY() - 4.f);
		break;
	case 140: // pink pad
		createAndAddParticle("bumpEffect.plist", 0);
		_particle->setStartColor({255, 0, 255, 255});
		_particle->setPositionY(getPositionY() - 4.f);
		break;
	case 1332: // red pad
		createAndAddParticle("bumpEffect.plist", 0);
		_particle->setStartColor({255, 0, 0, 255});
		_particle->setPositionY(getPositionY() - 4.f);
		break;
	case 36: // yellow orb
		createAndAddParticle("ringEffect.plist", 3);
		_particle->setStartColor({255, 255, 0, 255});
		break;
	case 84: // blue orb
		createAndAddParticle("ringEffect.plist", 3);
		_particle->setStartColor({0, 255, 255, 255});
		break;
	case 141: // pink orb
		createAndAddParticle("ringEffect.plist", 3);
		_particle->setStartColor({255, 0, 255, 255});
		break;
	case 1022: // green orb
		createAndAddParticle("ringEffect.plist", 3);
		_particle->setStartColor({0, 255, 0, 255});
		break;
	case 1333: // red orb
		createAndAddParticle("ringEffect.plist", 3);
		_particle->setStartColor({255, 0, 0, 255});
		break;
	case 366:
	case 367:
	case 1717:
	case 1718:
	case 1723:
	case 1724:
		_primaryInvisible = true;
		break;
	}
}

void GameObject::createAndAddParticle(const char* path, int zOrder)
{
	if (_particle)
	{
		AX_SAFE_RELEASE(_particle);
		_particle->cleanup();
		_particle = nullptr;
	}

	_hasParticle = true;

	_particle = ParticleSystemQuad::create(path);
	_particle->setGlobalZOrder(zOrder);
	_particle->retain();
	_particle->stopSystem();

	_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
	_particle->setRotation(getRotation());
	_particle->start();
}

GameObject* GameObject::createObject(std::string_view frame, std::string_view glowFrame)
{
	if (frame.find("ring_01_001") != std::string::npos)
		return GameObject::create(frame, glowFrame);
	else
		return nullptr;
}

GameObject* GameObject::create(std::string_view frame, std::string_view glowFrame)
{
	auto pRet = new (std::nothrow) GameObject();

	if (pRet && pRet->init(frame, glowFrame))
	{
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void GameObject::setPosition(const ax::Vec2& pos)
{
	Sprite::setPosition(pos);
	_parentMatrix = getNodeToParentTransform();
	if (_hasGlow)
		_glowSprite->setPosition(pos);
	if (_hasParticle)
		_particle->setPosition(pos);
}
void GameObject::setRotation(float rotation)
{
	Sprite::setRotation(rotation);
	_parentMatrix = getNodeToParentTransform();
	if (_hasGlow)
		_glowSprite->setRotation(rotation);
	if (_hasParticle)
		_particle->setRotation(rotation);
}
void GameObject::setScaleX(float scalex)
{
	Sprite::setScaleX(scalex);
	_parentMatrix = getNodeToParentTransform();
	if (_hasGlow)
		_glowSprite->setScaleX(scalex);
	if (_hasParticle)
		_particle->setRotation(scalex * (isFlippedX() ? -1.f : 1.f));
}
void GameObject::setScaleY(float scaley)
{
	Sprite::setScaleY(scaley);
	_parentMatrix = getNodeToParentTransform();
	if (_hasGlow)
		_glowSprite->setScaleY(scaley);
	if (_hasParticle)
		_particle->setScaleY(scaley * (isFlippedX() ? -1.f : 1.f));
}
void GameObject::setOpacity(uint8_t opacity)
{
	if (_primaryInvisible) opacity = 0;
	Sprite::setOpacity(opacity);
	if (_hasGlow)
		_glowSprite->setOpacity(opacity);
	if (_hasParticle)
		_particle->setOpacity(opacity);
}

Color3B GameObject::getChannelColor(SpriteColor *colorChannel)
{
	auto bgl = BaseGameLayer::getInstance();

	Color3B returnCol;

	if (colorChannel->_copyingColorID != -1)
		returnCol = getChannelColor(&bgl->_colorChannels[colorChannel->_copyingColorID]);
	else 
		returnCol = colorChannel->_color;

	GameToolbox::applyHSV(colorChannel->_hsvModifier, &returnCol);

	return returnCol;
}



void GameObject::applyColorChannel(ax::Sprite* sprite, int channelType, float opacityMultiplier, SpriteColor* col)
{

	float op = col->_opacity * opacityMultiplier * _effectOpacityMultipler;

	switch (channelType)
	{
	case 0:
		if(getOpacity() != op)
			sprite->setOpacity(op);
		if ((sprite->getBlendFunc() == GameToolbox::getBlending()) != col->_blending)
			removeFromGameLayer();
		break;
	case 1:
		if (getOpacity() != op)
			sprite->setOpacity(op);
		if ((sprite->getBlendFunc() == GameToolbox::getBlending()) != col->_blending)
			removeFromGameLayer();
		break;
	default:
		if (getOpacity() != op)
			sprite->setOpacity(op);
		break;
	}

	if (col->_blending)
		sprite->setBlendFunc(GameToolbox::getBlending());
	else
		sprite->setBlendFunc(BlendFunc::DISABLE);

	Color3B finalColor = getChannelColor(col);

	_tempColor = finalColor;
}

void GameObject::update()
{
	if (getEnterEffectID() == 0)
	{
		// setPosition(_startPosition);
		setScaleX(_startScale.x);
		setScaleY(_startScale.y);
	}

	auto bgl = BaseGameLayer::getInstance();
	if (!bgl)
		return;

	if (!_isTrigger)
		this->setPosition(this->_startPosition + this->_startPosOffset);

	float opacityMultiplier = 1.f;

	ax::Color3B groupColor;
	GroupProperties::GroupState state = GroupProperties::GroupState::NOT_CHANGING;

	for (int i : _groups | std::views::reverse)
	{
		opacityMultiplier *= bgl->_groups[i]._alpha;
		if (bgl->_groups[i].groupState != GroupProperties::GroupState::NOT_CHANGING)
		{
			groupColor = bgl->_groups[i]._color;
			state = bgl->_groups[i].groupState;
		}
	}

	if(!_mainColor) _mainColor = bgl->_colorChannels.contains(_mainColorChannel) ? &bgl->_colorChannels[_mainColorChannel] : nullptr;
	if(!_secColor) _secColor = bgl->_colorChannels.contains(_secColorChannel) ? &bgl->_colorChannels[_secColorChannel] : nullptr;

	switch (_childSpritesChannel[0])
	{
	case 0:

		if (state == GroupProperties::GroupState::MAIN_ONLY || state == GroupProperties::GroupState::MAIN_DETAIL)
			_tempColor = groupColor;
		else
			applyColorChannel(this, 0, opacityMultiplier, _mainColor);

		if (_mainHSVEnabled)
			GameToolbox::applyHSV(_mainHSV, &_tempColor);

		if (getColor() != _tempColor)
			setColor(_tempColor);
		break;
	case 1:

		if (state == GroupProperties::GroupState::DETAIL_ONLY || state == GroupProperties::GroupState::MAIN_DETAIL)
			_tempColor = groupColor;
		else if (_secColor)
			applyColorChannel(this, 1, opacityMultiplier, _secColor);
		else
			applyColorChannel(this, 1, opacityMultiplier, _mainColor);

		if (_secondaryHSVEnabled)
			GameToolbox::applyHSV(_secondaryHSV, &_tempColor);
		else if (_isOnlyDetail)
			GameToolbox::applyHSV(_mainHSV, &_tempColor);

		if (getColor() != _tempColor)
			setColor(_tempColor);
		break;
	case 2:
		applyColorChannel(this, 2, opacityMultiplier, &bgl->_colorChannels[1010]);
		if (getColor() != _tempColor)
			setColor(_tempColor);
		break;
	}

	for (size_t i = 0; i < _childSprites.size(); i++)
	{
		switch (_childSpritesChannel[i + 1])
		{
		case 0:

			if (state == GroupProperties::GroupState::MAIN_ONLY || state == GroupProperties::GroupState::MAIN_DETAIL)
				_tempColor = groupColor;
			else
				applyColorChannel(_childSprites[i], 0, opacityMultiplier, _mainColor);

			if (_mainHSVEnabled)
				GameToolbox::applyHSV(_mainHSV, &_tempColor);
			if (_childSprites[i]->getColor() != _tempColor)
				_childSprites[i]->setColor(_tempColor);
			break;
		case 1:

			if (state == GroupProperties::GroupState::DETAIL_ONLY || state == GroupProperties::GroupState::MAIN_DETAIL)
				_tempColor = groupColor;
			else if (_secColor)
				applyColorChannel(_childSprites[i], 1, opacityMultiplier, _secColor);
			else
				applyColorChannel(_childSprites[i], 1, opacityMultiplier, _mainColor);

			if (_secondaryHSVEnabled)
				GameToolbox::applyHSV(_secondaryHSV, &_tempColor);
			else if (_isOnlyDetail)
				GameToolbox::applyHSV(_mainHSV, &_tempColor);
			if (_childSprites[i]->getColor() != _tempColor)
				_childSprites[i]->setColor(_tempColor);
			break;
		case 2:
			applyColorChannel(_childSprites[i], 2, opacityMultiplier, &bgl->_colorChannels[1010]);
			if (_childSprites[i]->getColor() != _tempColor)
				_childSprites[i]->setColor(_tempColor);
			break;
		}
	}
}

GameObject* GameObject::createFromString(std::string_view data)
{
	// data = 1,2,3,4,5,6,7 where [key,value,key,value]
	auto properties = GameToolbox::splitByDelimStringView(data, ',');

	GameObject* obj = nullptr;

	// index 1 is object id
	// GameToolbox::log("loading: {}", data);
	int objectID = GameToolbox::stoi(properties[1]);

	if (!GameObject::_pBlocks.contains(objectID))
		objectID = 1;

	std::string_view frame = GameObject::_pBlocks.at(objectID);

	// actually create the object
	if (objectID != 1 && std::find(GameObject::_pTriggers.begin(), GameObject::_pTriggers.end(), objectID) !=
							 GameObject::_pTriggers.end())
	{
		// mylock.lock();
		obj = EffectGameObject::create(frame);
		// mylock.unlock();
		obj->_isTrigger = true;
	}
	else
	{
		// mylock.lock();
		obj = GameObject::create(frame, GameObject::getGlowFrame(objectID));
		// mylock.unlock();
	}

	if (!obj)
	{
		return nullptr;
	}

	AX_SAFE_RETAIN(obj);

	obj->setStretchEnabled(false);
	obj->setActive(true);
	obj->setID(objectID);
	obj->customSetup();

	auto bgl = BaseGameLayer::getInstance();
	// TODO: set uniqueID in base layer

	// iterate over every key
	for (size_t i = 0; i < properties.size() - 1; i += 2)
	{
		int key = GameToolbox::stoi(properties[i]);
		switch (key)
		{
		case 2:
			obj->setPositionX(GameToolbox::stof(properties[i + 1]));
			break;
		case 3:
			obj->setPositionY(GameToolbox::stof(properties[i + 1]) + 90.0f);
			break;
		case 4:
			obj->setScaleX(-1.f * GameToolbox::stof(properties[i + 1]));
			break;
		case 5:
			obj->setScaleY(-1.f * GameToolbox::stof(properties[i + 1]));
			break;
		case 6:
			obj->setRotation(GameToolbox::stof(properties[i + 1]));
			break;
		case 7:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_color.r = GameToolbox::stoi(properties[i + 1]);
			break;
		case 8:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_color.g = GameToolbox::stoi(properties[i + 1]);
			break;
		case 9:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_color.b = GameToolbox::stoi(properties[i + 1]);
			break;
		case 10:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_duration = GameToolbox::stof(properties[i + 1]);
			break;
		case 17:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_blending = GameToolbox::stoi(properties[i + 1]);
		case 20:
			obj->_editorLayer = GameToolbox::stoi(properties[i + 1]);
			break;
		case 21:
			obj->_mainColorChannel = GameToolbox::stoi(properties[i + 1]);
			if (!bgl->_colorChannels.contains(obj->_mainColorChannel))
			{
				bgl->_colorChannels.insert({obj->_mainColorChannel, SpriteColor(Color3B::WHITE, 255, 0)});
				bgl->_originalColors.insert({obj->_mainColorChannel, SpriteColor(Color3B::WHITE, 255, 0)});
			}
			break;
		case 22:
			obj->_secColorChannel = GameToolbox::stoi(properties[i + 1]);
			if (!bgl->_colorChannels.contains(obj->_secColorChannel))
			{
				bgl->_colorChannels.insert({obj->_secColorChannel, SpriteColor(Color3B::WHITE, 255, 0)});
				bgl->_originalColors.insert({obj->_secColorChannel, SpriteColor(Color3B::WHITE, 255, 0)});
			}
			break;
		case 23:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_targetColorId = GameToolbox::stoi(properties[i + 1]);
			break;
		case 24:
			obj->_zLayer = GameToolbox::stoi(properties[i + 1]);
			break;
		case 25:
			obj->setGlobalZOrder(static_cast<float>(GameToolbox::stoi(properties[i + 1])));
			break;
		case 28:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_offset.x = GameToolbox::stof(properties[i + 1]);
			break;
		case 29:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_offset.y = GameToolbox::stof(properties[i + 1]);
			break;
		case 30:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_easing = GameToolbox::stoi(properties[i + 1]);
			break;
		case 85:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_easeRate = GameToolbox::stof(properties[i + 1]);
			break;
		case 32:
			obj->setScaleX(obj->getScaleX() * GameToolbox::stof(properties[i + 1]));
			obj->setScaleY(obj->getScaleY() * GameToolbox::stof(properties[i + 1]));
			break;
		case 35:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_opacity = GameToolbox::stof(properties[i + 1]);
			break;
		case 41:
			obj->_mainHSVEnabled = GameToolbox::stoi(properties[i + 1]);
			break;
		case 42:
			obj->_secondaryHSVEnabled = GameToolbox::stoi(properties[i + 1]);
			break;
		case 43: {
			auto hsv = GameToolbox::splitByDelimStringView(properties[i + 1], 'a');
			obj->_mainHSV.h = GameToolbox::stof(hsv[0]);
			obj->_mainHSV.s = GameToolbox::stof(hsv[1]);
			obj->_mainHSV.v = GameToolbox::stof(hsv[2]);
			obj->_mainHSV.sChecked = GameToolbox::stoi(hsv[3]);
			obj->_mainHSV.vChecked = GameToolbox::stoi(hsv[4]);
		}
		break;
		case 44: {
			auto hsv = GameToolbox::splitByDelimStringView(properties[i + 1], 'a');
			obj->_secondaryHSV.h = GameToolbox::stof(hsv[0]);
			obj->_secondaryHSV.s = GameToolbox::stof(hsv[1]);
			obj->_secondaryHSV.v = GameToolbox::stof(hsv[2]);
			obj->_secondaryHSV.sChecked = GameToolbox::stoi(hsv[3]);
			obj->_secondaryHSV.vChecked = GameToolbox::stoi(hsv[4]);
		}
		break;
		case 45:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_fadeIn = GameToolbox::stof(properties[i + 1]);
			break;
		case 46:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_hold = GameToolbox::stof(properties[i + 1]);
			break;
		case 47:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_fadeOut = GameToolbox::stof(properties[i + 1]);
			break;
		case 48:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_pulseMode = GameToolbox::stoi(properties[i + 1]);
			break;
		case 49: {
			if (obj->_isTrigger)
			{
				auto hsv = GameToolbox::splitByDelimStringView(properties[i + 1], 'a');
				auto trigger = dynamic_cast<EffectGameObject*>(obj);
				trigger->_hsv.h = GameToolbox::stof(hsv[0]);
				trigger->_hsv.s = GameToolbox::stof(hsv[1]);
				trigger->_hsv.v = GameToolbox::stof(hsv[2]);
				trigger->_hsv.sChecked = GameToolbox::stoi(hsv[3]);
				trigger->_hsv.vChecked = GameToolbox::stoi(hsv[4]);
			}
			break;
		}

		case 50:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_copiedColorId = GameToolbox::stoi(properties[i + 1]);
			break;
		case 52:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_pulseType = GameToolbox::stoi(properties[i + 1]);
			break;
		case 51:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_targetGroupId = GameToolbox::stoi(properties[i + 1]);
			break;
		case 56:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_activateGroup = GameToolbox::stoi(properties[i + 1]);
			break;
		case 57: {
			auto groups = GameToolbox::splitByDelimStringView(properties[i + 1], '.');
			// pre-allocate
			obj->_groups.reserve(groups.size());
			for (std::string_view groupStr : groups)
			{
				int group = GameToolbox::stoi(groupStr);
				// TODO add groups in derived class
				BaseGameLayer::getInstance()->_groups[group]._objects.push_back(obj);
				obj->_groups.push_back(group);
			}
			break;
		}
		case 62:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_spawnTriggered = GameToolbox::stoi(properties[i + 1]);
			break;
		case 63:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_spawnDelay = GameToolbox::stof(properties[i + 1]);
			break;
		case 65:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_mainOnly = GameToolbox::stoi(properties[i + 1]);
			break;
		case 66:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_detailOnly = GameToolbox::stoi(properties[i + 1]);
			break;
		case 67: // dont enter
		case 64: // dont exit
			obj->setDontTransform(true);
			break;
		case 87:
			if (obj->_isTrigger)
				dynamic_cast<EffectGameObject*>(obj)->_multiTriggered = GameToolbox::stoi(properties[i + 1]);
			break;
		} // switch end
	}	  // for end

	if (obj)
	{
		Hitbox hb = {0, 0, 0, 0};

		if (GameObject::_pHitboxes.contains(objectID))
			hb = GameObject::_pHitboxes.at(objectID);
		if (GameObject::_pHitboxRadius.contains(objectID))
			obj->_radius = GameObject::_pHitboxRadius.at(objectID);

		ax::Mat4 tr;
		ax::Rect rec = {hb.x, hb.y, hb.w, hb.h};
		switch (obj->getGameObjectType())
		{
		default:

			tr.rotate(obj->getRotationQuat());

			tr.scale(obj->getScaleX() * (obj->isFlippedX() ? -1.f : 1.f),
					 obj->getScaleY() * (obj->isFlippedY() ? -1.f : 1.f), 1);

			rec = RectApplyTransform(rec, tr);

			obj->setOuterBounds(Rect(obj->getPosition() + Vec2(rec.origin.x, rec.origin.y) + Vec2(15, 15),
									 {rec.size.width, rec.size.height}));
			break;
		case kGameObjectTypeDecoration:
		case kGameObjectTypeSpecial:
			break;
		}
		obj->setStartPosition(obj->getPosition());
		obj->setStartScaleX(obj->getScaleX());
		obj->setStartScaleY(obj->getScaleY());
		return obj;
	}

	return nullptr;
}

void GameObject::removeFromGameLayer()
{
	setActive(false);

	for (auto sprite : _childSprites)
	{
		if (sprite->getBlendFunc() != getBlendFunc())
		{
			AX_SAFE_RETAIN(sprite);
			sprite->removeFromParentAndCleanup(true);
			/*addChild(sprite);
			AX_SAFE_RELEASE(sprite);*/
		}
	}

	AX_SAFE_RETAIN(this);
	if (_particle)
	{
		AX_SAFE_RETAIN(_particle);
		_particle->removeFromParentAndCleanup(true);
	}
	if (_glowSprite)
	{
		AX_SAFE_RETAIN(_glowSprite);
		_glowSprite->removeFromParentAndCleanup(true);
	}

	removeFromParentAndCleanup(true);
}

void GameObject::updateTweenAction(float value, std::string_view key)
{
}

std::string GameObject::keyToFrame(int key)
{
	return key > 0 && key < 8 ? fmt::format("square_{}_001.png", key) : "";
}

// couldn't understand jack shit from the original
// so I recreated from 2.1's GameToolbox::stringSetupToMap
std::map<std::string, std::string> GameObject::stringSetupToDict(std::string str)
{
	size_t index = 0;
	size_t length = str.length();

	std::map<std::string, std::string> output;

	size_t currentComma = str.find(',');
	while (true)
	{
		auto key = str.substr(index, currentComma - index);

		if (currentComma == str.npos)
			break;

		// find new comma
		index = currentComma + 1;
		currentComma = str.find(',', index);

		// set values
		output.insert(std::make_pair(key, str.substr(index, currentComma - index)));

		if (currentComma == str.npos)
			break;

		// find new comma (again)
		index = currentComma + 1;
		currentComma = str.find(',', index);
	}

	return output;
}
void GameObject::triggerActivated(PlayerObject* player)
{
	auto bgl = BaseGameLayer::getInstance();
	player == bgl->_player1 ? _hasBeenActivatedP1 = true : _hasBeenActivatedP2 = true;
}

bool GameObject::hasBeenActiavedByPlayer(PlayerObject* player)
{
	auto bgl = BaseGameLayer::getInstance();
	return player == bgl->_player1 ? _hasBeenActivatedP1 : _hasBeenActivatedP2;
}

ax::Rect GameObject::getOuterBounds(float a, float b)
{
	ax::Rect r = getOuterBounds();
	r.origin.x += r.size.width / 2;
	r.origin.y += r.size.height / 2;
	r.size.width *= a;
	r.size.height *= b;
	r.origin.x -= r.size.width / 2;
	r.origin.y -= r.size.height / 2;
	return r;
}

std::string_view GameObject::getGlowFrame(int objectID)
{
	return "";

	// switch(objectID)
	// {
	// [[likely]] default: return "";
	// case 44: return "checkpoint_01_glow_001";
	// [[likely]] case 1: return "square_01_glow_001";
	// [[likely]] case 2: return "square_02_glow_001";
	// [[likely]] case 3: return "square_03_glow_001";
	// [[likely]] case 4: return "square_04_glow_001";
	// [[likely]] case 6: return "square_06_glow_001";
	// [[likely]] case 7: return "square_07_glow_001";
	// [[likely]] case 8: return "spike_01_glow_001";
	// case 35: return "bump_01_glow_001";
	// case 39: return "spike_02_glow_001";
	// case 40: return "plank_01_glow_001";
	// [[unlikely]] case 1903: return "plank_01_glow_001";
	// }
}