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

	_loading = LoadingCircle::create();
	_loading->setPosition(winSize.x/2, winSize.y/2);
	_loading->setVisible(true);

	GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = ax::Menu::create();
	auto backBtn =
		MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) { GameToolbox::popSceneWithTransition(0.5f, kTransitionFade); });
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
		_loading->setVisible(true);
		listView->removeAllItems();

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
		_loading->setVisible(true);
		listView->removeAllItems();

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

	GameToolbox::createBG(this);
	GameToolbox::createCorners(this, false, false, true, true);

	auto pageTxt = ax::Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), "");
	pageTxt->setScale(100 / 0.6f);
	pageTxt->setAnchorPoint({ 1, 0.5 });
	pageTxt->setPosition({ winSize.width - 7, winSize.height - 9.05f });
	this->addChild(pageTxt, 5);

	listView = ax::ui::ListView::create();
	listView->setBounceEnabled(true);
	listView->setAnchorPoint({ 0.5, 0.5 });
	listView->setScrollBarEnabled(false);

	auto list = ListLayer::create(listView, "Online Levels", { 191, 114, 62, 255 });
	list->setPosition({ (winSize.width - 356) / 2, ((winSize.height - 220) / 2) - 5 });

	auto listener = ax::EventListenerKeyboard::create();
	listener->onKeyPressed = [=](ax::EventKeyboard::KeyCode key, ax::Event*) {
		switch (key) {
		case ax::EventKeyboard::KeyCode::KEY_ESCAPE:
			GameToolbox::popSceneWithTransition(0.5f);
			break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	addChild(list);
	addChild(_loading);

	std::string postData = fmt::format(
		"str={}&secret=Wmfd2893gb7&type={}&page={}", _searchObj->_searchQuery, (int)_searchObj->_screenID,
		_searchObj->_page);
	if (!_searchObj->_difficulty.empty()) postData += fmt::format("&diff={}", _searchObj->_difficulty);
	if (!_searchObj->_length.empty()) postData += fmt::format("&len={}", _searchObj->_length);
	GameToolbox::executeHttpRequest(
		"http://www.boomlings.com/database/getGJLevels21.php", postData, ax::network::HttpRequest::Type::POST,
		AX_CALLBACK_2(LevelBrowserLayer::onHttpRequestCompleted, this));

	return true;
}

void LevelBrowserLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	listView->removeAllItems();
	_loading->setVisible(false);
	if (auto str = GameToolbox::getResponse(response))
	{
		//error codes -1, -2 etc
		if((*str).length() < 5)
			return;
		
		GameToolbox::log("response: {}", *str);
		auto splits = GameToolbox::splitByDelimStringView((*str), '#');
		auto levels = GameToolbox::splitByDelimStringView(splits[0], '|');
		auto authorsStrings = GameToolbox::splitByDelimStringView(splits[1], '|');
		
		std::vector<std::vector<std::string_view>> authors;
		authors.reserve(authorsStrings.size()); //pre-allocate enough memory
		for(const std::string_view aStr : authorsStrings) {
			authors.push_back(std::move(GameToolbox::splitByDelimStringView(aStr, ':')));
		}
		
		auto getAuthor = [&](GJGameLevel* gjlevel) -> std::string_view
		{
			for(const auto& author : authors)
			{
				if(GameToolbox::stoi(author[0]) == gjlevel->_playerID)
					return author[1];
			}
			return std::string("-");
		};
		
		std::vector<GJGameLevel*> toInsert;
		toInsert.reserve(levels.size()); //pre-allocate enough memory
		
		for (size_t i = 0; i < levels.size(); i++)
		{
			auto gjlevel = GJGameLevel::createWithResponse(levels[i]);
			gjlevel->_levelCreator = getAuthor(gjlevel);
			gjlevel->_songName = "Cool catchy song";
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
	_loading->setVisible(false);
	_leftBtn->setEnabled(_searchObj->_page > 0);
	_leftBtn->setVisible(_searchObj->_page > 0);
	_rightBtn->setEnabled(true);
	_rightBtn->setVisible(true);

	int col = 0;
	for (auto l : _cachedLevels[_searchObj->_page])
	{
		auto levelCell = LevelCell::create(l);
		levelCell->updateBGColor(col);
		listView->pushBackCustomItem(levelCell);
		col++;
	}
}