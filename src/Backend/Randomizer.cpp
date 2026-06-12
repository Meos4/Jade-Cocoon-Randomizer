#include "Randomizer.hpp"

Randomizer::Randomizer(Game* game)
    : m_game(game), m_sharedData(game)
{
}

void Randomizer::init()
{
    m_sharedData.read();
}

const Game& Randomizer::game() const
{
    return *m_game;
}

void Randomizer::writeFrameFns() const
{
	m_game->writeFrameFns();
}