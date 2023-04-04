#pragma once

#include <axmol.h>
#include "PopupLayer.h"

class SongInfoLayer : public PopupLayer
{
private:
	std::string _downloadLink;
	std::string _ngArtistLink;
	std::string _ytArtistLink;
	std::string _fbArtistLink;
public:
	static SongInfoLayer* create(int soundtrackID);
	static SongInfoLayer* create(std::string_view songName, std::string_view artistName, std::string_view urlLink,
		std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink);
	bool init(std::string_view songName, std::string_view artistName, std::string_view urlLink,
		std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink);
};
