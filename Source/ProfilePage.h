#pragma once

#include "PopupLayer.h"
#include "GJUserScore.h"

class ProfilePage;

namespace ax 
{ 
	class Node; 
}


class ProfilePage : public PopupLayer
{
public:
	static ProfilePage* create(int accountID, bool mainMenuProfile);

	bool init(int accountID, bool mainMenuProfile);
	void loadPageFromUserInfo(GJUserScore* score);
};