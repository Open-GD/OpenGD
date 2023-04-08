#include "LevelEndStrings.h"

std::vector<std::string> LevelEndStrings::strings = {
    "Awesome!", "Good Job!", "Well Done!", "Impressive!",
    "Amazing!", "Incredible!", "Skillful!", "Brilliant!",
    "You are... The One!", "How this is possible?",
    "You beat me...", "Challenge Breaker!", "I am speechless...",
    "Not bad!", "Warp Speed", "Y u do dis?", "lol is lol backwards",
    "DROP THE BEAT", "Took you long enough...", "Teach me oh great one",
    "Haxxor?", "Tripple spikes?", "RubRubRubRubRubRub", "SO FAST",
    "Hmmmmmmmmmmm", "Ship part was cool!", "Timing could be better",
    "I cant hear the music.", "Pump. It. Up.", "I am Batman",
    "Take a break.", "AFK", "he protec"
};

std::string LevelEndStrings::getRandomString()
{
    srand(time(nullptr));
    return LevelEndStrings::strings[rand() % LevelEndStrings::strings.size()];
}