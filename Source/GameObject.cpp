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
#include "BaseGameLayer.h"
#include "EffectGameObject.h"
#include "PlayLayer.h"
#include "PlayerObject.h"
#include "external/json.hpp"
#include <fmt/format.h>
#include <fstream>
#include "2d/CCParticleSystemQuad.h"
#include "platform/CCFileUtils.h"
#include "GameToolbox/conv.h"

USING_NS_AX;

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

	return true;
}

void GameObject::customSetup()
{
	static nlohmann::json childJson;

	if (childJson.empty())
	{
		childJson = nlohmann::json::parse(FileUtils::getInstance()->getStringFromFile("Custom/object.json"));
	}

	if (childJson[std::to_string(getID())].contains("default_z_order"))
		setGlobalZOrder((int)childJson[std::to_string(getID())]["default_z_order"]);
	if (childJson[std::to_string(getID())].contains("default_z_layer"))
		_zLayer = (int)childJson[std::to_string(getID())]["default_z_layer"];
	if (childJson[std::to_string(getID())].contains("default_primary_channel"))
		_mainColorChannel = (int)childJson[std::to_string(getID())]["default_primary_channel"];
	if (childJson[std::to_string(getID())].contains("default_secondary_channel"))
		_secColorChannel = (int)childJson[std::to_string(getID())]["default_secondary_channel"];

	if (childJson[std::to_string(getID())].contains("object_type"))
		setGameObjectType((GameObjectType)childJson[std::to_string(getID())]["object_type"]);

	for (nlohmann::json jsonObj : childJson[std::to_string(getID())]["childrens"])
	{
		ax::Sprite* s = ax::Sprite::createWithSpriteFrameName(static_cast<std::string>(jsonObj["texture_name"]));
		if (!s)
			continue;
		s->setStretchEnabled(false);
		s->setAnchorPoint({static_cast<float>(jsonObj["anchor_x"]), static_cast<float>(jsonObj["anchor_y"])});
		s->setFlippedX(static_cast<bool>(jsonObj["flip_x"]));
		s->setFlippedY(static_cast<bool>(jsonObj["flip_y"]));
		s->setPosition({static_cast<float>(jsonObj["x"]), static_cast<float>(jsonObj["y"])});
		s->setLocalZOrder(static_cast<int>(jsonObj["z"]));
		s->setRotation(static_cast<float>(jsonObj["rot"]));
		s->setScaleX(static_cast<float>(jsonObj["scale_x"]));
		s->setScaleY(static_cast<float>(jsonObj["scale_y"]));
		s->setContentSize({static_cast<float>(jsonObj["content_x"]), static_cast<float>(jsonObj["content_y"])});
		_detailSprites.push_back(s);
		addChild(s);
	}

	if (childJson[std::to_string(getID())].contains("black"))
	{
		_forceBlack = true;
		setColor(ax::Color3B::BLACK);
		for (auto sp : _detailSprites)
		{
			sp->setColor(ax::Color3B::BLACK);
		}
	}
	else if (childJson[std::to_string(getID())].contains("black_detail"))
	{
		_forceBlackDetail = true;
		for (auto sp : _detailSprites)
		{
			sp->setColor(ax::Color3B::BLACK);
		}
	}

	_primaryInvisible = false;

	// robtop made 3d parts have lines by default but then decided to make them invisible instead of removing them for
	// whatever reason
	if (getID() >= 515 && getID() <= 640)
		_primaryInvisible = true;

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
	if(_hasGlow)
		_glowSprite->setPosition(pos);
	if(_hasParticle)
		_particle->setPosition(pos);
}
void GameObject::setRotation(float rotation)
{
	Sprite::setRotation(rotation);
	if(_hasGlow)
		_glowSprite->setRotation(rotation);
	if(_hasParticle)
		_particle->setRotation(rotation);
}
void GameObject::setScaleX(float scalex)
{
	Sprite::setScaleX(scalex);
	if(_hasGlow)
		_glowSprite->setScaleX(scalex);
	if(_hasParticle)
		_particle->setRotation(scalex * (isFlippedX() ? -1.f : 1.f));
}
void GameObject::setScaleY(float scaley)
{
	Sprite::setScaleY(scaley);
	if(_hasGlow)
		_glowSprite->setScaleY(scaley);
	if(_hasParticle)
		_particle->setScaleY(scaley * (isFlippedX() ? -1.f : 1.f));
}
void GameObject::setOpacity(uint8_t opacity)
{
	Sprite::setOpacity(opacity);
	if(_hasGlow)
		_glowSprite->setOpacity(opacity);
	if(_hasParticle)
		_particle->setOpacity(opacity);
}

void GameObject::update()
{
	if (getEnterEffectID() == 0)
	{
		//setPosition(_startPosition);
		setScaleX(_startScale.x);
		setScaleY(_startScale.y);
	}

	auto bgl = BaseGameLayer::getInstance();
	if (!bgl)
		return;

	this->setPosition(this->_startPosition + this->_startPosOffset);

	float opacityMultiplier = 1.f;

	for (int i : _groups)
	{
		opacityMultiplier *= bgl->_groups[i]._alpha;
	}

	if (bgl->_colorChannels.contains(_mainColorChannel))
	{
		auto sp1 = bgl->_colorChannels[_mainColorChannel];

		if (sp1._blending)
			setBlendFunc(GameToolbox::getBlending());
		else
			setBlendFunc(BlendFunc::DISABLE);

		if (_hadBlending != sp1._blending)
			removeFromGameLayer();

		_hadBlending = sp1._blending;

		if (sp1._copyingColorID != -1)
			sp1 = bgl->_colorChannels[sp1._copyingColorID];

		if (!_forceBlack && getColor() != sp1._color)
			setColor(sp1._color);

		float op = sp1._opacity * opacityMultiplier * _effectOpacityMultipler;

		if (getOpacity() != op)
			setOpacity(_primaryInvisible ? 0 : op);

		if (!_forceBlackDetail)
		{
			if (bgl->_colorChannels.contains(_secColorChannel))
			{
				auto sp2 = bgl->_colorChannels[_secColorChannel];

				if (_hadBlending2 != sp2._blending)
					removeFromGameLayer();
				_hadBlending = sp1._blending;
				if (sp2._copyingColorID != -1)
					sp2 = bgl->_colorChannels[sp2._copyingColorID];
				for (auto sp : _detailSprites)
				{
					if (sp2._blending)
						sp->setBlendFunc(GameToolbox::getBlending());
					else
						sp->setBlendFunc(BlendFunc::DISABLE);
					if (!_forceBlack && sp->getColor() != sp2._color)
						sp->setColor(sp2._color);
					op = sp2._opacity * opacityMultiplier * _effectOpacityMultipler;
					if (sp->getOpacity() != op)
						sp->setOpacity(op);
				}
			}
			else
			{
				for (auto sp : _detailSprites)
				{
					if (sp1._blending)
						sp->setBlendFunc(GameToolbox::getBlending());
					else
						sp->setBlendFunc(BlendFunc::DISABLE);
					if (!_forceBlack && sp->getColor() != sp1._color)
						sp->setColor(sp1._color);
					float op = sp1._opacity * opacityMultiplier * _effectOpacityMultipler;
					if (sp->getOpacity() != op)
						sp->setOpacity(op);
				}
			}
		}
	}
	else if (bgl->_colorChannels.contains(_secColorChannel))
	{
		auto sp1 = bgl->_colorChannels[_mainColorChannel];

		if (sp1._blending)
			setBlendFunc(GameToolbox::getBlending());
		else
			setBlendFunc(BlendFunc::DISABLE);

		if (sp1._copyingColorID != -1)
			sp1 = bgl->_colorChannels[sp1._copyingColorID];
		auto sp2 = bgl->_colorChannels[_secColorChannel];
		if (sp2._copyingColorID != -1)
			sp2 = bgl->_colorChannels[sp2._copyingColorID];
		if (_hadBlending != sp1._blending)
			removeFromGameLayer();
		if (_hadBlending2 != sp2._blending)
			removeFromGameLayer();
		if (!_forceBlack && getColor() != sp1._color)
			setColor(sp1._color);
		float op = sp1._opacity * opacityMultiplier * _effectOpacityMultipler;
		if (getOpacity() != op)
			setOpacity(_primaryInvisible ? 0 : op);

		if (_forceBlackDetail)
			return;
		for (auto sp : _detailSprites)
		{
			if (sp2._blending)
				sp->setBlendFunc(GameToolbox::getBlending());
			else
				sp->setBlendFunc(BlendFunc::DISABLE);
			if (sp->getColor() != sp2._color)
				sp->setColor(sp2._color);
			op = sp2._opacity * opacityMultiplier * _effectOpacityMultipler;
			if (sp->getOpacity() != op)
				sp->setOpacity(op);
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
	if (objectID != 1 && std::find(GameObject::_pTriggers.begin(), GameObject::_pTriggers.end(), objectID) != GameObject::_pTriggers.end())
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
		case 21:
			obj->_mainColorChannel = GameToolbox::stoi(properties[i + 1]);
			break;
		case 22:
			obj->_secColorChannel = GameToolbox::stoi(properties[i + 1]);
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
				// TODO: fix this
				trigger->_saturationTicked = GameToolbox::stoi(hsv[3]);
				trigger->_brightnessTicked = GameToolbox::stoi(hsv[4]);
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
		case 67: // dont enter
		case 64: // dont exit
			obj->setDontTransform(true);
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
	//_mainBatchNode->removeChild(section[j], true);
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