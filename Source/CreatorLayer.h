#pragma once
#include "2d/CCScene.h"
#include "CCEventKeyboard.h"

namespace ax 
{ 
	class Event;
	namespace ui 
	{ 
		class TextField; 
	} 
}

class CreatorLayer : public ax::Scene {
public:
	static ax::Scene* scene();
	static CreatorLayer* create();
	bool init();
	
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

private:
	ax::ui::TextField* _levelField;
};
