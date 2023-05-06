#pragma once
#include "2d/CCLayer.h"

class LoadingCircle;
class MenuItemSpriteExtra;
class GJGameLevel;

namespace ax 
{ 
	class Scene;
	namespace network 
	{ 
		class HttpRequest; 
		class HttpClient;
		class HttpResponse;
	} 
}


class LevelInfoLayer : public ax::Layer
{
private:
	LoadingCircle* loading;
	MenuItemSpriteExtra* playBtn;
	GJGameLevel* _level;
	ax::network::HttpRequest* _request;
public:
	static LevelInfoLayer* create(GJGameLevel* level);

	static ax::Scene* scene(GJGameLevel* level);
	bool init(GJGameLevel* level);

	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);

	void onDownloadFailed();
};
