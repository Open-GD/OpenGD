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

#include "BaseGameLayer.h"
#include "EffectGameObject.h"
#include "GJGameLevel.h"
#include "GameToolbox/conv.h"
#include "GameToolbox/log.h"
#include "external/benchmark.h"
#include "external/json.hpp"
#include <fstream>

#include "GameToolbox/getTextureString.h"
#include "platform/FileUtils.h"
#include <2d/SpriteBatchNode.h>

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

	_effectManager = EffectManager::create();
	this->addChild(_effectManager);

	initBatchNodes();
	loadLevel();

	return true;
}

void BaseGameLayer::loadLevel()
{
	// TODO: find a modern gzip decompress library or write own gzip decompress

	std::string levelStr = _level->_levelString;
	if (levelStr.empty())
	{
		nlohmann::json file =
			nlohmann::json::parse(FileUtils::getInstance()->getStringFromFile("Custom/mainLevels.json"));
		std::string levelID = std::to_string(_level->_levelID);

		if (!file.contains(levelID))
			return; // check if our level actually exists in mainlevels list before doing anything

		levelStr = fmt::format("H4sIAAAAAAAAA{}", file.at(levelID).get<std::string>());
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
			object->_section = section - 1 < 0 ? 0 : section - 1;
			_sectionObjects[section - 1 < 0 ? 0 : section - 1].push_back(object);

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
	_blendingBatchNodeB4->setName("_blendingBatchNodeB4");

	_mainBatchNodeB4 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB4, -22);
	_mainBatchNodeB4->setName("_mainBatchNodeB4");

	_blendingBatchNodeB3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB3, -16);
	_blendingBatchNodeB3->setBlendFunc(GameToolbox::getBlending());
	_blendingBatchNodeB3->setName("_blendingBatchNodeB3");

	_mainBatchNodeB3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB3, -15);
	_mainBatchNodeB3->setName("_mainBatchNodeB3");

	_blendingBatchNodeB2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB2, -9);
	_blendingBatchNodeB2->setBlendFunc(GameToolbox::getBlending());
	_blendingBatchNodeB2->setName("_blendingBatchNodeB2");

	_mainBatchNodeB2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB2, -8);
	_mainBatchNodeB2->setName("_mainBatchNodeB2");

	_blendingBatchNodeB1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeB1, -2);
	_blendingBatchNodeB1->setBlendFunc(GameToolbox::getBlending());
	_blendingBatchNodeB1->setName("_blendingBatchNodeB1");

	_mainBatchNodeB1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeB1, -1);
	_mainBatchNodeB1->setName("_mainBatchNodeB1");

	_blendingBatchNodeT1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeT1, 2);
	_blendingBatchNodeT1->setBlendFunc(GameToolbox::getBlending());
	_blendingBatchNodeT1->setName("_blendingBatchNodeT1");

	_mainBatchNodeT1 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeT1, 3);
	_mainBatchNodeT1->setName("_mainBatchNodeT1");

	_blendingBatchNodeT2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeT2, 9);
	_blendingBatchNodeT2->setBlendFunc(GameToolbox::getBlending());
	_blendingBatchNodeT2->setName("_blendingBatchNodeT2");

	_mainBatchNodeT2 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeT2, 10);
	_mainBatchNodeT2->setName("_mainBatchNodeT2");

	_blendingBatchNodeT3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_blendingBatchNodeT3, 24);
	_blendingBatchNodeT3->setBlendFunc(GameToolbox::getBlending());
	_blendingBatchNodeT3->setName("_blendingBatchNodeT3");

	_mainBatchNodeT3 = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_mainBatchNodeTexture), 50);
	this->addChild(_mainBatchNodeT3, 25);
	_mainBatchNodeT3->setName("_mainBatchNodeT3");

	_glowBatchNode = ax::SpriteBatchNode::create(GameToolbox::getTextureString("GJ_GameSheetGlow.png"), 150);
	this->addChild(_glowBatchNode);
	_glowBatchNode->setBlendFunc(GameToolbox::getBlending());
	_glowBatchNode->setName("_glowBatchNode");

	_main2BatchNode = ax::SpriteBatchNode::create(GameToolbox::getTextureString(_main2BatchNodeTexture), 150);
	this->addChild(_main2BatchNode);
	_main2BatchNode->setName("_main2BatchNode");

	//_particleBatchNode = ax::ParticleBatchNode::create("square.png", 30);
	// addChild(_particleBatchNode);

	_mainBatchNodeTexture = _mainBatchNodeT3->getTexture()->getPath();
	_main2BatchNodeTexture = _main2BatchNode->getTexture()->getPath();
}

void BaseGameLayer::createObjectsFromSetup(std::string_view uncompressedLevelString)
{
	//TODO: this function should only recieve vector of game object strings
	
	std::vector<std::string_view> objData = GameToolbox::splitByDelimStringView(uncompressedLevelString, ';');

	_allObjects.reserve(objData.size());

	objData.erase(objData.begin());

	//theres prob a better way to do this but this works for now
	if(const auto& last = objData.back(); last.front() != '1' || last[1] != ',')
	{
		objData.pop_back();
	}
	
	GameToolbox::log("creating & pushing");

	for (const auto& objectDataSpecific : objData)
	{
		GameObject* obj = GameObject::createFromString(objectDataSpecific);
		if (obj)
		{
			obj->_uniqueID = static_cast<int>(_allObjects.size());
			_allObjects.push_back(obj);
		}
	}

	// add the objects to batch nodes
}

ax::Color3B BaseGameLayer::getLightBG(ax::Color3B bg, ax::Color3B p1)
{
	ax::HSV hsv = ax::HSV(bg);
	hsv.s -= 0.07843137255f;

	return GameToolbox::hsvToRgb(hsv); GameToolbox::blendColor(p1, GameToolbox::hsvToRgb(hsv), hsv.v / 100);
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
					case 9:
						col._copyingColorID = GameToolbox::stoi(innerData[j + 1]);
						break;
					case 10:
						auto hsv = GameToolbox::splitByDelimStringView(innerData[j + 1], 'a');
						col._hsvModifier.h = GameToolbox::stof(hsv[0]);
						col._hsvModifier.s = GameToolbox::stof(hsv[1]);
						col._hsvModifier.v = GameToolbox::stof(hsv[2]);
						col._hsvModifier.sChecked = GameToolbox::stoi(hsv[3]);
						col._hsvModifier.vChecked = GameToolbox::stoi(hsv[4]);
						col._applyHsv = true;
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

	// change to get the player color not from player
	_colorChannels[1005]._color = Color3B::WHITE;
	_colorChannels[1005]._blending = true;
	_colorChannels[1006]._color = Color3B::WHITE;
	_colorChannels[1006]._blending = true;
	_colorChannels[1010]._color = Color3B::BLACK;
	_colorChannels[1007]._color = _colorChannels[1000]._color;
	_colorChannels[1007]._blending = true;

	_originalColors = std::unordered_map<int, SpriteColor, my_string_hash>(_colorChannels);
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

void BaseGameLayer::processMoveActions(float dt)
{
	for (auto dictObject : this->_effectManager->_activeMoveActions)
	{
		GroupProperties* currentGroup;
		float x, y;
		if (!this->_groups.contains(dictObject.first))
		{
			GroupProperties gp;
			this->_groups.insert({dictObject.first, gp});
			currentGroup = &gp;
		}
		else
			currentGroup = &this->_groups[dictObject.first];
		x = dictObject.second->_newPosOptimized.x;
		y = dictObject.second->_newPosOptimized.y;

		if (currentGroup && (x != 0 || y != 0))
		{
			for (auto obj : currentGroup->_objects)
			{
				if (!obj->_unkbool)
				{
					obj->_firstPosition.x = obj->_startPosition.x + obj->_startPosOffset.x;
					obj->_firstPosition.y = obj->_startPosition.y + obj->_startPosOffset.y;
					obj->_unkbool = true;
				}

				if (y != 0)
				{
					obj->_startPosOffset.y += y;
				}
				if (x != 0)
				{
					obj->_startPosOffset.x += x;

					int sectionSize = this->_sectionObjects.size();
					auto section = BaseGameLayer::sectionForPos(obj->_startPosition.x + obj->_startPosOffset.x);
					section = section - 1 < 0 ? 0 : section - 1;
					if (obj->_section != section)
					{
						auto vec = &this->_sectionObjects[obj->_section];
						auto newEnd = std::partition(vec->begin(), vec->end(), [&](GameObject* a) { return a != obj; });
						vec->resize(newEnd - vec->begin());
						while (section >= sectionSize)
						{
							std::vector<GameObject*> vec;
							this->_sectionObjects.push_back(vec);
							sectionSize++;
						}
						this->_sectionObjects[section].push_back(obj);
						obj->_section = section;
					}
				}
			}
		}
	}
}

void BaseGameLayer::runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt, int groupID)
{
	this->_effectManager->runMoveCommand(duration, offsetPos, easeType, easeAmt, groupID);
}

void BaseGameLayer::processMoveActionsStep(float dt)
{
	this->processMoveActions(dt);
}
