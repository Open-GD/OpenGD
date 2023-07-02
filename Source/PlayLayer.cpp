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

#include "PlayLayer.h"
#include "AudioEngine.h"
#include "CreatorLayer.h"
#include "EffectGameObject.h"
#include "EndLevelLayer.h"

#include "LevelInfoLayer.h"
#include "LevelPage.h"
#include "LevelSelectLayer.h"
#include "LevelTools.h"
#include "MenuItemSpriteExtra.h"
#include "Random.h"

#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"

#include "external/benchmark.h"
#include "external/json.hpp"
#include "external/constants.h"

#include <charconv>
#include <fstream>

#include "LevelDebugLayer.h"
#include "UILayer.h"
#include "GJGameLevel.h"
#include "GroundLayer.h"
#include "SimpleProgressBar.h"
#include "CircleWave.h"
#include "GameToolbox/log.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/rand.h"
#include "GameToolbox/math.h"
#include "GameToolbox/conv.h"
#include "GameToolbox/nodes.h"


USING_NS_AX;
USING_NS_AX_EXT;

bool showDn = false, noclip = false;

float gameSpeed = 1, fps = 0;

bool fullscreen = false;
int monitorN = 0;

static PlayLayer* Instance = nullptr;

ax::Node* cameraFollow;

Scene* PlayLayer::scene(GJGameLevel* level)
{
	return LevelDebugLayer::scene(level);
	//auto scene = Scene::create();
	//scene->addChild(PlayLayer::create(level));
	//return scene;
}

void PlayLayer::showCompleteText()
{
	m_bEndAnimation = true;

	auto size = Director::getInstance()->getWinSize();

	float scale = 1.1f;
	const char* spr = "GJ_levelComplete_001.png";
	/*if (m_isPracticeMode) {
		spr = "GJ_practiceComplete_001.png";
		scale = 1;
	}*/

	auto sprite = Sprite::createWithSpriteFrameName(spr);
	sprite->setScale(0.01f);
	sprite->setPosition({size.width / 2, size.height / 2 + 35});
	m_pHudLayer->addChild(sprite);

	sprite->runAction(Sequence::create(EaseElasticOut::create(ScaleTo::create(0.66f, scale), 0.6),
									   DelayTime::create(0.88f), EaseIn::create(ScaleTo::create(0.22f, 0), 2.0f),
									   RemoveSelf::create(true), nullptr));

	auto col1 = _player1->getMainColor();
	auto col2 = _player1->getSecondaryColor();

	auto par1 = ParticleSystemQuad::create("levelComplete01.plist");
	par1->setPosition(sprite->getPosition());
	par1->setStartColor({(GLfloat)col1.r, (GLfloat)col1.g, (GLfloat)col1.b, 255});
	par1->setEndColor({(GLfloat)col1.r, (GLfloat)col1.g, (GLfloat)col1.b, 0});
	m_pHudLayer->addChild(par1, -1);

	auto par2 = ParticleSystemQuad::create("levelComplete01.plist");
	par2->setPosition(par1->getPosition());
	par2->setStartColor({(GLfloat)col2.r, (GLfloat)col2.g, (GLfloat)col2.b, 255});
	par2->setEndColor({(GLfloat)col2.r, (GLfloat)col2.g, (GLfloat)col2.b, 0});
	m_pHudLayer->addChild(par2, -1);

	auto cir = CircleWave::create(0.8f, {col1.r, col1.g, col1.b, 255}, 5.f, size.width - 10, true, false);
	cir->setPosition({size.width - 10, size.height / 2});
	m_pHudLayer->addChild(cir, -1);

	auto cir2 = CircleWave::create(0.8f, {col1.r, col1.g, col1.b, 255}, 5.f, 250.0f, true, false);
	cir2->setPosition(sprite->getPosition());
	m_pHudLayer->addChild(cir2, -1);

	// for (int i = 0; i < 9; i++)
	// 	m_pHudLayer->runAction(Sequence::createWithTwoActions(
	// 		DelayTime::create(0.16f * i), CallFunc::create([&]() { PlayLayer::spawnCircle(); })));

	// m_pHudLayer->runAction(
	// 	Sequence::createWithTwoActions(DelayTime::create(1.5f), CallFunc::create([&]() { PlayLayer::showEndLayer();
	// })));
	for (int i = 0; i < 9; i++)
	{
		scheduleOnce([=](float d) { spawnCircle(); }, 0.16f * i, "playlayer_circles");
	}
	scheduleOnce([=](float d) { showEndLayer(); }, 1.5f, "playlayer_levelend");
}			

void PlayLayer::spawnCircle()
{
	auto size = Director::getInstance()->getWinSize();
	
	auto minArea = Vec2({40, 70});
	auto maxArea = Vec2({size.width - 40, size.height - 70});

	float x = ((float)rand() / (float)RAND_MAX) * (maxArea.x - minArea.x) + minArea.x;
	float y = ((float)rand() / (float)RAND_MAX) * (maxArea.y - minArea.y) + minArea.y;

	auto col1 = _player1->getMainColor();
	auto cir = CircleWave::create(0.5f, {col1.r, col1.g, col1.b, 255}, 5.f, 50, true, false);
	cir->setPosition({x, y});
	m_pHudLayer->addChild(cir, -1);
}

void PlayLayer::showEndLayer()
{
	createLevelEnd();
}

int PlayLayer::sectionForPos(float x)
{
	int section = x / 100;
	if (section < 0)
		section = 0;
	return section;
}

PlayLayer* PlayLayer::create(GJGameLevel* level)
{
	auto ret = new (std::nothrow) PlayLayer();
	if (ret && ret->init(level))
	{
		ret->autorelease();
		return ret;
	}

	AX_SAFE_DELETE(ret);
	return nullptr;
}


void PlayLayer::fillColorChannel(std::span<std::string_view> colorString, int id)
{
	for (size_t j = 0; j < colorString.size() - 1; j += 2)
	{
		switch (GameToolbox::stoi(colorString[j]))
		{
		case 1:
			m_pColorChannels.insert({id, SpriteColor(ax::Color3B(GameToolbox::stof(colorString[j + 1]), 0, 0), 255, 0)});
			break;
		case 2:
			m_pColorChannels.at(id)._color.g = GameToolbox::stof(colorString[j + 1]);
			break;
		case 3:
			m_pColorChannels.at(id)._color.b = GameToolbox::stof(colorString[j + 1]);
			break;
		}
	}
}

void PlayLayer::loadLevel(std::string_view levelStr)
{

	std::vector<std::string_view> objData = GameToolbox::splitByDelimStringView(levelStr, ';');
	std::vector<std::string_view> levelData = GameToolbox::splitByDelimStringView(objData[0], ',');
	
	objData.erase(objData.begin());
	
	std::thread t_colorChannels([&]()
	{
		for (size_t i = 0; i < levelData.size() - 1; i += 2)
		{
			if (levelData[i] == "kS1")
			{
				m_pColorChannels.insert({1000, SpriteColor(ax::Color3B(GameToolbox::stof(levelData[i + 1]), 0, 0), 255, 0)});
			}
			else if (levelData[i] == "kS2")
			{
				m_pColorChannels.at(1000)._color.g = GameToolbox::stof(levelData[i + 1]);
			}
			else if (levelData[i] == "kS3")
			{
				m_pColorChannels.at(1000)._color.b = GameToolbox::stof(levelData[i + 1]);
			}
			else if (levelData[i] == "kS4")
			{
				m_pColorChannels.insert({1001, SpriteColor(ax::Color3B(GameToolbox::stof(levelData[i + 1]), 0, 0), 255, 0)});
			}
			else if (levelData[i] == "kS5")
			{
				m_pColorChannels.at(1001)._color.g = GameToolbox::stof(levelData[i + 1]);
			}
			else if (levelData[i] == "kS6")
			{
				m_pColorChannels.at(1001)._color.b = GameToolbox::stof(levelData[i + 1]);
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
					m_pColorChannels.insert({key, col});
				}
			}
			else if (levelData[i] == "kA6")
			{
				_bgID = GameToolbox::stoi(levelData[i + 1]);
				if (!_bgID) _bgID = 1;
			}
			else if (levelData[i] == "kA7")
			{
				_groundID = GameToolbox::stoi(levelData[i + 1]);
				if (!_groundID) _groundID = 1;
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
			
		} //for (size_t i = 0; i < levelData.size() - 1; i += 2)
		
	}); //thread
	
	t_colorChannels.join();
	
	//STOP
	std::thread t_gameObjects([&]() {
		
		
	m_pColorChannels[1005]._color = _player1->getMainColor();
	m_pColorChannels[1005]._blending = true;
	m_pColorChannels[1006]._color = _player1->getSecondaryColor();
	m_pColorChannels[1006]._blending = true;
	m_pColorChannels[1010]._color = Color3B::BLACK;
	m_pColorChannels[1007]._color = getLightBG();

	_originalColors = std::unordered_map<int, SpriteColor, my_string_hash>(m_pColorChannels);

	for (std::string_view data : objData)
	{
		auto d = GameToolbox::splitByDelimStringView(data, ',');

		GameObject* obj = nullptr;

		Hitbox hb = {0, 0, 0, 0};

		for (size_t i = 0; i < d.size() - 1; i += 2)
		{
			int key = GameToolbox::stoi(d[i]);

			if (key != 1 && obj == nullptr)
				break;

			switch (key)
			{
			case 1:
			{
				int id = GameToolbox::stoi(d[i + 1]);

				if (!GameObject::_pBlocks.contains(id))
					continue;

				std::string_view frame = GameObject::_pBlocks.at(id);

				if (std::find(std::begin(GameObject::_pTriggers), std::end(GameObject::_pTriggers), id) !=
					std::end(GameObject::_pTriggers))
				{
					obj = EffectGameObject::create(frame);
					obj->_isTrigger = true;
				}
				else
					obj = GameObject::create(frame, GameObject::getGlowFrame(id));

				if (obj == nullptr)
					break;

				AX_SAFE_RETAIN(obj);

				obj->setStretchEnabled(false);
				obj->setActive(true);
				obj->setID(id);

				// obj->setupColors();

				obj->customSetup();

				if (GameObject::_pHitboxes.contains(id))
					hb = GameObject::_pHitboxes.at(id);
				if (GameObject::_pHitboxRadius.contains(id))
					obj->_radius = GameObject::_pHitboxRadius.at(id);

				obj->_uniqueID = _pObjects.size();

				_pObjects.push_back(obj);
			}
			break;
			case 2:
				obj->setPositionX(GameToolbox::stof(d[i + 1]));
				break;
			case 3:
				obj->setPositionY(GameToolbox::stof(d[i + 1]) + 90.0f);
				break;
			case 4:
				obj->setScaleX(-1.f * GameToolbox::stoi(d[i + 1]));
				break;
			case 5:
				obj->setScaleY(-1.f * GameToolbox::stoi(d[i + 1]));
				break;
			case 6:
				obj->setRotation(GameToolbox::stof(d[i + 1]));
				break;
			case 7:
				dynamic_cast<EffectGameObject*>(obj)->_color.r = GameToolbox::stof(d[i + 1]);
				break;
			case 8:
				dynamic_cast<EffectGameObject*>(obj)->_color.g = GameToolbox::stof(d[i + 1]);
				break;
			case 9:
				dynamic_cast<EffectGameObject*>(obj)->_color.b = GameToolbox::stof(d[i + 1]);
				break;
			case 10:
				dynamic_cast<EffectGameObject*>(obj)->_duration = GameToolbox::stof(d[i + 1]);
				break;
			case 21:
				obj->_mainColorChannel = GameToolbox::stoi(d[i + 1]);
				break;
			case 22:
				obj->_secColorChannel = GameToolbox::stoi(d[i + 1]);
				break;
			case 23:
				dynamic_cast<EffectGameObject*>(obj)->_targetColorId = GameToolbox::stof(d[i + 1]);
				break;
			case 24:
				obj->_zLayer = GameToolbox::stoi(d[i + 1]);
				break;
			case 25:
				obj->setGlobalZOrder(GameToolbox::stoi(d[i + 1]));
				break;
			case 32:
				obj->setScaleX(obj->getScaleX() * GameToolbox::stof(d[i + 1]));
				obj->setScaleY(obj->getScaleY() * GameToolbox::stof(d[i + 1]));
				break;
			case 35:
				dynamic_cast<EffectGameObject*>(obj)->_opacity = GameToolbox::stof(d[i + 1]);
				break;
			case 45:
				dynamic_cast<EffectGameObject*>(obj)->_fadeIn = GameToolbox::stof(d[i + 1]);
				break;
			case 46:
				dynamic_cast<EffectGameObject*>(obj)->_hold = GameToolbox::stof(d[i + 1]);
				break;
			case 47:
				dynamic_cast<EffectGameObject*>(obj)->_fadeOut = GameToolbox::stof(d[i + 1]);
				break;
			case 48:
				dynamic_cast<EffectGameObject*>(obj)->_pulseMode = GameToolbox::stoi(d[i + 1]);
				break;
			case 49: {
				auto hsv = GameToolbox::splitByDelimStringView(d[i + 1], 'a');
				auto trigger = dynamic_cast<EffectGameObject*>(obj);
				trigger->_hsv.h = GameToolbox::stof(hsv[0]);
				trigger->_hsv.s = GameToolbox::stof(hsv[1]);
				trigger->_hsv.v = GameToolbox::stof(hsv[2]);
				trigger->_hsv.sChecked = GameToolbox::stof(hsv[3]);
				trigger->_hsv.vChecked = GameToolbox::stof(hsv[4]);
			}
			break;
			case 50:
				dynamic_cast<EffectGameObject*>(obj)->_copiedColorId = GameToolbox::stoi(d[i + 1]);
				break;
			case 52:
				dynamic_cast<EffectGameObject*>(obj)->_pulseType = GameToolbox::stoi(d[i + 1]);
				break;
			case 51:
				if (obj->_isTrigger)
					dynamic_cast<EffectGameObject*>(obj)->_targetGroupId = GameToolbox::stoi(d[i + 1]);
				break;
			case 57: {
				auto groups = GameToolbox::splitByDelimStringView(data, '.');
				for (auto group : groups)
				{
					int g = GameToolbox::stoi(group);
					_groups[g]._objects.push_back(obj);
					obj->_groups.push_back(g);
				}
				break;
			}
			case 67: // dont enter
			case 64: // dont exit
				obj->setDontTransform(true);
				break;
			}
		}
		if (obj)
		{
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
		}
	}
	});
	t_gameObjects.join();
}

bool PlayLayer::isObjectBlending(GameObject* obj)
{
	return m_pColorChannels.contains(obj->_mainColorChannel) && m_pColorChannels[obj->_mainColorChannel]._blending &&
			   m_pColorChannels.contains(obj->_secColorChannel) && m_pColorChannels[obj->_secColorChannel]._blending ||
		   !m_pColorChannels.contains(obj->_mainColorChannel) && m_pColorChannels.contains(obj->_secColorChannel) &&
			   m_pColorChannels[obj->_secColorChannel]._blending ||
		   !m_pColorChannels.contains(obj->_secColorChannel) && m_pColorChannels.contains(obj->_mainColorChannel) &&
			   m_pColorChannels[obj->_mainColorChannel]._blending;
}

bool PlayLayer::init(GJGameLevel* level)
{
	if (!Layer::init())
		return false;
	setLevel(level);

	Instance = this;

	auto winSize = Director::getInstance()->getWinSize();

	dn = ax::DrawNode::create();
	dn->setPosition({-15, -15});
	addChild(dn, 99999);

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

	this->_player1 = PlayerObject::create(GameToolbox::randomInt(1, 12), this);
	this->_player1->setPosition({-20, 105});
	_main2BatchNode->addChild(this->_player1, 3);
	this->_player1->setAnchorPoint({0, 0});

	_player1->setMainColor({125, 255, 0});
	_player1->setSecondaryColor({0, 255, 255});

	this->_player2 = PlayerObject::create(GameToolbox::randomInt(1, 12), this);
	this->_player2->setPosition({-20, 105});
	_main2BatchNode->addChild(this->_player2, 3);
	this->_player2->setAnchorPoint({0, 0});

	_player2->setMainColor({125, 255, 0});
	_player2->setSecondaryColor({0, 255, 255});

	// std::string levelStr = FileUtils::getInstance()->getStringFromFile("level.txt");
	std::string levelStr = level->_levelString;

	if (levelStr.empty())
	{
		nlohmann::json file = nlohmann::json::parse(FileUtils::getInstance()->getStringFromFile("Custom/mainLevels.json"));
		levelStr = fmt::format("H4sIAAAAAAAAA{}", file[std::to_string(level->_levelID)]);
	}

	// scope based timer
	{
		auto s = BenchmarkTimer("load level");
		levelStr = GJGameLevel::decompressLvlStr(levelStr);
		loadLevel(levelStr);
	}

	this->_bottomGround = GroundLayer::create(_groundID);
	this->_ceiling = GroundLayer::create(_groundID);
	cameraFollow = ax::Node::create();
	cameraFollow->addChild(this->_bottomGround, 1);
	cameraFollow->addChild(this->_ceiling, 1);
	this->addChild(cameraFollow, 100);

	this->_ceiling->setScaleY(-1);
	_ceiling->setVisible(false);
	_bottomGround->setPositionY(-cameraFollow->getPositionY() + 12);
	_ceiling->setPositionY(winSize.height + _ceiling->_sprite->getTextureRect().size.height);

	this->m_pBG = Sprite::create(GameToolbox::getTextureString(fmt::format("game_bg_{:02}_001.png", _bgID)));
	if (!this->m_pBG)
	{
		this->m_pBG = Sprite::create(GameToolbox::getTextureString(fmt::format("game_bg_{:02}_001.png", 1)));
	}
	m_pBG->setStretchEnabled(false);
	const Texture2D::TexParams texParams = {backend::SamplerFilter::LINEAR, backend::SamplerFilter::LINEAR,
											backend::SamplerAddressMode::REPEAT, backend::SamplerAddressMode::REPEAT};
	this->m_pBG->getTexture()->setTexParameters(texParams);
	this->m_pBG->setTextureRect(Rect(0, 0, 1024 * 5, 1024));
	this->m_pBG->setPosition(winSize.x / 2, winSize.y / 4);
	this->addChild(this->m_pBG, -100);

	if (this->m_pColorChannels.contains(1000))
		this->m_pBG->setColor(this->m_pColorChannels.at(1000)._color);
	this->_bottomGround->update(0);

	if (_pObjects.size() != 0)
	{
		this->m_lastObjXPos = 570.0f;

		for (GameObject* object : _pObjects)
		{
			// GameToolbox::log("pos: {}", object->getPositionX());
			if (this->m_lastObjXPos < object->getPositionX())
				this->m_lastObjXPos = object->getPositionX();
		}

		GameToolbox::log("last x: {}", m_lastObjXPos);

		for (size_t i = 0; i < sectionForPos(this->m_lastObjXPos); i++)
		{
			// GameToolbox::log("i = {}", i);
			std::vector<GameObject*> vec;
			m_pSectionObjects.push_back(vec);
		}

		for (GameObject* object : _pObjects)
		{
			int section = sectionForPos(object->getPositionX());
			m_pSectionObjects[section - 1 < 0 ? 0 : section - 1].push_back(object);

			if (m_pColorChannels.contains(object->_mainColorChannel) &&
				m_pColorChannels[object->_mainColorChannel]._blending)
			{
				object->setBlendFunc(GameToolbox::getBlending());
			}

			if (m_pColorChannels.contains(object->_secColorChannel) &&
				m_pColorChannels[object->_secColorChannel]._blending)
			{
				for (auto s : object->_childSprites)
					s->setBlendFunc(GameToolbox::getBlending());
			}
			object->setCascadeOpacityEnabled(false);
			object->update();
		}
	}

	m_pHudLayer = UILayer::create();

	m_pBar = SimpleProgressBar::create();
	m_pBar->setPercentage(0.f);
	m_pBar->setPosition({winSize.width / 2, winSize.height - 10});
	m_pHudLayer->addChild(m_pBar);

	m_pBar->setPosition({winSize.width / 2, winSize.height - 10});

	m_pPercentage = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "0%");
	m_pPercentage->setScale(.5f);
	m_pPercentage->setAnchorPoint({0, .5f});
	m_pPercentage->setPosition({m_pBar->getPositionX() + 110, m_pBar->getPositionY() + 1});
	m_pHudLayer->addChild(m_pPercentage);

	this->addChild(m_pHudLayer, 1000);

	//bool levelValid = LevelTools::verifyLevelIntegrity(levelStr, this->getLevel()->_levelID);
	constexpr bool levelValid = true;

	if (!levelValid)
	{
		auto loadfailedstr = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Load Failed!");
		loadfailedstr->setPosition({winSize.width / 2, winSize.height / 2});
		addChild(loadfailedstr, 128);
	}
	
	updateVisibility();
	updateVisibility();

	scheduleOnce(
		[=](float d) {
			if (levelValid)
			{
				incrementTime();
				resetLevel();
			}
			else
			{
				exit();
			}
		},
		1.f, "playlayer_levelstartdelay");

	return true;
}

void PlayLayer::createLevelEnd()
{
	_jumps = _player1->_jumpedTimes;
	auto levelend = EndLevelLayer::create(this);
	addChild(levelend);
}

double lastY = 0;

void PlayLayer::incrementTime()
{
	scheduleOnce(
		[=](float d) {
			_secondsSinceStart++;
			incrementTime();
		},
		1.f, "playlayer_stopwatch");
}

void PlayLayer::update(float dt)
{

	if (m_freezePlayer)
	{
		AudioEngine::pauseAll();
		return;
	}
	else
	{
		AudioEngine::resumeAll();
	}

	float step = std::min(2.0f, dt * 60.0f);

	_player1->m_bIsPlatformer = m_platformerMode;
	_player1->noclip = noclip;
	_player2->noclip = noclip;

	auto winSize = Director::getInstance()->getWinSize();

	this->m_pColorChannels.at(1005)._color = this->_player1->getMainColor();
	this->m_pColorChannels.at(1006)._color = this->_player1->getSecondaryColor();

	m_pColorChannels[1007]._color = getLightBG();

	if (!m_freezePlayer && (!this->_player1->isDead() || !this->_player2->isDead()))
	{
		step /= 4.0f;
		lastY = _player1->getYVel();
		for (int i = 0; i < 4; i++)
		{
			this->_player1->update(step);

			_player1->setOuterBounds(Rect(_player1->getPosition(), Vec2(30, 30)));
			_player1->setInnerBounds(Rect(_player1->getPosition() + Vec2(11.25f, 11.25f), Vec2(7.5, 7.5)));

			this->checkCollisions(_player1, step);

			if (this->_player1->isDead())
				break;

			if (!_isDualMode)
				continue;

			this->_player2->update(step);

			_player2->setOuterBounds(Rect(_player1->getPosition(), Vec2(30, 30)));
			_player2->setInnerBounds(Rect(_player1->getPosition() + Vec2(11.25f, 11.25f), Vec2(7.5, 7.5)));

			this->checkCollisions(_player2, step);

			if (this->_player2->isDead())
				break;
		}
		step *= 4.0f;
	}

	m_pBar->setPercentage(_player1->getPositionX() / this->m_lastObjXPos * 100.f);
	float val = m_pBar->getPercentage();
	m_pPercentage->setString(StringUtils::format("%.02f%%", val > 100 ? 100 : val < 0 ? 0 : val));

	if (val >= 100 && !m_bEndAnimation)
		this->showCompleteText();

	this->updateVisibility();
	this->updateCamera(step);
	if (_player1->_currentGamemode == PlayerGamemodeShip)
		_player1->updateShipRotation(step);
	if (_isDualMode && _player2->_currentGamemode == PlayerGamemodeShip)
		_player2->updateShipRotation(step);

	m_pColorChannels[1005]._color = _player1->getMainColor();
	m_pColorChannels[1006]._color = _player1->getSecondaryColor();
}

ax::Color3B PlayLayer::getLightBG()
{
	return m_pColorChannels[1000]._color;
}

void PlayLayer::destroyPlayer(PlayerObject* player)
{

	if (player->isDead() || player->noclip)
		return;

	player->setIsDead(true);
	player->playDeathEffect();
	player->stopRotation();
	player->setVisible(false);

	scheduleOnce([=](float d) { resetLevel(); }, 1.f, "playlayer_restart");
}

void PlayLayer::updateCamera(float dt)
{
	auto winSize = Director::getInstance()->getWinSize();
	Vec2 cam = m_obCamPos;

	PlayerObject* player = _player1;
	Vec2 pPos = player->getPosition();

	if (player->_currentGamemode != PlayerGamemodeCube || _isDualMode)
	{
		cam.y = (winSize.height * -0.5f) + m_fCameraYCenter;
		if (cam.y <= 0.0f)
			cam.y = 0.0f;
	}
	else
	{
		float unk2 = 90.0f;
		float unk3 = 120.0f;
		if (player->isGravityFlipped())
		{
			unk2 = 120.0f;
			unk3 = 90.0f;
		}
		if (pPos.y <= winSize.height + cam.y - unk2)
		{
			if (pPos.y < unk3 + cam.y)
				cam.y = pPos.y - unk3;
		}
		else
			cam.y = pPos.y - winSize.height + unk2;
		if (!player->isGravityFlipped())
		{
			Vec2 lastGroundPos = player->getLastGroundPos();

			if (lastGroundPos.y == 105.f)
				if (pPos.y <= cam.y + winSize.height - unk2)
					cam.y = 0.0f;
		}
	}

	cam.y = clampf(cam.y, 0.0f, 1140.f - winSize.height);

	if (pPos.x >= winSize.width / 2.5f && !_player1->isDead() && !_player2->isDead() &&
		!player->m_bIsPlatformer) // wrong but works for now
	{
		this->m_pBG->setPositionX(this->m_pBG->getPositionX() -
								  dt * player->getPlayerSpeed() * _bottomGround->getSpeed() * 0.1175f);
		_bottomGround->update(dt * player->getPlayerSpeed());
		_ceiling->update(dt * player->getPlayerSpeed());
		cam.x = pPos.x - (winSize.width / 2.5f);
	}
	else if (player->m_bIsPlatformer)
		cam.x = pPos.x - winSize.width / 2.f;

	if (this->m_pBG->getPosition().x <= cam.x - 1024.f)
		this->m_pBG->setPositionX(this->m_pBG->getPositionX() + 1024.f);

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

	Camera::getDefaultCamera()->setPosition(this->m_obCamPos + winSize / 2);

	cameraFollow->setPosition(m_obCamPos);
	_ceiling->setVisible(_player1->_currentGamemode != PlayerGamemodeCube);
	if (_player1->_currentGamemode == PlayerGamemodeCube)
		_bottomGround->setPositionY(-cameraFollow->getPositionY() + 12);

	m_pHudLayer->setPosition(this->m_obCamPos);
}

float PlayLayer::getRelativeMod(Vec2 pos, float v1, float v2, float v3)
{
	auto winSize = ax::Director::getInstance()->getWinSize();
	float camX = m_obCamPos.x;
	float centerX = winSize.width / 2.f;
	float camXCenter = camX + centerX;
	float posX = pos.x;

	float vv1;
	float vv2;
	float vv3;
	float result;

	if (posX <= camXCenter)
	{
		vv2 = v2;
		vv3 = (camXCenter - posX) - v3;
	}
	else
	{
		vv1 = ((posX - v3) - camX) - centerX;
		vv2 = v1;
		vv3 = vv1;
	}
	if (vv2 < 1.f)
		vv2 = 1.f;

	result = (centerX - vv3) / vv2;

	return result;
}

void PlayLayer::applyEnterEffect(GameObject* obj)
{
	if (obj->getEnterEffectID() != _enterEffectID)
		obj->setEnterEffectID(_enterEffectID);
	Vec2 objStartPos = obj->getStartPosition();
	Vec2 objStartScale = obj->getStartScale();
	float rModn = getRelativeMod(objStartPos, 60.f, 60.f, 0.f);
	float rMod = clampf(rModn, 0.f, 1.f);

	switch (obj->getEnterEffectID())
	{
	case 2:
		if (obj->getGameObjectType() != kGameObjectTypeYellowJumpPad)
		{
			obj->setScaleX(rMod * objStartScale.x);
			obj->setScaleY(rMod * objStartScale.y);
		}
		break;
	case 3:
		if (obj->getGameObjectType() != kGameObjectTypeYellowJumpPad)
		{
			obj->setScaleX((2.f - rMod) * objStartScale.x);
			obj->setScaleY((2.f - rMod) * objStartScale.y);
		}
		break;
	case 4:
		if (obj->getGameObjectType() != kGameObjectTypeYellowJumpPad)
			obj->setPositionY((1.0 - rMod) * 100.f + objStartPos.y);
		break;
	case 5:
		if (obj->getGameObjectType() != kGameObjectTypeYellowJumpPad)
			obj->setPositionY((1.0 - rMod) * -100.f + objStartPos.y);
		break;
	case 6:
		if (obj->getGameObjectType() != kGameObjectTypeYellowJumpPad)
			obj->setPositionX((1.0 - rMod) * -100.f + objStartPos.x);
		break;
	case 7:
		if (obj->getGameObjectType() != kGameObjectTypeYellowJumpPad)
			obj->setPositionX((1.0 - rMod) * 100.f + objStartPos.x);
		break;
	default:
		obj->setPosition(objStartPos);
		break;
	}
	obj->setEnterEffectID(0);
}

void PlayLayer::updateVisibility()
{
	auto winSize = ax::Director::getInstance()->getWinSize();

	float unk = 70.0f;

	int prevSection = floorf(this->m_obCamPos.x / 100) - 1.0f;
	int nextSection = ceilf((this->m_obCamPos.x + winSize.width) / 100) + 1.0f;

	for (int i = prevSection; i < nextSection; i++)
	{
		if (i >= 0)
		{
			if (i < m_pSectionObjects.size())
			{
				auto section = m_pSectionObjects[i];
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

					// if (obj->getType() == kBallFrame || obj->getType() ==
					// kYellowJumpRing)
					//	 obj->setScale(this->getAudioEffectsLayer()->getAudioScale())

					// auto pos = obj->getPosition();

					float unk2 = 0.0f;
					if (obj->getGameObjectType() == kGameObjectTypeDecoration)
						unk2 = obj->getTextureRect().origin.x * abs(obj->getScaleX()) * 0.4f;

					float opacity = clampf(getRelativeMod(obj->getPosition(), 70.f, 70.f, unk2), 0.f, 1.f);
					if (!obj->getDontTransform())
					{
						obj->_effectOpacityMultipler = opacity;
						this->applyEnterEffect(obj);
					}
				}
			}
		}
	}

	if (_prevSection - 1 >= 0 && m_pSectionObjects.size() != 0 && _prevSection <= m_pSectionObjects.size())
	{
		auto section = m_pSectionObjects[_prevSection - 1];
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
				if (isObjectBlending(section[j]))
				{
					switch (section[j]->_zLayer)
					{
					case -3:
						_blendingBatchNodeB4->removeChild(section[j], true);
						break;
					case -1:
						_blendingBatchNodeB3->removeChild(section[j], true);
						break;
					case 1:
						_blendingBatchNodeB2->removeChild(section[j], true);
						break;
					case 3:
						_blendingBatchNodeB1->removeChild(section[j], true);
						break;
					default:
					case 5:
						_blendingBatchNodeT1->removeChild(section[j], true);
						break;
					case 7:
						_blendingBatchNodeT2->removeChild(section[j], true);
						break;
					case 9:
						_blendingBatchNodeT3->removeChild(section[j], true);
						break;
					}
				}
				else
				{
					if (section[j]->_texturePath == _mainBatchNodeTexture)
					{
						switch (section[j]->_zLayer)
						{
						case -3:
							_mainBatchNodeB4->removeChild(section[j], true);
							break;
						case -1:
							_mainBatchNodeB3->removeChild(section[j], true);
							break;
						case 1:
							_mainBatchNodeB2->removeChild(section[j], true);
							break;
						case 3:
							_mainBatchNodeB1->removeChild(section[j], true);
							break;
						default:
						case 5:
							_mainBatchNodeT1->removeChild(section[j], true);
							break;
						case 7:
							_mainBatchNodeT2->removeChild(section[j], true);
							break;
						case 9:
							_mainBatchNodeT3->removeChild(section[j], true);
							break;
						}
					}
					else if (section[j]->_texturePath == _main2BatchNodeTexture)
						_main2BatchNode->removeChild(section[j], true);
				}
			}
		}
	}

	this->_prevSection = prevSection;
	this->_nextSection = nextSection;
}

void PlayLayer::changeGameMode(GameObject* obj, PlayerObject* player, PlayerGamemode gameMode)
{
	obj->triggerActivated(player);
	switch (gameMode)
	{
	case PlayerGamemodeShip:
	case PlayerGamemodeUFO:
	case PlayerGamemodeWave:
		if (obj->getPositionY() < 270)
			m_fCameraYCenter = 240.0f;
		else
			m_fCameraYCenter = (floorf(obj->getPositionY() / 30.0f) * 30.0f);

		tweenBottomGround(-68);
		tweenCeiling(388);
		break;
	case PlayerGamemodeBall:
		if (obj->getPositionY() < 240.0f)
			m_fCameraYCenter = 210.0f;
		else
			m_fCameraYCenter = (floorf(obj->getPositionY() / 30.0f) * 30.0f);

		tweenBottomGround(-38);
		tweenCeiling(358);
		break;
	default:
		break;
	}

	player->setRotation(0.f);
	player->setGamemode(gameMode);
}

void PlayLayer::moveCameraToPos(Vec2 pos)
{
	auto moveX = [this](float a, float b, float c) -> void {
		this->stopActionByTag(0);
		auto tweenAction = ActionTween::create(b, "cTX", m_obCamPos.x, a);
		auto easeAction = EaseInOut::create(tweenAction, c);
		easeAction->setTag(0);
		this->runAction(easeAction);
	};
	auto moveY = [this](float a, float b, float c) -> void {
		this->stopActionByTag(1);
		auto tweenAction = ActionTween::create(b, "cTY", m_obCamPos.y, a);
		auto easeAction = EaseInOut::create(tweenAction, c);
		easeAction->setTag(1);
		this->runAction(easeAction);
	};
	moveX(pos.x, 1.2f, 1.8f);
	moveY(pos.y, 1.2f, 1.8f);
}

void PlayLayer::checkCollisions(PlayerObject* player, float dt)
{
	auto playerOuterBounds = player->_mini ? player->getOuterBounds(0.6f, 0.6f) : player->getOuterBounds();
	if (player->getPositionY() < (player->_mini ? 99.f : 105.0f) && player->_currentGamemode == PlayerGamemodeCube)
	{
		if (player->isGravityFlipped())
		{
			this->destroyPlayer(player);
			return;
		}

		player->setPositionY((player->_mini ? 99.f : 105.0f));

		player->hitGround(false);
	}

	else if (player->getPositionY() > 1290.0f)
	{
		this->destroyPlayer(player);
		return;
	}

	if (player->_currentGamemode != PlayerGamemodeCube)
	{
		if (player->getPositionY() <
			_bottomGround->getPositionY() + cameraFollow->getPositionY() + (player->_mini ? 87.f : 93.0f))
		{
			player->setPositionY(_bottomGround->getPositionY() + cameraFollow->getPositionY() +
								 (player->_mini ? 87.f : 93.0f));

			if (!player->isGravityFlipped())
				player->hitGround(false);

			player->setYVel(0.f);
		}
		if (player->getPositionY() >
			_ceiling->getPositionY() - (player->_mini ? 234.f : 240.f) + m_fCameraYCenter - 12.f)
		{
			player->setPositionY(_ceiling->getPositionY() - (player->_mini ? 234.f : 240.f) + m_fCameraYCenter - 12.f);

			if (player->isGravityFlipped())
				player->hitGround(true);

			player->setYVel(0.f);
		}
	}

	dn->setVisible(showDn);

	if (showDn)
	{
		dn->clear();
		renderRect(playerOuterBounds, ax::Color4B::RED);
		renderRect(player->getInnerBounds(), ax::Color4B::GREEN);
	}

	int current_section = this->sectionForPos(player->getPositionX());

	std::deque<GameObject*> m_pHazards;

	for (int i = current_section - 2; i < current_section + 1; i++)
	{
		if (i < m_pSectionObjects.size() && i >= 0)
		{
			std::vector<GameObject*> section = m_pSectionObjects[i];

			for (int j = 0; j < section.size(); j++)
			{
				GameObject* obj = section[j];

				auto objBounds = obj->getOuterBounds();

				if ((objBounds.size.width <= 0 || objBounds.size.height <= 0))
					continue;

				if (obj->getGameObjectType() == kGameObjectTypeHazard)
				{
					m_pHazards.push_back(obj);
					if (showDn)
					{
						if (obj->_radius <= 0)
							renderRect(objBounds, ax::Color4B::RED);
						else
							dn->drawCircle(obj->getPosition() + Vec2(15, 15), obj->_radius, 0, 20, 0, ax::Color4B::RED);
					}
				}
				else if (obj->isActive())
				{
					if (obj->_isTrigger)
					{
						auto trigger = dynamic_cast<EffectGameObject*>(obj);
						if (!trigger->_wasTriggerActivated && trigger->getPositionX() <= player->getPositionX())
						{
							trigger->triggerActivated(dt);
						}
					}
					if (showDn)
					{
						renderRect(objBounds, ax::Color4B::BLUE);
					}

					if (playerOuterBounds.intersectsRect(objBounds) && !obj->hasBeenActiavedByPlayer(player))
					{
						switch (obj->getGameObjectType())
						{
						case kGameObjectTypeInverseGravityPortal:
							// if (!player->isGravityFlipped())
							//	 this->playGravityEffect(true);
							obj->triggerActivated(player);
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							changeGravity(true);
							break;

						case kGameObjectTypeNormalGravityPortal:
							// if (player->isGravityFlipped())
							//	 this->playGravityEffect(false);
							obj->triggerActivated(player);
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							changeGravity(false);
							break;

						case kGameObjectTypeShipPortal:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							this->changeGameMode(obj, player, PlayerGamemodeShip);
							break;

						case kGameObjectTypeBallPortal:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							this->changeGameMode(obj, player, PlayerGamemodeBall);
							break;

						case kGameObjectTypeUfoPortal:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							this->changeGameMode(obj, player, PlayerGamemodeUFO);
							break;

						case kGameObjectTypeCubePortal:

							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							this->changeGameMode(obj, player, PlayerGamemodeCube);
							break;

						case kGameObjectTypeYellowJumpPad:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);
							obj->triggerActivated(player);
							player->propellPlayer(1);
							player->_touchedPadObject = obj;
							break;

						case kGameObjectTypeGravityPad: {
							if (player->_touchedPadObject)
								break;
							auto pos = obj->getPosition();
							pos.y -= 10;
							player->setPortalP(pos);
							player->setPortalObject(obj);
							obj->triggerActivated(player);
							player->propellPlayer(0.8);
							player->_touchedPadObject = obj;
							changeGravity(!player->isGravityFlipped());
							break;
						}

						case kGameObjectTypePinkJumpPad:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);
							obj->triggerActivated(player);
							player->propellPlayer(0.65);
							player->_touchedPadObject = obj;
							break;

						case kGameObjectTypeRedJumpPad:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);
							obj->triggerActivated(player);
							player->propellPlayer(1.25);
							player->_touchedPadObject = obj;
							break;

						case kGameObjectTypeYellowJumpRing:
						case kGameObjectTypeDashRing:
						case kGameObjectTypeGravityRing:
						case kGameObjectTypeRedJumpRing:
						case kGameObjectTypePinkJumpRing:
						case kGameObjectTypeDropRing:
						case kGameObjectTypeGreenRing:
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);

							player->setTouchedRing(obj);

							player->ringJump(obj);
							break;
						case kGameObjectTypeModifier:
							switch (obj->getID())
							{
							case 201:
								changePlayerSpeed(0);
								break;
							case 200:
								changePlayerSpeed(1);
								break;
							case 202:
								changePlayerSpeed(2);
								break;
							case 203:
								changePlayerSpeed(3);
								break;
							case 1334:
								changePlayerSpeed(4);
								break;
							}
							break;
						case kGameObjectTypeSpecial:
						case kGameObjectTypeNormalMirrorPortal:
						case kGameObjectTypeInverseMirrorPortal:
							break;
						case kGameObjectTypeMiniSizePortal:
							obj->triggerActivated(player);
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);
							player->toggleMini(true);
							break;
						case kGameObjectTypeRegularSizePortal:
							obj->triggerActivated(player);
							player->setPortalP(obj->getPosition());
							player->setPortalObject(obj);
							player->toggleMini(false);
							break;
						default:
							player->collidedWithObject(dt, obj);
							break;
						}
					}
				}
			}
		}
	}
	for (unsigned int i = 0; i < m_pHazards.size(); ++i)
	{
		GameObject* hazard = m_pHazards[i];
		if (hazard->_radius > 0)
		{
			if (playerOuterBounds.intersectsCircle(hazard->getPosition() + Vec2(15, 15), hazard->_radius))
				destroyPlayer(player);
		}
		else if (playerOuterBounds.intersectsRect(hazard->getOuterBounds()))
		{
			destroyPlayer(player);
		}
	}
	m_pHazards.clear();

	if (player->_currentGamemode == PlayerGamemodeShip)
		player->_queuedHold = false;
}

void PlayLayer::onDrawImGui()
{
	extern bool _showDebugImgui;
	if (!_showDebugImgui)
		return;
	ImGui::SetNextWindowPos({1000.0f, 200.0f}, ImGuiCond_FirstUseEver);

	ImGui::Begin("PlayLayer Debug");

	ImGui::Text("%s", std::to_string(_player1->_queuedHold).c_str());

	ImGui::Checkbox("Freeze Player", &m_freezePlayer);
	ImGui::Checkbox("Platformer Mode (Basic)", &m_platformerMode);

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

	ImGui::Text("yVel %.3f", _player1->getYVel());

	ImGui::Checkbox("Show Hitboxes", &showDn);
	ImGui::Checkbox("Gain the power of invincibility", &noclip);

	if (ImGui::InputFloat("Speed", &gameSpeed))
		Director::getInstance()->getScheduler()->setTimeScale(gameSpeed);

	if (ImGui::InputFloat("FPS", &fps))
		Director::getInstance()->setAnimationInterval(1.0f / fps);

	ImGui::Text("Sections: %zu", m_pSectionObjects.size());
	if (m_pSectionObjects.size() > 0 && sectionForPos(_player1->getPositionX()) - 1 < m_pSectionObjects.size())
		ImGui::Text("Current Section Size: %zu", m_pSectionObjects[sectionForPos(_player1->getPositionX()) <= 0
																	  ? 0
																	  : sectionForPos(_player1->getPositionX()) - 1]
													.size());

	if (ImGui::Button("Reset"))
	{
		this->resetLevel();
	}
	ImGui::End();
}

void PlayLayer::resetLevel()
{
	_attempts++;
	auto dir = Director::getInstance();
	_player1->setPosition({2, 105});
	_player1->setRotation(0);
	_player1->setVisible(true);
	_player2->setPosition({2, 105});
	_player2->setRotation(0);
	_player2->setVisible(false);
	_player2->setActive(false);
	m_obCamPos.x = 0;
	m_obCamPos.y = 0;
	_bottomGround->setPositionX(0);
	_ceiling->setPositionX(0);
	_player1->reset();
	_player2->reset();
	m_pBG->setPositionX(dir->getWinSize().x / 2);
	_enterEffectID = 0;
	m_bEndAnimation = false;
	_isDualMode = false;
	_secondsSinceStart = 0;

	for (size_t i = 0; i < _groups.size(); i++)
		_groups[i]._alpha = 1.f;

	for (size_t i = 0; i < _groups.size(); i++)
		_groups[i]._alpha = 1.f;

	for (auto obj : this->_pObjects)
	{
		if (!obj)
			continue;
		if (obj->_isTrigger)
		{
			auto trigger = dynamic_cast<EffectGameObject*>(obj);
			trigger->_wasTriggerActivated = false;
		}
		obj->_hasBeenActivatedP1 = false;
		obj->_hasBeenActivatedP2 = false;
		obj->_effectOpacityMultipler = 1.f;
		obj->setActive(false);
		if (obj->getParent() != nullptr)
		{
			if (obj->_particle)
			{
				AX_SAFE_RETAIN(obj->_particle);
				removeChild(obj->_particle, true);
			}
			if (obj->_glowSprite)
			{
				AX_SAFE_RETAIN(obj->_glowSprite);
				_glowBatchNode->removeChild(obj->_glowSprite, true);
			}
			AX_SAFE_RETAIN(obj);
			//_mainBatchNode->removeChild(obj, true);
			if (isObjectBlending(obj))
			{
				switch (obj->_zLayer)
				{
				case -3:
					_blendingBatchNodeB4->removeChild(obj, true);
					break;
				case -1:
					_blendingBatchNodeB3->removeChild(obj, true);
					break;
				case 1:
					_blendingBatchNodeB2->removeChild(obj, true);
					break;
				case 3:
					_blendingBatchNodeB1->removeChild(obj, true);
					break;
				default:
				case 5:
					_blendingBatchNodeT1->removeChild(obj, true);
					break;
				case 7:
					_blendingBatchNodeT2->removeChild(obj, true);
					break;
				case 9:
					_blendingBatchNodeT3->removeChild(obj, true);
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
						_mainBatchNodeB4->removeChild(obj, true);
						break;
					case -1:
						_mainBatchNodeB3->removeChild(obj, true);
						break;
					case 1:
						_mainBatchNodeB2->removeChild(obj, true);
						break;
					case 3:
						_mainBatchNodeB1->removeChild(obj, true);
						break;
					default:
					case 5:
						_mainBatchNodeT1->removeChild(obj, true);
						break;
					case 7:
						_mainBatchNodeT2->removeChild(obj, true);
						break;
					case 9:
						_mainBatchNodeT3->removeChild(obj, true);
						break;
					}
				}
				else if (obj->_texturePath == _main2BatchNodeTexture)
					_main2BatchNode->removeChild(obj, true);
			}
		}
	}

	dir->getActionManager()->removeAllActions();

	m_pColorChannels = std::unordered_map<int, SpriteColor, my_string_hash>(_originalColors);

	_prevSection = -1;
	_nextSection = -1;

	if (this->m_pColorChannels.contains(1000))
		this->m_pBG->setColor(this->m_pColorChannels.at(1000)._color);
	this->_bottomGround->update(0);
	this->_ceiling->update(0);

	AudioEngine::stopAll();
	AudioEngine::setCurrentTime(AudioEngine::play2d(LevelTools::getAudioFilename(getLevel()->_musicID), false, 0.1f),
								_levelSettings.songOffset);

	changeGameMode(_player1, _player1, _levelSettings.gamemode);
	changeGameMode(_player1, _player2, _levelSettings.gamemode);
	_player1->toggleMini(_levelSettings.mini);
	_player2->toggleMini(_levelSettings.mini);
	changePlayerSpeed(_levelSettings.speed);
	m_obCamPos.y = m_fCameraYCenter;
	_isDualMode = _levelSettings.dual;
	if (_isDualMode)
	{
		// toggle dual
	}
	scheduleUpdate();
}

void PlayLayer::renderRect(ax::Rect rect, ax::Color4B col)
{
	dn->drawRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()}, col);
	dn->drawSolidRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()},
					  Color4B(col.r, col.g, col.b, 100));
}

void PlayLayer::onEnter()
{
	Layer::onEnter();

	auto listener = EventListenerKeyboard::create();
	auto dir = Director::getInstance();

	listener->onKeyPressed = AX_CALLBACK_2(PlayLayer::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(PlayLayer::onKeyReleased, this);
	dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto current = dir->getRunningScene();
#if SHOW_IMGUI == true
	ImGuiPresenter::getInstance()->addRenderLoop("#playlayer", AX_CALLBACK_0(PlayLayer::onDrawImGui, this), current);
#endif
}

void PlayLayer::onExit()
{

#if SHOW_IMGUI == true
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
	ImGuiPresenter::getInstance()->removeRenderLoop("#playlayer");
#endif
	LevelPage::replacingScene = false;
	Layer::onExit();
}

void PlayLayer::exit()
{

	_player1->deactivateStreak();
	_player2->deactivateStreak();
	unscheduleAllCallbacks();
	_player1->unscheduleAllCallbacks();
	_player2->unscheduleAllCallbacks();

	int size = _pObjects.size();
	for (int i = 0; i < size; i++)
	{
		GameObject* obj = _pObjects.at(i);
		if (obj && !obj->getParent())
		{
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

	Instance = nullptr;

	AudioEngine::stopAll();
	AudioEngine::play2d("quitSound_01.ogg", false, 0.1f);
	AudioEngine::play2d("menuLoop.mp3", true, 0.2f);

	auto id = getLevel()->_levelID;
	if (id <= 0 || id > 22)
		return GameToolbox::popSceneWithTransition(0.5f);

	Director::getInstance()->replaceScene(
		TransitionFade::create(0.5f, LevelSelectLayer::scene(getLevel()->_levelID - 1)));
}

void PlayLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	GameToolbox::log("Key with keycode {} pressed", static_cast<int>(keyCode));
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_R: {
		resetLevel();
	}
	break;
	case EventKeyboard::KeyCode::KEY_F: {
		extern bool _showDebugImgui;
		_showDebugImgui = !_showDebugImgui;
	}
	break;
	case EventKeyboard::KeyCode::KEY_SPACE: {
		if (!_player1->m_bIsHolding)
			_player1->pushButton();
		if (_isDualMode && !_player2->m_bIsHolding)
			_player2->pushButton();
	}
	break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW: {
		if (!_player1->m_bIsHolding)
			_player1->pushButton();
		if (_isDualMode && !_player2->m_bIsHolding)
			_player2->pushButton();
	}
	break;
		break;
	case EventKeyboard::KeyCode::KEY_BACK: {
		_player1->releaseButton();
		if (_isDualMode)
			_player2->releaseButton();
		this->exit();
	}
	default:
		break;
	}
	if (keyCode == EventKeyboard::KeyCode::KEY_A && _player1->m_bIsPlatformer)
		_player1->direction = -1.f;
	else if (keyCode == EventKeyboard::KeyCode::KEY_D && _player1->m_bIsPlatformer)
		_player1->direction = 1.f;
}

void PlayLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	GameToolbox::log("Key with keycode {} released", static_cast<int>(keyCode));
	if ((keyCode == EventKeyboard::KeyCode::KEY_A && _player1->direction == -1.f) ||
		(keyCode == EventKeyboard::KeyCode::KEY_D && _player1->direction == 1.f) && _player1->m_bIsPlatformer)
		_player1->direction = 0.f;
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_SPACE: {
		if (_player1->m_bIsHolding)
			_player1->releaseButton();
		if (_isDualMode && _player2->m_bIsHolding)
			_player2->releaseButton();
	}
	break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW: {
		if (_player1->m_bIsHolding)
			_player1->releaseButton();
		if (_isDualMode && _player2->m_bIsHolding)
			_player2->releaseButton();
	}
	default:
		break;
	}
}

void PlayLayer::tweenBottomGround(float y)
{
	_bottomGround->runAction(EaseInOut::create(ActionTween::create(0.1f, "y", _bottomGround->getPositionY(), y), 2.f));
	//_bottomGround->setPositionY(y);
}

void PlayLayer::tweenCeiling(float y)
{
	_ceiling->runAction(EaseInOut::create(ActionTween::create(0.1f, "y", _ceiling->getPositionY(), y), 2.f));
	//_ceiling->setPositionY(y);
}

void PlayLayer::changePlayerSpeed(int speed)
{
	switch (speed)
	{
	case 0:
		_player1->m_dXVel = 5.77;
		_player1->setPlayerSpeed(0.9);
		_player2->m_dXVel = 5.77;
		_player2->setPlayerSpeed(0.9);
		break;
	case 1:
		_player1->m_dXVel = 5.98;
		_player1->setPlayerSpeed(0.7);
		_player2->m_dXVel = 5.98;
		_player2->setPlayerSpeed(0.7);
		break;
	case 2:
		_player1->m_dXVel = 5.87;
		_player1->setPlayerSpeed(1.1);
		_player2->m_dXVel = 5.87;
		_player2->setPlayerSpeed(1.1);
		break;
	case 3:
		_player1->m_dXVel = 6;
		_player1->setPlayerSpeed(1.3);
		_player2->m_dXVel = 6;
		_player2->setPlayerSpeed(1.3);
		break;
	case 4:
		_player1->m_dXVel = 6;
		_player1->setPlayerSpeed(1.6);
		_player2->m_dXVel = 6;
		_player2->setPlayerSpeed(1.6);
		break;
	}
}

void PlayLayer::changeGravity(bool gravityFlipped)
{
	_player1->flipGravity(gravityFlipped);
	if (_isDualMode)
		_player2->flipGravity(!gravityFlipped);
}

PlayLayer* PlayLayer::getInstance()
{
	return Instance;
}
