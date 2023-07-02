/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2021 Bytedance Inc.

 https://axmolengine.github.io/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "LoadingLayer.h"
#include "GameManager.h"
#include "ResourcesLoadingLayer.h"
#include "external/constants.h"
#include "GameToolbox/log.h"

#include "platform/Application.h"
#include "platform/GLView.h"
#include "base/Director.h"
#include "base/EventDispatcher.h"

#ifdef AX_PLATFORM_PC
	#include "platform/desktop/GLViewImpl-desktop.h"
#elif (AX_TARGET_PLATFORM == AX_PLATFORM_ANDROID)
	#include "platform/android/GLViewImpl-android.h"
#elif (AX_TARGET_PLATFORM == AX_PLATFORM_IOS)
	#include "platform/ios/GLViewImpl-ios.h"
#elif (AX_TARGET_PLATFORM == AX_PLATFORM_WINRT)
	#include "platform/winrt/GLViewImpl-winrt.h"
#endif

#define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include <audio/AudioEngine.h>
#endif

USING_NS_AX;

static ax::Size designResolutionSize = ax::Size(480, 320);
static ax::Size smallResolutionSize = ax::Size(480, 320);
static ax::Size mediumResolutionSize = ax::Size(1024, 768);
static ax::Size largeResolutionSize = ax::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}


// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
	return 0; // flag for packages manager
}
int AppDelegate::applicationGetRefreshRate()
{
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
	auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return mode->refreshRate;
#else
	return 60;
#endif
}


static void setupDesignResolution(GLView* glView)
{
	auto screen_size = glView->getFrameSize();
	ResolutionPolicy resPolicy;

	if (screen_size.height / designResolutionSize.height <= screen_size.width / designResolutionSize.width)
	{
		resPolicy = ResolutionPolicy::FIXED_HEIGHT;
	}
	else
	{
		resPolicy = ResolutionPolicy::FIXED_WIDTH;
	}

	glView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, resPolicy);
}

#ifdef AX_PLATFORM_PC
static void onGLFWwindowSizeCallback(GLFWwindow*, int w, int h)
{
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();

	glView->setFrameSize(w, h);
	setupDesignResolution(glView);

	director->getEventDispatcher()->dispatchCustomEvent(GLViewImpl::EVENT_WINDOW_RESIZED, nullptr);
}
#endif


bool AppDelegate::applicationDidFinishLaunching()
{


	// initialize director
	auto director = Director::getInstance();
	auto glView = director->getOpenGLView();
	if (!glView)
	{
#ifdef AX_PLATFORM_PC
		glView = GLViewImpl::createWithRect(
			"OpenGD", ax::Rect(0, 0, 1280, 720), 1.f, true);
#else
		glView = GLViewImpl::create("OpenGD");
#endif

#if (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
	int dispX;
	int dispY;
	auto disp = glfwGetPrimaryMonitor();
	glfwGetMonitorPhysicalSize(disp, &dispX, &dispY);
#endif

#if FULLSCREEN == true
		auto full = dynamic_cast<GLViewImpl *>(glView);
		full->setFullscreen();
#endif
		director->setOpenGLView(glView);
	}

	// display FPS stats or not.
	director->setStatsDisplay(SHOW_FPS);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / applicationGetRefreshRate());

	setupDesignResolution(glView);

#ifdef AX_PLATFORM_PC

	glfwSetWindowAspectRatio(static_cast<GLViewImpl*>(glView)->getWindow(), 16, 9);

	glfwSetWindowSizeCallback(static_cast<GLViewImpl*>(glView)->getWindow(), onGLFWwindowSizeCallback);

#endif

	GameToolbox::log("APLICATION INIT");
	director->setContentScaleFactor(GameManager::getInstance()->isHigh() ? 4.0f : 2.0f);

#if FULLSCREEN == true && AX_TARGET_PLATFORM == AX_PLATFORM_LINUX
	std::cout << "X " << dispX << " Y " << dispY << std::endl;
	glView->setFrameSize((float)dispX, (float)dispY);
	// director->setAnimationInterval(1.0f / applicationGetRefreshRate());
#endif

	register_all_packages();

	// create a scene. it's an autorelease object
	director->runWithScene(ResourcesLoadingLayer::scene());

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#endif
}
