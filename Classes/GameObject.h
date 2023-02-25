#pragma once
#include <axmol.h>
#include <map>
#include <string>

class GameObject : public ax::Sprite {
public:
    static GameObject* create(std::string_view frame);
    static GameObject* createObject(std::string_view frame);
    static GameObject* objectFromString(std::string);
    bool init(std::string_view frame);

    static std::string keyToFrame(int key);
    static std::map<std::string, std::string> stringSetupToDict(std::string);
};