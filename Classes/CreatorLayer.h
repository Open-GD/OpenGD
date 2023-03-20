#pragma once
#include <axmol.h>
#include <network/HttpClient.h>
#include <ui/CocosGUI.h>

class CreatorLayer : public ax::Scene {
public:
	bool _ctrl_down;
	
	ax::ui::TextField* levelField;
	CREATE_FUNC(CreatorLayer)
	bool init();
	static ax::Scene* scene();
	void onDrawImGui();
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
};