#pragma once

#include <axmol.h>
#include "DropDownLayer.h"
#include <ui/CocosGUI.h>

class SongsLayer : public DropDownLayer
{
private:
	ax::ui::ListView* _listView = ax::ui::ListView::create();
public:
	static SongsLayer* create();
	void customSetup() override;
};
