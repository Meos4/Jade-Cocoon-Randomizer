#pragma once

#include "Backend/Entity.hpp"

class Game;
class Random;
class RawFile;
class SharedData;

namespace Helpers
{
	void randomizeModelInterp(RawFile* file, Model_t model, const SharedData& sharedData, Game* game, RawFile& executable);
	void randomizeCLUT(RawFile* file, Random* random);
}
