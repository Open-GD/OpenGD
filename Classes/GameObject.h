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
	// opengd additions //
	kGameObjectTypeSpecial = 40
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

	int _pColorRed;
	int _pColorGreen;
	int _pColorBlue;
	float _pDuration;

	ax::Sprite* _glowSprite;

public:
	int _enterEffectID = 1;

	void setEnterEffectID(int id) { _enterEffectID = id; }
	int getEnterEffectID() { return _enterEffectID; }

	bool m_bHasBeenActivated;
	int _mainColorChannel = -1, _secColorChannel = -1;

	int _uniqueID = -1;

	ax::ParticleSystemQuad* _particle;

	virtual ~GameObject();

	static const std::map<int, Hitbox> _pHitboxes;
	static const std::map<int, float> _pHitboxRadius;
	// from https://gist.github.com/absoIute/c8fa23c9b2cb39252755465345bc6e35
	static const std::map<int, std::vector<const char *>> _pBlocks;

	static const std::vector<int> _pSolids, _pTriggers;

	static GameObject *create(std::string_view frame, std::string_view glowFrame = "");
	static GameObject *createObject(std::string_view frame, std::string_view glowFrame = "");
	bool init(std::string_view frame, std::string_view glowFrame = "");

	void updateObjectType();

	void customSetup();
	void setupColors();

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
	void triggerActivated();

	void update();

	int getColorRed() { return _pColorRed; }
	void setColorRed(int col) { _pColorRed = col; }
	int getColorGreen() { return _pColorGreen; }
	void setColorGreen(int col) { _pColorGreen = col; }
	int getColorBlue() { return _pColorBlue; }
	void setColorBlue(int col) { _pColorBlue = col; }
	float getDuration() { return _pDuration; }
	void setDuration(float dura) { _pDuration = dura; }

	void createAndAddParticle(const char* particle, int a3);

	// AX_SYNTESIZE(int, _pColorRed, ColorRed); // does not work
	// AX_SYNTESIZE(int, _pColorGreen, ColorGreen);
	// AX_SYNTESIZE(int, _pColorBlue, ColorBlue);
};