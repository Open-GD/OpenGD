#pragma once
#include <axmol.h>
#include <map>
#include <string>

enum GameObjectType {
    kObjectTypePlayer = 0,
    kObjectTypeTile = 1,
    kObjectTypeHazard = 2
};

class GameObject : public ax::Sprite {
private:
    ax::Rect _pOuterBounds;
    ax::Rect _pInnerBounds;

    GameObjectType _pObjectType;

public:
    static GameObject* create(std::string_view frame);
    static GameObject* createObject(std::string_view frame);
    static GameObject* objectFromString(std::string);
    bool init(std::string_view frame);

    static std::string keyToFrame(int key);
    static std::map<std::string, std::string> stringSetupToDict(std::string);

    void setOuterBounds(const ax::Rect& value) { _pOuterBounds = value; }
    ax::Rect getOuterBounds() { return _pOuterBounds; }

    void setInnerBounds(const ax::Rect& value) { _pInnerBounds = value; }
    ax::Rect getInnerBounds() { return _pInnerBounds; }

    GameObjectType getGameObjectType() { return _pObjectType; }
    void setGameObjectType(const GameObjectType& value) { _pObjectType = value; }
};