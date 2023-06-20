#include "rand.h"
#include <random>

int GameToolbox::randomInt(int min, int max) {
	static std::random_device device;
	static std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

int GameToolbox::randomInt(int max) {
	return GameToolbox::randomInt(0, max);
}
float GameToolbox::randomFloat(int min, int max) {
	return static_cast<float>(GameToolbox::randomInt(min, max));
}
float GameToolbox::randomFloat(int max) {
	return static_cast<float>(GameToolbox::randomInt(max));
}

ax::Color3B GameToolbox::randomColor3B() {
	uint8_t r = GameToolbox::randomInt(255);
	uint8_t g = GameToolbox::randomInt(255);
	uint8_t b = GameToolbox::randomInt(255);

	return {r, g, b};
}
