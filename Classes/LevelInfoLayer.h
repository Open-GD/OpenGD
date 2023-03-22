#pragma once

#include <axmol.h>
#include "GJGameLevel.h"
#include <network/HttpClient.h>

class LevelInfoLayer : public ax::Layer
{
public:
	static LevelInfoLayer* create(std::vector<std::string> levelinfo);

	static ax::Scene* scene(std::vector<std::string> levelinfo);
	bool init(std::vector<std::string> levelinfo);
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};
