#pragma once
#include "DropDownLayer.h"
#include "ui/UIListView.h"



class SongsLayer : public DropDownLayer
{
private:
	ax::ui::ListView* _listView = ax::ui::ListView::create();
public:
	static SongsLayer* create();
	void customSetup() override;
};
