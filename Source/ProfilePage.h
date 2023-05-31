#pragma once

#include "PopupLayer.h"
#include "GJUserScore.h"

class LoadingCircle;
class ProfilePage;

namespace ax 
{ 
	class Node;
	class Scene;
	class Label;
	namespace network 
	{ 
		class HttpRequest; 
		class HttpClient;
		class HttpResponse;
	} 
}


class ProfilePage : public PopupLayer
{
private:
	int _accountID;
	LoadingCircle* _loadingcircle;
	ax::Label* _errorMsg;
public:
	static ProfilePage* create(int accountID, bool mainMenuProfile);

	bool init(int accountID, bool mainMenuProfile);
	void loadPageFromUserInfo(GJUserScore* score);
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};
