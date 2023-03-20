#include "CreatorLayer.h"
#include "GameToolbox.h"
#include "LevelSearchLayer.h"
#include "MenuLayer.h"
#include "MenuItemSpriteExtra.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "PlayLayer.h"
#include "LevelEditorLayer.h"
#include "GJGameLevel.h"
#include <AudioEngine.h>

USING_NS_AX;
using namespace ax::network;

#include "base64.h"
#include <LevelPage.h>

using GameToolbox::getTextureString;

Scene* CreatorLayer::scene() 
{
	return CreatorLayer::create();
}

bool CreatorLayer::init() {
	if (!Scene::init()) return false;
	
  //  ImGuiPresenter::getInstance()->setViewResolution(1280, 720);
   // ImGuiPresenter::getInstance()->addRenderLoop("#test", AX_CALLBACK_0(CreatorLayer::onDrawImGui, this), this);

	auto winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create(getTextureString("GJ_gradientBG.png"));
	bg->setStretchEnabled(false);
	bg->setScaleX(winSize.width / bg->getContentSize().width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setAnchorPoint({0, 0});
	bg->setColor({0, 102, 255});
	this->addChild(bg);

	GameToolbox::createAllCorners(this);

	auto createBtnSpr = Sprite::createWithSpriteFrameName("GJ_createBtn_001.png");
	auto savedBtnSpr = Sprite::createWithSpriteFrameName("GJ_savedBtn_001.png");
	auto featuredBtnSpr = Sprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
	auto searchBtnSpr = Sprite::createWithSpriteFrameName("GJ_searchBtn_001.png");

	auto savedBtn = MenuItemSpriteExtra::create(savedBtnSpr, [&](Node*) {
		GameToolbox::log("on saved");
	});
	auto featuredBtn = MenuItemSpriteExtra::create(featuredBtnSpr, [&](Node*) {
		GameToolbox::log("on featured");
	});
	auto searchBtn = MenuItemSpriteExtra::create(searchBtnSpr, [&](Node*) {
		
	//	Director::getInstance()->replaceScene(TransitionFade::create(.5, LevelSearchLayer::scene()));
	
		
		std::string levelID(levelField->getString());
		std::string postData = fmt::format("levelID={}&secret=Wmfd2893gb7", levelID);
		GameToolbox::log("postData: {}", postData);
		HttpRequest* request = new HttpRequest();
		request->setTag(levelID);
		request->setUrl("http://www.boomlings.com/database/downloadGJLevel22.php");
		request->setRequestType(HttpRequest::Type::POST);
        request->setHeaders(std::vector<std::string>{"User-Agent: "});
		request->setRequestData(postData.c_str(), postData.length());
		request->setResponseCallback(AX_CALLBACK_2(CreatorLayer::onHttpRequestCompleted, this));
		request->setTag("GET test3");
		HttpClient::getInstance()->send(request);
		request->release();
		
	});
	auto createBtn = MenuItemSpriteExtra::create(createBtnSpr, [&](Node*) {
		auto scene = LevelEditorLayer::scene(GJGameLevel::createWithMinimumData("cool level", "partur", 5));
		Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
	});

	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [&](Node*) {
		Director::getInstance()->replaceScene(TransitionFade::create(.5, MenuLayer::scene()));
	});

	auto menu = Menu::create(createBtn, savedBtn, featuredBtn, searchBtn, nullptr);
	GameToolbox::alignItemsInColumnsWithPadding(menu, 2, 130, 130);
	menu->addChild(backBtn);
	backBtn->setPosition(menu->convertToNodeSpace({25, winSize.height - 25}));
	
	this->addChild(menu);
	
	levelField = ui::TextField::create("Enter ID", GameToolbox::getTextureString("bigFont.fnt"), 20);
	levelField->setPlaceHolderColor({ 120, 170, 240 });
	levelField->setMaxLength(10);
	levelField->setMaxLengthEnabled(true);
	levelField->setCursorEnabled(true);
	levelField->setString("128");
	levelField->setPosition({winSize.width / 2 + 150, winSize.height / 2});
	this->addChild(levelField);
	
	//auto path = FileUtils::getInstance()->getWritablePath();
	//GameToolbox::log("path: {}", path);
	
	#ifdef AX_PLATFORM_PC
	{
		auto listener = EventListenerKeyboard::create();
		auto dir = Director::getInstance();

		listener->onKeyPressed = AX_CALLBACK_2(CreatorLayer::onKeyPressed, this);
		listener->onKeyReleased = AX_CALLBACK_2(CreatorLayer::onKeyReleased, this);
		dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
	#endif
	
	return true;
}


void CreatorLayer::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_CTRL: _ctrl_down = true; break;
		
		case EventKeyboard::KeyCode::KEY_V:
		{
			if(_ctrl_down)
			{
				GameToolbox::log("paste");
				levelField->setString(GameToolbox::getClipboardString());
			}
		}
			
	}
}
void CreatorLayer::onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_CTRL: _ctrl_down = false; break;
	}
}
	
void CreatorLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	if(auto str = GameToolbox::getResponse(response))
	{
		//validate by checking if level ID key/value is set
		if((*str).find(fmt::format("1:{}", levelField->getString())) == std::string::npos)
			return GameToolbox::log("invalid response: {}", *str);
				
		//std::vector<std::string> levelStuff = GameToolbox::splitByDelim(*str, ':');
		//std::string compressedStr = fmt::format("{}", levelStuff.at(7));
		//GameToolbox::log("compressed: {}", compressedStr);
		GJGameLevel* level = GJGameLevel::createWithResponse(*str);
		GameToolbox::log("playing level<{}>:{},{}*", level->_LevelID, level->_LevelName, level->_Stars);

		ax::AudioEngine::stopAll();
		ax::AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
		ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));
		LevelPage::replacingScene = true;
	}
	else
		GameToolbox::log("request failed");
}

/*
static bool show_test_window	= true;
static bool show_another_window = true;
static ImVec4 clear_color	   = ImColor(114, 144, 154);

void CreatorLayer::onDrawImGui()
{
	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if (ImGui::Button("Test Window"))
			show_test_window ^= 1;
		if (ImGui::Button("Another Window"))
			show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
					ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(170, 80), ImGuiCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);

		ImGui::Text("Hello");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::ShowDemoWindow();
	}
}
*/