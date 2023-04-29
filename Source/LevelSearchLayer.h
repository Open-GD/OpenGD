#pragma once

#include <vector>

#include "2d/CCLayer.h"

class GJSearchObject;
class TextInputNode;

namespace ax 
{ 
	class Ref; 
	class Node;
	class Menu;
	class Scene;
}

class LevelSearchLayer : public ax::Layer {
private:
	void toggleDifficulty(ax::Ref*);
	void toggleTime(ax::Ref*);
	void onSearchProfile(ax::Node*);
	void onSearch(ax::Node*);

	static ax::Menu* _difficultyMenu;
	static ax::Menu* _lengthFilter;

	GJSearchObject* _searchObject;
	ax::Node* _extraDemonsBtn;
	ax::Node* _starIcon;

public:
	TextInputNode* _searchField;

	static std::vector<int> _selectedDifficulties;
	static std::vector<int> _selectedTimes;

	CREATE_FUNC(LevelSearchLayer);
	static ax::Scene* scene();

	bool init();
};
