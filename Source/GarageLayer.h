#pragma once
#include <array>

#include "2d/CCScene.h"

enum IconType;
class SimplePlayer;

namespace ax 
{ 
	class Menu;
	class Sprite;

	namespace ui 
	{ 
		class TextField; 
	}
}


class GarageLayer : public ax::Scene {
public:
	static ax::Scene* scene(bool popSceneWithTransition = false);
	static GarageLayer* create();
	bool init();
	void setupIconSelect();
	const char* getSpriteName(int id, bool actived);
	void setupPage(IconType mode, int page);
	void createStat(const char* sprite, const char* statKey);
	int selectedGameModeInt();

private:
	bool _popSceneWithTransition;
	SimplePlayer* _iconPrev;
	ax::ui::TextField* _userNameField;
	ax::Menu* _menuIcons;
	ax::Sprite* _selectSprite;
	int _numPerRow = 12;
	int _numPerColumn = 3;
	int _stats = 0;
	IconType _selectedMode;
	std::array<int, 8> _modePages{0};
};
