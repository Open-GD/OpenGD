/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "GameManager.h"
#include "ResourcesLoadingLayer.h"
#include "LoadingLayer.h"

#include <2d/Scene.h>
#include <2d/Label.h>
#include <2d/ActionInterval.h>
#include <2d/ActionInstant.h>
#include <platform/FileUtils.h>
#include <base/Director.h>
#include "GameToolbox/log.h"

USING_NS_AX;

ResourcesLoadingLayer* ResourcesLoadingLayer::create() {
	ResourcesLoadingLayer* pRet = new ResourcesLoadingLayer();
	if (pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Scene* ResourcesLoadingLayer::scene()
{
	auto scene = ax::Scene::create();
	scene->addChild(ResourcesLoadingLayer::create());
	return scene;
}

void ResourcesLoadingLayer::loadLoadingLayer()
{
	this->runAction(Sequence::create(DelayTime::create(0), CallFunc::create([]() {
		Director::getInstance()->replaceScene(LoadingLayer::scene()); 
	}), nullptr));
}

bool ResourcesLoadingLayer::init()
{
	_fu = FileUtils::getInstance();

	for (std::string_view a : _fu->getSearchPaths())
	{
		GameToolbox::log("search path: {}", a);
	}
	//some random files to check if theres already a path added
	if (_fu->isFileExist("game_bg_01_001-hd.png") && _fu->isFileExist("GJ_LaunchSheet-hd.png") && _fu->isFileExist("GJ_GameSheet03-uhd.png"))
	{
		loadLoadingLayer();
		return true;
	}

	_gm = GameManager::getInstance();
	_dir = Director::getInstance();
	_writablePath = _fu->getWritablePath();

	auto winSize = _dir->getWinSize();
	_posMiddle = { winSize.width / 2, winSize.height / 2 };

	if(auto path = _gm->get<std::string>("resources_path"); !path.empty() && _fu->isDirectoryExist(path))
	{
		_fu->addSearchPath(path, true);
		loadLoadingLayer();
		return true;
	}

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
	handleWindows();
#endif

#if (AX_TARGET_PLATFORM == AX_PLATFORM_MAC)
	handleMac();
#endif

#if (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
	handleLinux();
#endif
	return true;
}

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>


void ResourcesLoadingLayer::handleWindows()
{
	auto label = Label::create();
	label->setString("Geometry Dash Resources not found!\n Waiting for GeometryDash.exe");
	label->setSystemFontSize(20);
	label->setPosition(_posMiddle);
	addChild(label);

	_gdProcessAction = RepeatForever::create(Sequence::create(DelayTime::create(0.6), CallFunc::create([=]() {
		static int nPoints = -1;
		++nPoints;
		if (nPoints > 3)
			nPoints = 0;

		label->setString(fmt::format("Geometry Dash Resources not found!\n Waiting for GeometryDash.exe{}", nPoints == 1 ? "." : nPoints == 2 ? ".." : nPoints == 3 ? "..." : ""));

		if (std::string exepath = ResourcesLoadingLayer::getRunningGDPathWindows(); !exepath.empty())
		{
			if (isWindowsGDPathValid(exepath))
			{
				exepath.erase(exepath.find_last_of('\\'));
				std::string resourcesPath = fmt::format("{}\\Resources", exepath);
				_fu->addSearchPath(resourcesPath, true);
				_gm->set<std::string>("resources_path", resourcesPath);
				_gm->save();
				loadLoadingLayer();
			}
			else
			{
				label->setString("Invalid GeometryDash.exe path found...");
				nPoints = 0;
			}
		}
	}), nullptr));

	this->runAction(_gdProcessAction);
}

bool ResourcesLoadingLayer::isWindowsGDPathValid(std::string exepath)
{
	// Check the size of the executable file
	HANDLE hFile = CreateFile(std::wstring(exepath.begin(), exepath.end()).c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return false;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	//if lower than 6 mb or greater than 7mb return false
	if (dwFileSize < 6 * 1024 * 1024 || dwFileSize >= 7 * 1024 * 1024)
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);

	exepath.erase(exepath.find_last_of('\\'));

	if (!_fu->isDirectoryExist(fmt::format("{}\\Resources", exepath))) return false;
	if (!_fu->isFileExist(fmt::format("{}\\libcocos2d.dll", exepath))) return false;
	if (!_fu->isFileExist(fmt::format("{}\\libcurl.dll", exepath))) return false;

	return true;
}


std::string ResourcesLoadingLayer::getRunningGDPathWindows()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return "";
	}

	cProcesses = cbNeeded / sizeof(DWORD);

	for (unsigned int i = 0; i < cProcesses; i++)
	{
		if (DWORD processID = aProcesses[i]; processID != 0)
		{
			HANDLE processHandle = NULL;
			TCHAR filename[MAX_PATH];
			processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
			if (processHandle != NULL)
			{
				if (GetModuleFileNameEx(processHandle, NULL, filename, MAX_PATH) != 0)
				{
					std::wstring str(filename);
					if (str.find(L"GeometryDash.exe") != std::string::npos)
					{
						TerminateProcess(processHandle, 1); //doesnt seem to work?
						CloseHandle(processHandle);
						return { str.begin(), str.end() };
					}
				}
				CloseHandle(processHandle);
			}
		}
	}
	return "";
}

#endif //#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)


//#if (AX_TARGET_PLATFORM == AX_PLATFORM_MAC)
void ResourcesLoadingLayer::handleMac() {}
//#endif

//#if (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
void ResourcesLoadingLayer::handleLinux() {}
//#endif