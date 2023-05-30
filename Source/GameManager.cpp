#include "GameManager.h"
#include "external/hps/hps.h"
#include "GameToolbox.h"
#include "ResourcesLoadingLayer.h"
#include "AppDelegate.h"

#include <platform/CCFileUtils.h>

struct GameOptions
{
	std::map<std::string, int> intOptions;
	std::map<std::string, bool> boolOptions;
	std::map<std::string, std::string> stringOptions;
	
	template <class B>
	void serialize(B& buf) const
	{
		buf << intOptions << boolOptions << stringOptions;
	}

	template <class B>
	void parse(B& buf)
	{
		buf >> intOptions >> boolOptions >> stringOptions;
	}

};

//cpp struct declaration and definition to avoid including hps in header
static GameOptions _options;

GameManager* GameManager::getInstance()
{
	static GameManager* _gameManager = nullptr;
	if (!_gameManager)
	{
		_gameManager = new GameManager();
		return _gameManager->init() ? _gameManager : nullptr;
	}
	return _gameManager;
	
}

bool GameManager::init()
{
	GameToolbox::log("GAME MANAGER INIT");

	auto fu = ax::FileUtils::getInstance();
	auto wp = fu->getWritablePath();
	_filepath = fmt::format("{}GameManager.opengd", wp);
	
	print();
	
	if (fu->isFileExist(_filepath))
		load();

	return true;
}


void GameManager::load()
{
	GameToolbox::log("gamemanager load");
	std::string byteStr = ax::FileUtils::getInstance()->getStringFromFile(_filepath);
	_options = hps::from_string<GameOptions>(byteStr);
	loadMembersFromMap();
	print();
}

void GameManager::save()
{
	setMembersToMap();
	GameToolbox::log("gamemanager save");
	const std::string& serialized = hps::to_string(_options);
	ax::FileUtils::getInstance()->writeStringToFile(serialized, _filepath);
}

void GameManager::print()
{
	for (const auto& [key, val] : _options.boolOptions) {
		GameToolbox::log("boolOptions[{}] = {}", key, val);
	}

	for (const auto& [key, val] : _options.intOptions) {
		GameToolbox::log("intOptions[{}] = {}", key, val);
	}

	for (const auto& [key, val] : _options.stringOptions) {
		GameToolbox::log("stringOptions[{}] = {}", key, val);
	}
}

template<>
bool GameManager::get<bool>(const std::string& key)
{
	if (_options.boolOptions.count(key))
		return _options.boolOptions.at(key);

	return false;
}

template<>
int GameManager::get<int>(const std::string& key)
{
	if (_options.intOptions.count(key))
		return _options.intOptions.at(key);

	return 0;
}

template<>
std::string GameManager::get<std::string>(const std::string& key)
{
	if (_options.stringOptions.count(key))
		return _options.stringOptions.at(key);

	return "";
}

template<>
void GameManager::set<bool>(const std::string& key, const bool& val) {
	_options.boolOptions[key] = val;
}

template<>
void GameManager::set<int>(const std::string& key, const int& val) {
	_options.intOptions[key] = val;
}

template<>
void GameManager::set<std::string>(const std::string& key, const std::string& val) {
	_options.stringOptions[key] = val;
}


void GameManager::setMembersToMap()
{
	#define SAVE_BOOL(member) set<bool>(#member, member)
	SAVE_BOOL(_openedGarage);
	SAVE_BOOL(_openedCreator);
	SAVE_BOOL(_openedPracticeMode);
	SAVE_BOOL(_mediumQuality);
	#undef SAVE_BOOL

	#define SAVE_INT(member) set<int>(#member, member)
	SAVE_INT(_selectedCube);
	SAVE_INT(_selectedShip);
	SAVE_INT(_selectedBall);
	SAVE_INT(_selectedUfo);
	SAVE_INT(_selectedWave);
	SAVE_INT(_selectedRobot);
	SAVE_INT(_selectedSpider);
	SAVE_INT(_selectedSpecial);
	SAVE_INT(_selectedDeathEffect);
	#undef SAVE_INT
	
	set<int>("_mainSelectedMode", static_cast<int>(_mainSelectedMode));
}

void GameManager::loadMembersFromMap()
{
	#define LOAD_BOOL(member) member = get<bool>(#member)
	LOAD_BOOL(_openedGarage);
	LOAD_BOOL(_openedCreator);
	LOAD_BOOL(_openedPracticeMode);
	LOAD_BOOL(_mediumQuality);
	#undef LOAD_BOOL
	
	#define LOAD_INT(member) member = get<int>(#member)
	LOAD_INT(_selectedCube);
	LOAD_INT(_selectedShip);
	LOAD_INT(_selectedBall);
	LOAD_INT(_selectedUfo);
	LOAD_INT(_selectedRobot);
	LOAD_INT(_selectedSpider);
	LOAD_INT(_selectedSpecial);
	LOAD_INT(_selectedDeathEffect);
	#undef LOAD_INT
	
	_mainSelectedMode = static_cast<IconType>(get<int>("_mainSelectedMode"));
}

bool GameManager::isMedium() { return _mediumQuality; }
bool GameManager::isHigh() { return !_mediumQuality; }


void GameManager::setQuality(bool medium)
{
	_mediumQuality = medium;
	this->save();
	ax::Director::getInstance()->replaceScene(ResourcesLoadingLayer::scene());
}

void GameManager::setQualityMedium() { setQuality(true); }
void GameManager::setQualityHigh() { setQuality(false); }

int GameManager::getSelectedIcon(IconType mode)
{
	switch (mode)
	{
	case kIconTypeCube: return _selectedCube;
	case kIconTypeShip: return _selectedShip;
	case kIconTypeBall: return _selectedBall;
	case kIconTypeUfo: return _selectedUfo;
	case kIconTypeWave: return _selectedWave;
	case kIconTypeRobot: return _selectedRobot;
	case kIconTypeSpider: return _selectedSpider;
	case kIconTypeSpecial: return _selectedSpecial;
	case kIconTypeDeathEffect: return _selectedDeathEffect;
	default: return 0;
	}
}

void GameManager::setSelectedIcon(IconType mode, int id) {
	switch (mode)
	{
	case kIconTypeCube: _selectedCube = id; break;
	case kIconTypeShip: _selectedShip = id; break;
	case kIconTypeBall: _selectedBall = id; break;
	case kIconTypeUfo: _selectedUfo = id; break;
	case kIconTypeWave: _selectedWave = id; break;
	case kIconTypeRobot: _selectedRobot = id; break;
	case kIconTypeSpider: _selectedSpider = id; break;
	case kIconTypeSpecial: _selectedSpecial = id; break;
	case kIconTypeDeathEffect: _selectedDeathEffect = id; break;
	}
}

