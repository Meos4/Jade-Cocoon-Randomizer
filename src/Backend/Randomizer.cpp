#include "Randomizer.hpp"

#include "Backend/Mips.hpp"

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

void Randomizer::expandPacketArena() const
{
	static constexpr u16 ceiling{ 0xE000 }; // 0x801FE000, was 0x801FC000
	static constexpr u16 split{ 0x9000 }; // 0x801E9000, was 0x801E8000

	auto executable{ m_game->executable() };
	const auto ceilingOffset{ m_game->offset().file.executable.packetArenaCeiling };
	const auto splitOffset{ ceilingOffset + 0x48 };
	const auto ceilingOri{ executable.read<Mips_t>(ceilingOffset) };
	const auto splitOri{ executable.read<Mips_t>(splitOffset) };

	executable.write(ceilingOffset, static_cast<Mips_t>((ceilingOri & 0xFFFF0000) | ceiling));
	executable.write(splitOffset, static_cast<Mips_t>((splitOri & 0xFFFF0000) | split));
}