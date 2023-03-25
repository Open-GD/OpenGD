#include "GameObject.h"
#include "GameToolbox.h"
#include "PlayLayer.h"
#include "PlayerObject.h"
#include "json.hpp"
#include <fmt/format.h>
#include <fstream>

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

	switch (getID())
	{
		case 10:
			createAndAddParticle("portalEffect01.plist", 3);
			_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
			_particle->start();
			//_particle->setEmissionRate(10);
			break;
		case 11:
			createAndAddParticle("portalEffect02.plist", 3);
			_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
			_particle->start();
			//_particle->setEmissionRate(10);
			break;
		case 12:
			createAndAddParticle("portalEffect03.plist", 3);
			_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
			_particle->start();
			//_particle->setEmissionRate(10);
			break;
		case 13:
			createAndAddParticle("portalEffect04.plist", 3);
			_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
			_particle->start();
			//_particle->setEmissionRate(10);
			break;
		case 35:
			createAndAddParticle("bumpEffect.plist", 0);
			_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
			_particle->setPositionY(getPositionY() - 4.f);
			_particle->start();
			break;
		case 36:
			createAndAddParticle("ringEffect.plist", 3);
			_particle->setPositionType(ParticleSystem::PositionType::GROUPED);
			_particle->start();
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

void GameObject::update()
{
	if (_hasGlow)
	{
		auto pos = getPosition();
		
		if (_glowSprite->getPosition() != pos)
			_glowSprite->setPosition(pos);
		
		_glowSprite->setScaleX(getScaleX());
		_glowSprite->setScaleY(getScaleY());
		_glowSprite->setRotation(getRotation());
		_glowSprite->setFlippedX(isFlippedX());
		_glowSprite->setFlippedY(isFlippedY());
		_glowSprite->setLocalZOrder(-1);
		float op = getOpacity();
		
		if (_glowSprite->getOpacity() != op)
			_glowSprite->setOpacity(op);
	}
	if (_hasParticle)
	{
		auto pos = getPosition();
		
		if (_particle->getPosition() != pos)
			_particle->setPosition(pos);
		
		_particle->setRotation(getRotation());
		_particle->setScaleX(getScaleX() * isFlippedX() ? -1.f : 1.f);
		_particle->setScaleY(getScaleY() * isFlippedY() ? -1.f : 1.f);
		float op = getOpacity();
		
		if (_particle->getOpacity() != op)
			_particle->setOpacity(op);
	}

	if (getEnterEffectID() == 0)
	{
		setPosition(_startPosition);
		setScaleX(_startScale.x);
		setScaleY(_startScale.y);
	}

	auto pl = PlayLayer::getInstance();
	if (!pl) return;

	if (pl->m_pColorChannels.contains(_mainColorChannel))
	{
		auto sp1 = pl->m_pColorChannels[_mainColorChannel];
		if (getColor() != sp1._color)
			setColor(sp1._color);
		float op = sp1._opacity;
		if (getOpacity() != op)
			setOpacity(op);
		if (pl->m_pColorChannels.contains(_secColorChannel))
		{
			auto sp2 = pl->m_pColorChannels[_secColorChannel];
			for (auto sp : _detailSprites)
			{
				if (sp->getColor() != sp2._color)
					sp->setColor(sp2._color);
				op = sp2._opacity;
				if (sp->getOpacity() != op)
					sp->setOpacity(op);
			}
		}
		else
		{
			auto sp1 = pl->m_pColorChannels[_mainColorChannel];
			for (auto sp : _detailSprites)
			{
				if (sp->getColor() != sp1._color)
					sp->setColor(sp1._color);
				float op = sp1._opacity;
				if (sp->getOpacity() != op)
					sp->setOpacity(op);
			}
		}
	}
	else if (pl->m_pColorChannels.contains(_secColorChannel))
	{
		auto sp1 = pl->m_pColorChannels[_mainColorChannel];
		auto sp2 = pl->m_pColorChannels[_secColorChannel];
		if (getColor() != sp1._color)
			setColor(sp1._color);
		float op = sp1._opacity;
		if (getOpacity() != op)
			setOpacity(op);
		for (auto sp : _detailSprites)
		{
			if (sp->getColor() != sp2._color)
				sp->setColor(sp2._color);
			op = sp2._opacity;
			if (sp->getOpacity() != op)
				sp->setOpacity(op);
		}
	}
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
	auto pl = PlayLayer::getInstance();
	player == pl->_player1 ? _hasBeenActivatedP1 = true : _hasBeenActivatedP2 = true;
}

bool GameObject::hasBeenActiavedByPlayer(PlayerObject* player)
{
	auto pl = PlayLayer::getInstance();
	return player == pl->_player1 ? _hasBeenActivatedP1 : _hasBeenActivatedP2;
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