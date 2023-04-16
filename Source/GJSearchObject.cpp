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

GJSearchObject* GJSearchObject::create(SearchType type)
{
	GJSearchObject* ret = GJSearchObject::create(); 
	if (ret) {
		ret->_screenID = type;
	}
	return ret;
}

GJSearchObject* GJSearchObject::create(std::string_view query)
{
	GJSearchObject* ret = GJSearchObject::create();
	if (ret) {
		ret->_searchQuery = {query.begin(), query.end()};
	}
	
	return ret;
}