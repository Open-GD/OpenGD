#include "GJSearchObject.h"

GJSearchObject* GJSearchObject::create()
{
	GJSearchObject* ret = new GJSearchObject();
	if (ret->init()) 
	{
		ret->autorelease();
		return ret;
	} else {
		delete ret;
		ret = nullptr;
		return nullptr;
	}
}

GJSearchObject* GJSearchObject::create(std::string query)
{
	auto ret = GJSearchObject::create();
	if(!ret)
	{
		return nullptr;
	}

	ret->_searchQuery = query;

	return ret;
}