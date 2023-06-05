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

class SecretLayer2 : public ax::Scene {
public:
	static ax::Scene* scene();
	static SecretLayer2* create();
	bool init();
	
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
};
