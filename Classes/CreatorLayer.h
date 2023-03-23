#pragma once
#include <axmol.h>
#include <network/HttpClient.h>
#include <ui/CocosGUI.h>

class CreatorLayer : public ax::Scene {
public:
	static ax::Scene* scene();
	static CreatorLayer* create();
	bool init();
	
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);

private:
	bool _ctrlDown;
	ax::ui::TextField* _levelField;
};
