#pragma once

#include <axmol.h>
#include "TextInputNode.h"

class LevelSearchLayer : public ax::Layer
{
	static void toggleDifficulty(ax::Node*);
	static void toggleTime(ax::Ref*);

	static ax::Menu* m_pDifficultyFilterMenu;
	static ax::Menu* m_pLengthFilterMenu;
public:
	static TextInputNode* m_pTextField;

	static std::vector<int> m_dSelectedDifficulties;
	static std::vector<int> m_dSelectedTimes;

	CREATE_FUNC(LevelSearchLayer);
	static ax::Scene* scene();

	bool init();
};
