#pragma once
#include <axmol.h>
#include <map>
#include <string>

enum GameObjectType
{
    kGameObjectTypePlayer = 0,
    kGameObjectTypeSolid = 1,
    kGameObjectTypeHazard = 2,
    kGameObjectTypeInverseGravityPortal = 3,
    kGameObjectTypeNormalGravityPortal = 4,
    kGameObjectTypeShipPortal = 5,
    kGameObjectTypeCubePortal = 6,
    kGameObjectTypeDecoration = 7,
    kGameObjectTypeYellowJumpPad = 8,
    kGameObjectTypeYellowJumpRing = 11,
};

struct Hitbox
{
    float h, w, x, y;
};

class GameObject : public ax::Sprite
{
private:
    ax::Rect _pOuterBounds;
    ax::Rect _pInnerBounds;

    int m_pId;

    bool m_bActive;

    GameObjectType _pObjectType;

public:
    static const std::map<int, Hitbox> _pHitboxes;
    static const std::map<int, float> _pHitboxRadius;
    // from https://gist.github.com/absoIute/c8fa23c9b2cb39252755465345bc6e35
    static const std::map<int, const char *> _pBlocks;

    static const std::vector<int> _pSolids;

    static GameObject *create(std::string_view frame);
    static GameObject *createObject(std::string_view frame);
    static GameObject *objectFromString(std::string);
    bool init(std::string_view frame);

    static std::string keyToFrame(int key);
    static std::map<std::string, std::string> stringSetupToDict(std::string);

    void setOuterBounds(const ax::Rect &value) { _pOuterBounds = value; }
    ax::Rect getOuterBounds() { return _pOuterBounds; }

    void setInnerBounds(const ax::Rect &value) { _pInnerBounds = value; }
    ax::Rect getInnerBounds() { return _pInnerBounds; }

    GameObjectType getGameObjectType() { return _pObjectType; }
    void setGameObjectType(const GameObjectType &value) { _pObjectType = value; }

    int getID() { return m_pId; }
    void setID(int id) { m_pId = id; }

    void setActive(bool active) { m_bActive = active; }

    bool isActive() { return m_bActive; }
};