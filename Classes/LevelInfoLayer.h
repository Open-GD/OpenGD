#pragma once

#include <axmol.h>
#include "GJGameLevel.h"
#include "network/HttpClient.h"
#include "LoadingCircle.h"
#include "MenuItemSpriteExtra.h"

class LevelInfoLayer : public ax::Layer
{
private:
	LoadingCircle* loading;
	MenuItemSpriteExtra* playBtn;
	GJGameLevel* _level;
public:
	static LevelInfoLayer* create(GJGameLevel* level);

	static ax::Scene* scene(GJGameLevel* level);
	bool init(GJGameLevel* level);

	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);

	void onDownloadFailed();
};
