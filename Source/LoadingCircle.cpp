/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "LoadingCircle.h"

#include "GameToolbox/log.h"
#include "GameToolbox/getTextureString.h"
#include "GameToolbox/conv.h"

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
	if(m_pCircle) 
	{
		float rot = m_pCircle->getRotation();
		rot += delta * 60 * 4;
		if (rot > 360) 
		{
	        	rot -= 360;
	        }
		m_pCircle->setRotation(rot);
	}
}
void LoadingCircle::removeMeAndCleanup()
{
	m_pCircle->removeFromParentAndCleanup(true);
	m_pCircle = nullptr;

	removeFromParentAndCleanup(true);
}
