#pragma once
#include <axmol.h>

USING_NS_AX;

class AltDirector : public Director { 
public:
	static AltDirector* getInstance() {
		return (AltDirector*)Director::getInstance();
	}

	// Director have this variables in "protected", so i did this class to use them
	Vector<Scene*> getScenesStack() {
		return Director::_scenesStack;
	}
	void setSendCleanupToScene(bool cleanupToScene) {
		Director::_sendCleanupToScene = cleanupToScene;
	}
};