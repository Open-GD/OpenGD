#pragma once
#include <string>
#include <string_view>

#include "2d/CCNode.h"

enum SearchType {
	kGJSearchTypeLiked = 2,
	kGJSearchTypeDownloaded = 1,
	kGJSearchTypeSearch = 0,
	kGJSearchTypeTrending = 3,
	kGJSearchTypeRecent = 4,
	kGJSearchTypeUsersLevels = 5,
	kGJSearchTypeFeatured = 6,
	kGJSearchTypeMagic = 7,
	kGJSearchTypeSends = 8,
	kGJSearchTypeMapPack = 9,
	kGJSearchTypeMapPackOnClick = 10,
	kGJSearchTypeAwarded = 11,
	kGJSearchTypeFollowed = 12,
	kGJSearchTypeFriends = 13,
	kGJSearchTypeUsers = 14,
	kGJSearchTypeLikedGDW = 15,
	kGJSearchTypeHallOfFame = 16,
	kGJSearchTypeFeaturedGDW = 17,
	kGJSearchTypeSimilar = 18,
	kGJSearchTypeMyLevels = 98,
	kGJSearchTypeSavedLevels = 99,
	kGJSearchTypeFavouriteLevels = 100
};
	

	class GJSearchObject : public ax::Node {
	public:
			SearchType _screenID;
			std::string _searchQuery = "favij";
			std::string _difficulty;
			std::string _length;
			int _page;
			bool _starFilter;
			bool _noStarFilter;
			bool _uncompletedFilter;
			bool _completedFilter;
			bool _featuredFilter;
			bool _originalFilter;
			bool _twoPlayerFilter;
			bool _coinsFilter;
			bool _epicFilter;
			//GJDifficulty m_eDemonFilter;
			int _songID;
			bool _customSongFilter;
			bool _songFilter;

		static GJSearchObject* create();
		static GJSearchObject* create(std::string_view query);
		static GJSearchObject* create(SearchType type);
	};