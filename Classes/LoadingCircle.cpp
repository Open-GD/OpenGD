#include "LoadingCircle.h"
#include "GameToolbox.h"

LoadingCircle *LoadingCircle::create()
{ 
	LoadingCircle* pRet = new LoadingCircle(); 
	if (pRet->init()) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} else { 
		delete pRet; 
		pRet = nullptr; 
		return nullptr; 
	} 
}

bool LoadingCircle::init()
{
	m_pCircle = ax::Sprite::create(GameToolbox::getTextureString("loadingCircle.png"));

	if(!m_pCircle)
	{
		GameToolbox::log("WARN: loadingCircle is not present in your gd textures!");
	} else {
		m_pCircle->setBlendFunc(GameToolbox::getBlending());
		this->addChild(m_pCircle);
	}

	scheduleUpdate();

	return true;
}
void LoadingCircle::update(float delta) 
{
	if(m_pCircle) {
		float rot = m_pCircle->getRotation();
		rot += delta * 60 * 4;
		m_pCircle->setRotation(rot);
	}
}
void LoadingCircle::removeMeAndCleanup()
{
	m_pCircle->removeFromParentAndCleanup(true);
	m_pCircle = nullptr;

	removeFromParentAndCleanup(true);
}