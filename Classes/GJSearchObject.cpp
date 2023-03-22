#include "GJSearchObject.h"

GJSearchObject* GJSearchObject::create()
{
    GJSearchObject* ret = new GJSearchObject();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}