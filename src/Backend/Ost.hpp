#pragma once

#include "Common/Types.hpp"

#include <map>
#include <vector>

namespace Ost
{
	inline const std::map<s16, const char*> names
	{
		{ 11, "[Unused] 1" },
		{ 12, "Time of Judgment" },
		{ 13, "Syrus Village ~ Night of the Funeral" },
		{ 14, "Syrus Village" },
		{ 15, "Syrus Village ~ Chrysalis" },
		{ 16, "Invasion" },
		{ 17, "Beetle Forest" },
		{ 18, "Yamu Tribe" },
		{ 19, "Legend of Arcana ~ Clandestine Meeting" },
		{ 20, "[Unused] 2" },
		{ 21, "Passion" },
		{ 22, "Gate to the Binding World" },
		{ 23, "The Binding Ceremony" },
		{ 24, "Birdman Kikinak" },
		{ 25, "The Blue Cocoon Master ~ Koris" },
		{ 26, "Spider Forest" },
		{ 27, "Legend of Arcana ~ Main Theme" },
		{ 28, "The Godtree" },
		{ 29, "Legend of Arcana ~ Adventure" },
		{ 30, "Legend of Arcana ~ Clandestine Meeting B" },
		{ 31, "Moth Forest" },
		{ 32, "Dragonfly Forest" },
		{ 33, "The Promised Land ~ Kemuel Temple" },
		{ 34, "Dream" },
		{ 35, "Masked Boy ~ Lui"},
		{ 36, "Battle ~ The One Who Stood In The Way" },
		{ 37, "Birdman Kikinak of the Netherworld" },
		{ 38, "The Blue Cocoon Master ~ Koris of the Netherworld" },
		{ 39, "Spider Forest of the Netherworld" },
		{ 40, "Gehenna's Art ~ The Seal" },
		{ 41, "Beetle Forest of the Netherworld" },
		{ 42, "Dragonfly Forest of the Netherworld" },
		{ 43, "Yamu Tribe of the Netherworld" },
		{ 122, "Gatewatcher Of Time ~ Ni, The Chief" },
		{ 124, "Syrus Village ~ The Holy Ghost" },
		{ 125, "[Unused] 3" },
		{ 126, "Legend of Arcana ~ Clandestine Meeting C" },
		{ 128, "Gi, The Seer" },
		{ 141, "[Unused] 4" },
		{ 142, "Area of Space - Time Part 1" },
		{ 143, "Neverending Corridor" },
		{ 144, "Legend of Arcana ~ Clandestine Meeting D" },
		{ 145, "[Unknown] 1" },
		{ 146, "Battle ~ Destiny" },
		{ 153, "Area of Space - Time Part 2" }
	};

	std::vector<s16> id();
};