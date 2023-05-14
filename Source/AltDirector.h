#pragma once
#include "base/CCDirector.h"

	
class AltDirector : public ax::Director { 
public:
	static AltDirector* getInstance() {
		return (AltDirector*)ax::Director::getInstance();
	}

	// Director have this variables in "protected", so i did this class to use them
	ax::Vector<ax::Scene*> getScenesStack() {
		return Director::_scenesStack;
	}
	void setSendCleanupToScene(bool cleanupToScene) {
		ax::Director::_sendCleanupToScene = cleanupToScene;
	}
};