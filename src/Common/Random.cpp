#include "Random.hpp"

Random::Random()
	: m_mte(std::random_device{}())
{
}

bool Random::generateProba(float percent)
{
	return percent >= 100.f || generatePercent() <= percent;
}

float Random::generatePercent()
{
	return generate(100.f);
}

void Random::setSeed(Random::Type seed)
{
	m_mte.seed(seed);
}