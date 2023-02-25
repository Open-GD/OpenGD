#include "GameObject.h"
#include <fmt/format.h>

USING_NS_AX;

bool GameObject::init(std::string_view frame) {
    // so ObjectManager is supposed to do some shit here
    // but there's not even any purpose for it to even exist

    // so this part is empty

    // avoid player lol
    if(frame.find("player") != std::string::npos) return true;
    if(!Sprite::initWithSpriteFrameName(frame)) return false;

    return true;
}

GameObject* GameObject::objectFromString(std::string str) {
    auto values = stringSetupToDict(str);

    if(values.count("1")) return nullptr; // you obviously can't do anything if there's no object ID

    int ID = stoi(values["1"]);
    auto frame = keyToFrame(ID);

    auto obj = createObject(frame);

    return obj;
}

GameObject* GameObject::createObject(std::string_view frame) {
    // if RingObject
    if(frame.find("ring_01_001.png") != std::string::npos) {
        return GameObject::create(frame);
    }
    else {
        return nullptr;
    }
}

GameObject* GameObject::create(std::string_view frame) {
    auto pRet = new(std::nothrow) GameObject();

    if(pRet && pRet->init(frame)) {
        pRet->autorelease();
        return pRet;
    }
    AX_SAFE_DELETE(pRet);
    return nullptr;
}

std::string GameObject::keyToFrame(int key) {
    return key > 0 && key < 8 ? fmt::format("square_{}_001.png", key) : "";
}

// couldn't understand jack shit from the original
// so I recreated from 2.1's GameToolbox::stringSetupToMap
std::map <std::string, std::string> GameObject::stringSetupToDict(std::string str) {
    size_t index = 0;
    size_t length = str.length();

    std::map<std::string, std::string> output;
    
    size_t currentComma = str.find(',');
    while (true)
    {
        auto key = str.substr(index, currentComma - index);

        if(currentComma == str.npos) break;

        // find new comma
        index = currentComma + 1;
        currentComma = str.find(',', index);

        // set values
        output.insert(std::make_pair(key, str.substr(index, currentComma - index)));

        if(currentComma == str.npos) break;

        // find new comma (again)
        index = currentComma + 1;
        currentComma = str.find(',', index);
    }

    return output;
}