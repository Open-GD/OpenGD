#pragma once

enum IconType {
	kIconTypeCube		= 0,
	kIconTypeShip		= 1,
	kIconTypeBall		= 2,
	kIconTypeUfo		 = 3,
	kIconTypeWave		= 4,
	kIconTypeRobot	   = 5,
	kIconTypeSpider	  = 6,
	kIconTypeSwing	   = 7,
	kIconTypeDeathEffect = 98,
	kIconTypeSpecial	 = 99,
};

enum popTransition {
	kTransitionFade = 0,
	kTransitionShop
};

namespace GameToolbox
{
	enum GTRes
	{
		Low = 0,
		Medium,
		High
	};
}