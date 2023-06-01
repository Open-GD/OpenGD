#include "getGJRewards.h"

#include "external/base64.h"
#include "GameToolbox.h"

getGJRewards* getGJRewards::create() 
{
	getGJRewards* rewards = new getGJRewards;
	return rewards;
}
