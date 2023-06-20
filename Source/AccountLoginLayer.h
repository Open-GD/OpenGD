#pragma once

#include "PopupLayer.h"

class LoadingCircle;

namespace ax 
{ 
	namespace network 
	{ 
		class HttpRequest; 
		class HttpClient;
		class HttpResponse;
	} 
}


class AccountLoginLayer : public PopupLayer
{
public:
	LoadingCircle* _loadingcircle;
	static AccountLoginLayer* create();

	bool init();
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};
