#include "LevelTools.h"
#include "base64.h"
#include "external/constants.h"
#include <cstring>

constexpr int ARTIST_DJVI = 0;
constexpr int ARTIST_WATERFLAME = 1;
constexpr int ARTIST_OCULAR = 2;
constexpr int ARTIST_FBOUND = 3;
constexpr int ARTIST_STEP = 4;
constexpr int ARTIST_DJNATE = 5;
constexpr int ARTIST_F777 = 6;
constexpr int ARTIST_MDK = 7;

bool LevelTools::verifyLevelIntegrity(std::string_view levelData, int id)
{
	if(!levelData.starts_with("H4sIAAAAAAAA") && id <= 21)
		return false;
	
	return true;
}

std::string LevelTools::getAudioFilename(int lid)
{
	switch(lid) {
		case 1: return "BackOnTrack.mp3";
		case 2: return "Polargeist.mp3";
		case 3: return "DryOut.mp3";
		case 4: return "BaseAfterBase.mp3";
		case 5: return "CantLetGo.mp3";
		case 6: return "Jumper.mp3";
		case 7: return "TimeMachine.mp3";
		case 8: return "Cycles.mp3";
		case 9: return "xStep.mp3";
		case 10: return "Clutterfunk.mp3";
		case 11: return "TheoryOfEverything.mp3";
		case 12: return "Electroman.mp3";
		case 13: return "Clubstep.mp3";
		case 14: return "Electrodynamix.mp3";
		case 15: return "HexagonForce.mp3";
		case 16: return "BlastProcessing.mp3";
		case 17: return "TheoryOfEverything2.mp3";
		case 18: return "GeometricalDominator.mp3";
		case 19: return "Deadlocked.mp3";
		case 20: return "Fingerdash.mp3";
		default: return "StereoMadness.mp3";
	}
}

std::string LevelTools::getAudioTitle(int lid) {
	switch(lid) {
		case -1: return "Practice: Stay Inside Me";
		case 1: return "Back On Track";
		case 2: return "Polargeist";
		case 3: return "Dry Out";
		case 4: return "Base After Base";
		case 5: return "Cant Let Go";
		case 6: return "Jumper";
		case 7: return "Time Machine";
		case 8: return "Cycles";
		case 9: return "xStep";
		case 10: return "Clutterfunk";
		case 11: return "Theory Of Everything";
		case 12: return "Electroman Adventures";
		case 13: return "Clubstep";
		case 14: return "Electrodynamix";
		case 15: return "Hexagon Force";
		case 16: return "Blast Processing";
		case 17: return "Theory Of Everything 2";
		case 18: return "Geometrical Dominator";
		case 19: return "Deadlocked";
		case 20: return "Fingerdash";
		default: return "Stereo Madness";
	}
}

int LevelTools::getArtistForAudio(int lid)
{
	switch(lid) {
		case -1: return ARTIST_OCULAR;
		case 0: return ARTIST_FBOUND;
		case 1: return ARTIST_DJVI;
		case 2: return ARTIST_STEP;
		case 3: return ARTIST_FBOUND;
		case 4: return ARTIST_FBOUND;
		case 5: return ARTIST_FBOUND;
		case 6: return ARTIST_DJVI;
		case 7: return ARTIST_WATERFLAME;
		case 8: return ARTIST_DJVI;
		case 9: return ARTIST_DJVI;
		case 10: return ARTIST_WATERFLAME;
		case 11: return ARTIST_DJNATE;
		case 12: return ARTIST_WATERFLAME;
		case 13: return ARTIST_DJNATE;
		case 14: return ARTIST_DJNATE;
		case 15: return ARTIST_WATERFLAME;
		case 16: return ARTIST_WATERFLAME;
		case 17: return ARTIST_DJNATE;
		case 18: return ARTIST_WATERFLAME;
		case 19: return ARTIST_F777;
		case 20: return ARTIST_MDK;
		default: return ARTIST_FBOUND;
	}
}

std::string LevelTools::getNameForArtist(int artistId)
{
	switch (artistId)
	{
		case 0: return "DJVI";
		case 1: return "Waterflame";
		case 2: return "OcularNebula";
		case 3: return "ForeverBound";
		case 4: return "Step";
		case 5: return "DJ-Nate";
		case 6: return "F-777";
		case 7: return "MDK";
		default: return "Foreverbound";
	}
}

std::string LevelTools::getFbURLForArtist(int artistId)
{
	switch (artistId)
	{
		case 0: return "https://www.facebook.com/DJVITechno";
		case 1: return "http://www.facebook.com/pages/Waterflame/210371073165";
		case 2: return "";
		case 3: return "https://www.facebook.com/foreverboundofficial";
		case 4: return "https://www.facebook.com/StephanWellsMusic";
		case 5: return "https://www.facebook.com/pages/Dj-Nate/280339788656689";
		case 6: return "http://www.facebook.com/pages/F-777/286884484660892";
		case 7: return "https://www.facebook.com/MDKOfficial";
		default: return "";
	}
}

std::string LevelTools::getNgURLForArtist(int artistId)
{
	switch (artistId)
	{
		case 0: return "https://djvi.newgrounds.com/";
		case 1: return "https://waterflame.newgrounds.com/";
		case 2: return "https://ocularnebula.newgrounds.com/";
		case 3: return "http://foreverbound.newgrounds.com/";
		case 4: return "http://step.newgrounds.com/";
		case 5: return "http://dj-nate.newgrounds.com/";
		case 6: return "http://f-777.newgrounds.com/";
		case 7: return "";
		default: return "";
	}
}

std::string LevelTools::getYtURLForArtist(int artistId)
{
	switch (artistId)
	{
		case 0: return "https://www.youtube.com/user/DJVITechno";
		case 1: return "https://www.youtube.com/user/waterflame89";
		case 2: return "https://www.youtube.com/@OcularNebula";
		case 3: return "https://www.youtube.com/user/ForeverBoundOfficial";
		case 4: return "https://www.youtube.com/user/NGStep";
		case 5: return "https://www.youtube.com/@dj-Nate";
		case 6: return "https://www.youtube.com/user/JesseValentineMusic";
		case 7: return "https://www.youtube.com/user/MDKOfficialYT";
		default: return "";
	}
}

std::string LevelTools::getURLForAudio(int lid)
{
	switch(lid)
	{
		case -1: return "https://www.youtube.com/watch?v=5Epc1Beme90";
		case 0: return "https://www.youtube.com/watch?v=JhKyKEDxo8Q";
		case 1: return "https://www.youtube.com/watch?v=N9vDTYZpqXM";
		case 2: return "https://www.youtube.com/watch?v=4W28wWWxKuQ";
		case 3: return "https://www.youtube.com/watch?v=FnXabH2q2A0";
		case 4: return "https://www.youtube.com/watch?v=TZULkgQPHt0";
		case 5: return "https://www.youtube.com/watch?v=fLnF-QnR1Zw";
		case 6: return "https://www.youtube.com/watch?v=ZXHO4AN_49Q";
		case 7: return "https://www.youtube.com/watch?v=zZ1L9JD6l0g";
		case 8: return "https://www.youtube.com/watch?v=KDdvGZn6Gfs";
		case 9: return "https://www.youtube.com/watch?v=PSvYfVGyQfw";
		case 10: return "https://www.youtube.com/watch?v=D5uJOpItgNg";
		case 11: return "https://www.newgrounds.com/audio/listen/354826";
		case 12: return "https://www.youtube.com/watch?v=Pb6KyewC_Vg";
		case 13: return "https://www.newgrounds.com/audio/listen/396093";
		case 14: return "https://www.newgrounds.com/audio/listen/368392";
		case 15: return "https://www.youtube.com/watch?v=afwK743PL2Y";
		case 16: return "https://www.youtube.com/watch?v=Z5RufkDHsdM";
		case 17: return "https://www.youtube.com/watch?v=fn98711CEoI";
		case 18: return "https://www.robtopgames.com/geometricaldominator";
		case 19: return "https://www.youtube.com/watch?v=QRGkFkf2r0U";
		case 20: return "https://www.youtube.com/watch?v=BuPmq7yjDnI";
		default: return "https://www.youtube.com/watch?v=JhKyKEDxo8Q";
	}
}
