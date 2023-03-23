#include "LevelBrowserLayer.h"
#include "LevelCell.h"
#include "LevelSearchLayer.h"

ax::Scene* LevelBrowserLayer::scene(GJSearchObject* search)
{
	auto scene = ax::Scene::create();
	scene->addChild(LevelBrowserLayer::create(search));
	return scene;
}

LevelBrowserLayer* LevelBrowserLayer::create(GJSearchObject* search)
{
	LevelBrowserLayer* ret = new LevelBrowserLayer();
	if (ret->init(search))
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret = nullptr;
		return nullptr;
	}
}

bool LevelBrowserLayer::init(GJSearchObject* search)
{
	if (!Layer::init()) return false;
	_searchObj = search;

	auto director = ax::Director::getInstance();
	auto winSize = director->getWinSize();

	GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = ax::Menu::create();
	auto backBtn =
		MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) { ax::Director::getInstance()->popScene(); });
	backBtnMenu->addChild(backBtn);
	backBtnMenu->setPosition({24.0, winSize.height - 23.0f});
	this->addChild(backBtnMenu, 5);
	auto pageMenu = ax::Menu::create();
	_rightBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [this](Node*) {
		_searchObj->_page++;
		_leftBtn->setEnabled(false);
		_leftBtn->setVisible(false);
		_rightBtn->setEnabled(false);
		_rightBtn->setVisible(false);

		if (_cachedLevels.contains(_searchObj->_page))
		{
			fillList();
			return;
		}

		std::string postData = fmt::format(
			"str={}&secret=Wmfd2893gb7&type={}&page={}", _searchObj->_searchQuery, (int)_searchObj->_screenID,
			_searchObj->_page);

		if (!_searchObj->_difficulty.empty()) postData += fmt::format("&diff={}", _searchObj->_difficulty);
		if (!_searchObj->_length.empty()) postData += fmt::format("&len={}", _searchObj->_length);
		GameToolbox::executeHttpRequest(
			"http://www.boomlings.com/database/getGJLevels21.php", postData, ax::network::HttpRequest::Type::POST,
			AX_CALLBACK_2(LevelBrowserLayer::onHttpRequestCompleted, this));
	});
	_rightBtn->setPosition({259.5, 0});
	_leftBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [this](Node*) {
		_searchObj->_page--;
		_leftBtn->setEnabled(false);
		_leftBtn->setVisible(false);
		_rightBtn->setEnabled(false);
		_rightBtn->setVisible(false);

		if (_cachedLevels.contains(_searchObj->_page))
		{
			fillList();
			return;
		}

		std::string postData = fmt::format(
			"str={}&secret=Wmfd2893gb7&type={}&page={}", _searchObj->_searchQuery, (int)_searchObj->_screenID,
			_searchObj->_page);
		if (!_searchObj->_difficulty.empty()) postData += fmt::format("&diff={}", _searchObj->_difficulty);
		if (!_searchObj->_length.empty()) postData += fmt::format("&len={}", _searchObj->_length);
		GameToolbox::executeHttpRequest(
			"http://www.boomlings.com/database/getGJLevels21.php", postData, ax::network::HttpRequest::Type::POST,
			AX_CALLBACK_2(LevelBrowserLayer::onHttpRequestCompleted, this));
	});
	_leftBtn->setEnabled(false);
	_leftBtn->setVisible(false);
	_leftBtn->setPosition({-259.5, 0});
	_rightBtn->setScaleX(-1);
	_rightBtn->setEnabled(false);
	_rightBtn->setVisible(false);
	pageMenu->addChild(_leftBtn);
	pageMenu->addChild(_rightBtn);
	pageMenu->setPosition({283.5, 160});
	this->addChild(pageMenu, 5);

	auto bg = ax::Sprite::create("GJ_gradientBG.png");
	bg->setStretchEnabled(false);
	bg->setScaleX(winSize.width / bg->getContentSize().width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setAnchorPoint({0, 0});
	bg->setColor({0, 102, 255});
	addChild(bg);

	listView = ax::ui::ListView::create();
	listView->setAnchorPoint({0.5, 0.5});
	listView->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
	listView->setBackGroundColor(ax::Color3B::ORANGE);

	auto list = ListLayer::create(listView, "Online Levels");
	listView->setPosition({180, 120});
	list->setPosition({106.5, 45});
	listView->setTopPadding(25);

	addChild(list);

	std::string postData = fmt::format(
		"str={}&secret=Wmfd2893gb7&type={}&page={}", _searchObj->_searchQuery, (int)_searchObj->_screenID,
		_searchObj->_page);
	if (!_searchObj->_difficulty.empty()) postData += fmt::format("&diff={}", _searchObj->_difficulty);
	if (!_searchObj->_length.empty()) postData += fmt::format("&len={}", _searchObj->_length);
	GameToolbox::executeHttpRequest(
		"http://www.boomlings.com/database/getGJLevels21.php", postData, ax::network::HttpRequest::Type::POST,
		AX_CALLBACK_2(LevelBrowserLayer::onHttpRequestCompleted, this));
}

void LevelBrowserLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	listView->removeAllItems();
	if (auto str = GameToolbox::getResponse(response))
	{
		//error codes -1, -2 etc
		if((*str).length() < 5)
			return;
		
		GameToolbox::log("response: {}", *str);
		auto splits = GameToolbox::splitByDelim((*str), '#');
		auto levels = GameToolbox::splitByDelim(splits[0], '|');
		auto authorsStrings = GameToolbox::splitByDelim(splits[1], '|');
		
		std::vector<std::vector<std::string>> authors;
		authors.reserve(authorsStrings.size()); //pre-allocate enough memory
		for(const std::string& aStr : authorsStrings) {
			authors.push_back(std::move(GameToolbox::splitByDelim(aStr, ':')));
		}
		
		auto getAuthor = [&](GJGameLevel* gjlevel) -> std::string
		{
			for(const auto& author : authors)
			{
				if(std::stoi(author[0]) == gjlevel->_PlayerID)
					return author[1];
			}
			return std::string("-");
		};
		
		std::vector<GJGameLevel*> toInsert;
		toInsert.reserve(levels.size()); //pre-allocate enough memory
		
		for (size_t i = 0; i < levels.size(); i++)
		{
			auto gjlevel = GJGameLevel::createWithResponse(levels[i]);
			gjlevel->_LevelCreator = getAuthor(gjlevel);
			gjlevel->_SongName = "Cool catchy song";
			toInsert.push_back(gjlevel);
		}

		_cachedLevels.insert({_searchObj->_page, toInsert});

		fillList();
	}
	else
	{
		GameToolbox::log("request failed");
	}
}

void LevelBrowserLayer::fillList()
{
	listView->removeAllItems();
	_leftBtn->setEnabled(_searchObj->_page > 0);
	_leftBtn->setVisible(_searchObj->_page > 0);
	_rightBtn->setEnabled(true);
	_rightBtn->setVisible(true);

	for (auto l : _cachedLevels[_searchObj->_page])
	{
		auto levelCell = LevelCell::create(l);
		listView->pushBackCustomItem(levelCell);
	}
}