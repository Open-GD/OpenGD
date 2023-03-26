#include "GameManager.h"
#include "external/hps/hps.h"
#include <axmol.h>
#include "GameToolbox.h"

static struct GameOptions
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
	set<bool>("_openedGarage", _openedGarage);
	set<bool>("_openedCreator", _openedCreator);
	set<bool>("_openedPracticeMode", _openedPracticeMode);
}

void GameManager::loadMembersFromMap()
{
	_openedGarage = get<bool>("_openedGarage");
	_openedCreator = get<bool>("_openedCreator");
	_openedPracticeMode = get<bool>("_openedPracticeMode");
}