#pragma once
#include "CCEventListenerKeyboard.h"
#include "CCEventKeyboard.h"
#include "2d/CCNode.h"

namespace GameToolbox
{
	void onKeyDown(bool equalZOrder, ax::Node*, const std::function<void(ax::EventKeyboard::KeyCode, ax::Event*)>& callback);
}
