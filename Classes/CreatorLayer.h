#pragma once
#include <axmol.h>
#include <network/HttpClient.h>
#include <ui/CocosGUI.h>

class CreatorLayer : public ax::Scene {
public:
	ax::ui::TextField * levelField;
	CREATE_FUNC(CreatorLayer)
	bool init();
	static ax::Scene* scene();
	void onDrawImGui();
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};