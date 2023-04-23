#include "BaseGameLayer.h"
#include "EffectGameObject.h"
#include "GameToolbox.h"
#include "external/json.hpp"

USING_NS_AX;

// #define USE_MULTITHREADING
// std::mutex mylock;

BaseGameLayer* BaseGameLayer::_instance = nullptr;

BaseGameLayer* BaseGameLayer::create(GJGameLevel* level)
{
	BaseGameLayer* ret = new BaseGameLayer();
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

bool BaseGameLayer::init(GJGameLevel* level)
{
	if (!Layer::init())
		return false;

	_instance = this;
	_level = level;

	initBatchNodes();
	loadLevel();

	return true;
}

GameObject* GameObject_createFromString(std::string_view data)
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

	// actually create the object, use std::ranges
	if (objectID != 1 && std::ranges::find(GameObject::_pTriggers, objectID) != GameObject::_pTriggers.end())
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
				// trigger->_saturationTicked = GameToolbox::stof(hsv[3]);
				// trigger->_brightnessTicked = GameToolbox::stof(hsv[4]);
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
		case 57: {
			auto groups = GameToolbox::splitByDelimStringView(data, '.');
			// pre-allocate
			obj->_groups.reserve(groups.size());
			for (std::string_view groupStr : groups)
			{
				int group = GameToolbox::stoi(groupStr);
				// TODO add groups in derived class
				//_groups[g]._objects.push_back(obj);
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
void BaseGameLayer::loadLevel()
{
	// TODO: find a modern gzip decompress library or write own gzip decompress

	std::string levelStr = _level->_levelString;
	if (levelStr.empty())
	{
		nlohmann::json file = nlohmann::json::parse(GameToolbox::getFileContentsResources("Custom/mainLevels.json"));
		levelStr = fmt::format("H4sIAAAAAAAAA{}", file[std::to_string(_level->_levelID)]);
	}
	levelStr = GJGameLevel::decompressLvlStr(levelStr);
	{
		auto s = BenchmarkTimer("load level");
		setupLevel(levelStr);
		createObjectsFromSetup(levelStr);
	}

	if (_allObjects.size() != 0)
	{
		_lastObjXPos = 570.0f;

		for (GameObject* object : _allObjects)
		{
			if (_lastObjXPos < object->getPositionX())
				_lastObjXPos = object->getPositionX();
		}

		GameToolbox::log("last x: {}", _lastObjXPos);

		for (size_t i = 0; i < sectionForPos(_lastObjXPos); i++)
		{
			std::vector<GameObject*> vec;
			_sectionObjects.push_back(vec);
		}

		for (GameObject* object : _allObjects)
		{
			int section = sectionForPos(object->getPositionX());
			_sectionObjects[section - 1 < 0 ? 0 : section - 1].push_back(object);

			if (_colorChannels.contains(object->_mainColorChannel) &&
				_colorChannels[object->_mainColorChannel]._blending)
			{
				object->setBlendFunc(GameToolbox::getBlending());
			}

			if (_colorChannels.contains(object->_secColorChannel) && _colorChannels[object->_secColorChannel]._blending)
			{
				for (auto s : object->_detailSprites)
					s->setBlendFunc(GameToolbox::getBlending());
			}
			object->setCascadeOpacityEnabled(false);
			object->update();
		}
	}
}

void BaseGameLayer::initBatchNodes()
{
	_blendingBatchNodeB4 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB4, -23);
	_blendingBatchNodeB4->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeB4 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB4, -22);

	_blendingBatchNodeB3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB3, -16);
	_blendingBatchNodeB3->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeB3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB3, -15);

	_blendingBatchNodeB2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB2, -9);
	_blendingBatchNodeB2->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeB2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB2, -8);

	_blendingBatchNodeB1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB1, -2);
	_blendingBatchNodeB1->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeB1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB1, -1);

	_blendingBatchNodeT1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeT1, 2);
	_blendingBatchNodeT1->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeT1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeT1, 3);

	_blendingBatchNodeT2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeT2, 9);
	_blendingBatchNodeT2->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeT2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeT2, 10);

	_blendingBatchNodeT3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeT3, 24);
	_blendingBatchNodeT3->setBlendFunc(GameToolbox::getBlending());

	_mainBatchNodeT3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeT3, 25);

	_glowBatchNode = ax::SpriteBatchNode::create(GameToolbox::getTextureString("GJ_GameSheetGlow.png"), 150);
	this->addChild(_glowBatchNode);
	_glowBatchNode->setBlendFunc(GameToolbox::getBlending());

	_main2BatchNode = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_main2BatchNodeTexture), 150);
	this->addChild(_main2BatchNode);

	//_particleBatchNode = ax::ParticleBatchNode::create("square.png", 30);
	// addChild(_particleBatchNode);

	_mainBatchNodeTexture = _mainBatchNodeT3->getTexture()->getPath();
	_main2BatchNodeTexture = _main2BatchNode->getTexture()->getPath();
}

void BaseGameLayer::createObjectsFromSetup(std::string_view uncompressedLevelString)
{
	// level string = leveldata;objects;something else
	// split the entire thing
	std::vector<std::string_view> objData = GameToolbox::splitByDelimStringView(uncompressedLevelString, ';');
	// loadLevelData(objData[0]);

	// pre-allocate
	_allObjects.reserve(objData.size());

	// erase the first element (level data), we are left with all the objects
	objData.erase(objData.begin());

	// erase whatever the last thing is
	objData.pop_back();
	GameToolbox::log("creating & pushing");

#ifdef USE_MULTITHREADING
	auto loadObjectsInRange = [this](std::span<std::string_view> span) {
		for (auto& objectDataSpecific : span)
		{
			GameObject* obj = GameObject_createFromString(objectDataSpecific);
			if (obj)
			{
				obj->_uniqueID = static_cast<int>(_allObjects.size());
				_allObjects.push_back(obj);
			}
		}
	};

	auto mid = objData.begin() + objData.size() / 2;
	std::thread firstPart(loadObjectsInRange, std::span<std::string_view>(objData.begin(), mid));
	std::thread secondPart(loadObjectsInRange, std::span<std::string_view>(mid, objData.end()));

	firstPart.join();
	secondPart.join();
#else
	for (auto objectDataSpecific : objData)
	{
		GameObject* obj = GameObject_createFromString(objectDataSpecific);
		if (obj)
		{
			obj->_uniqueID = static_cast<int>(_allObjects.size());
			_allObjects.push_back(obj);
		}
	}
#endif

	// add the objects to batch nodes
}

void BaseGameLayer::setupLevel(std::string_view uncompressedLevelString)
{
	std::vector<std::string_view> levelData =
		GameToolbox::splitByDelimStringView(GameToolbox::splitByDelimStringView(uncompressedLevelString, ';')[0], ',');

	for (size_t i = 0; i < levelData.size() - 1; i += 2)
	{
		if (levelData[i] == "kS1")
		{
			_colorChannels.insert({1000, SpriteColor(ax::Color3B(GameToolbox::stof(levelData[i + 1]), 0, 0), 255, 0)});
			GameToolbox::log("わかってた");
		}
		else if (levelData[i] == "kS2")
		{
			_colorChannels.at(1000)._color.g = GameToolbox::stof(levelData[i + 1]);
		}
		else if (levelData[i] == "kS3")
		{
			_colorChannels.at(1000)._color.b = GameToolbox::stof(levelData[i + 1]);
		}
		else if (levelData[i] == "kS4")
		{
			_colorChannels.insert({1001, SpriteColor(ax::Color3B(GameToolbox::stof(levelData[i + 1]), 0, 0), 255, 0)});
		}
		else if (levelData[i] == "kS5")
		{
			_colorChannels.at(1001)._color.g = GameToolbox::stof(levelData[i + 1]);
		}
		else if (levelData[i] == "kS6")
		{
			_colorChannels.at(1001)._color.b = GameToolbox::stof(levelData[i + 1]);
		}
		else if (levelData[i] == "kS29")
		{
			auto colorString = GameToolbox::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1000);
		}
		else if (levelData[i] == "kS30")
		{
			auto colorString = GameToolbox::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1001);
		}
		else if (levelData[i] == "kS31")
		{
			auto colorString = GameToolbox::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1002);
		}
		else if (levelData[i] == "kS32")
		{
			auto colorString = GameToolbox::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1004);
		}
		else if (levelData[i] == "kS37")
		{
			auto colorString = GameToolbox::splitByDelimStringView(levelData[i + 1], '_');
			fillColorChannel(colorString, 1003);
		}
		else if (levelData[i] == "kS38")
		{
			auto colorString = GameToolbox::splitByDelimStringView(levelData[i + 1], '|');
			for (std::string_view colorData : colorString)
			{
				auto innerData = GameToolbox::splitByDelimStringView(colorData, '_');
				int key;
				SpriteColor col;
				col._blending = false;
				for (size_t j = 0; j < innerData.size() - 1; j += 2)
				{
					switch (GameToolbox::stoi(innerData[j]))
					{
					case 1:
						col._color.r = GameToolbox::stof(innerData[j + 1]);
						break;
					case 2:
						col._color.g = GameToolbox::stof(innerData[j + 1]);
						break;
					case 3:
						col._color.b = GameToolbox::stof(innerData[j + 1]);
						break;
					case 5:
						col._blending = GameToolbox::stoi(innerData[j + 1]);
						break;
					case 6:
						key = GameToolbox::stoi(innerData[j + 1]);
						break;
					case 7:
						col._opacity = GameToolbox::stof(innerData[j + 1]) * 255.f;
						break;
					}
				}
				_colorChannels.insert({key, col});
			}
		}
		else if (levelData[i] == "kA6")
		{
			_levelSettings._bgID = GameToolbox::stoi(levelData[i + 1]);
			if (!_levelSettings._bgID)
				_levelSettings._bgID = 1;
		}
		else if (levelData[i] == "kA7")
		{
			_levelSettings._groundID = GameToolbox::stoi(levelData[i + 1]);
			if (!_levelSettings._groundID)
				_levelSettings._groundID = 1;
		}
		else if (levelData[i] == "kA2")
		{
			_levelSettings.gamemode = (PlayerGamemode)GameToolbox::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA3")
		{
			_levelSettings.mini = GameToolbox::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA4")
		{
			_levelSettings.speed = GameToolbox::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA8")
		{
			_levelSettings.dual = GameToolbox::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA10")
		{
			_levelSettings.twoPlayer = GameToolbox::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA11")
		{
			_levelSettings.flipGravity = GameToolbox::stoi(levelData[i + 1]);
		}
		else if (levelData[i] == "kA13")
		{
			_levelSettings.songOffset = GameToolbox::stof(levelData[i + 1]);
		}
	}
}

void BaseGameLayer::fillColorChannel(std::span<std::string_view> colorString, int id)
{
	for (size_t j = 0; j < colorString.size() - 1; j += 2)
	{
		switch (GameToolbox::stoi(colorString[j]))
		{
		case 1:
			_colorChannels.insert({id, SpriteColor(ax::Color3B(GameToolbox::stof(colorString[j + 1]), 0, 0), 255, 0)});
			break;
		case 2:
			_colorChannels.at(id)._color.g = GameToolbox::stof(colorString[j + 1]);
			break;
		case 3:
			_colorChannels.at(id)._color.b = GameToolbox::stof(colorString[j + 1]);
			break;
		}
	}
}

bool BaseGameLayer::isObjectBlending(GameObject* obj)
{
	return _colorChannels.contains(obj->_mainColorChannel) && _colorChannels[obj->_mainColorChannel]._blending &&
			   _colorChannels.contains(obj->_secColorChannel) && _colorChannels[obj->_secColorChannel]._blending ||
		   !_colorChannels.contains(obj->_mainColorChannel) && _colorChannels.contains(obj->_secColorChannel) &&
			   _colorChannels[obj->_secColorChannel]._blending ||
		   !_colorChannels.contains(obj->_secColorChannel) && _colorChannels.contains(obj->_mainColorChannel) &&
			   _colorChannels[obj->_mainColorChannel]._blending;
}

int BaseGameLayer::sectionForPos(float x)
{
	int section = x / 100;
	if (section < 0)
		section = 0;
	return section;
}

void BaseGameLayer::addObject(GameObject* obj)
{
}

void BaseGameLayer::loadLevelData(std::string_view levelDataString)
{
	// std::vector<std::string_view> levelData = GameToolbox::splitByDelimStringView(levelDataString, ',');
}
