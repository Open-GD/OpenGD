#include "getTextureString.h"
#include "GameManager.h"

static inline std::string getTextureString_AppendResources(std::string texture)
{
	std::string nTexture = "Resources/";
	nTexture += texture;

	auto pos = nTexture.find('.');
	if (pos != std::string::npos) {
		nTexture.insert(pos, GameManager::getInstance()->isHigh() ? "-uhd" : "-hd");	
	}
	// GameToolbox::log("texture: {}", nTexture);
	return nTexture;
}

static inline std::string getTextureString_WithoutResources(std::string texture)
{
	auto pos = texture.find('.');
	if (pos != std::string::npos) {
		texture.insert(pos, GameManager::getInstance()->isHigh() ? "-uhd" : "-hd");	
	}
	return texture;
}


std::string GameToolbox::getTextureString(std::string texture) {
	return getTextureString_WithoutResources(texture);
}
