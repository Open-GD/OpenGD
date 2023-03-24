#include "LoadingCircle.h"
#include "GameToolbox.h"

bool LoadingCircle::init() {
	m_pCircle = ax::Sprite::create(GameToolbox::getTextureString("loadingCircle.png"));
	m_pCircle->setBlendFunc(GameToolbox::getBlending());

	if(!m_pCircle) {
		GameToolbox::log("WARN: loadingCircle is not present in your gd textures!");
	} else {
		this->addChild(m_pCircle);
	}

	scheduleUpdate();

	return true;
}
void LoadingCircle::update(float delta) {
	if(m_pCircle) {
		float rot = m_pCircle->getRotation();
		rot += delta * 60 * 4;
		m_pCircle->setRotation(rot);
	}
}
void LoadingCircle::removeMeAndCleanup() {
	m_pCircle->removeFromParentAndCleanup(true);
	m_pCircle = nullptr;

	removeFromParentAndCleanup(true);
}