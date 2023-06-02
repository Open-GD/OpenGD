#include "CurrencyRewardLayer.h"

CurrencyRewardLayer* CurrencyRewardLayer::create()
{
    auto pRet = new (std::nothrow) CurrencyRewardLayer();

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return pRet;
}

bool CurrencyRewardLayer::init()
{
	
    return true;
}