#include "Fixes.hpp"

#include "Backend/Equipment.hpp"
#include "Backend/File.hpp"
#include "Backend/Item.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFunctionTypes.hpp"

#include <array>
#include <cmath>
#include <utility>

Fixes::Fixes(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData)
	: m_game(std::move(game)), m_sharedData(std::move(sharedData))
{
}

void Fixes::setBodyEnhancement() const
{
	const auto
		li32_minionStats{ Mips::li32(Mips::Register::v0, m_game->offset().game.minionStats) },
		li32_bossStatsEC{ Mips::li32(Mips::Register::v0, m_game->offset().game.bossStatsEC) };

	const auto fixEntityEnhancementOffset{ m_game->customCodeOffset(sizeof(MipsFn::FixEntityEnhancement)) };

	using EntityEnhancement = std::array<Mips_t, 7>;

	const EntityEnhancement entityEnhancementFn
	{
		0x27BDFFF0, // addiu sp, -0x10
		0xAFBF0000, // sw ra, 0(sp)
		Mips::j(fixEntityEnhancementOffset.game),
		0x00000000, // nop
		0x8FBF0000, // lw ra, 0(sp)
		0x03E00008, // jr ra
		0x27BD0010  // addiu sp, 0x10
	};

	const MipsFn::FixEntityEnhancement fixEntityEnhancementFn
	{
		li32_minionStats[0], // lui v0, 0xXXXX
		li32_minionStats[1], // ori v0, 0xXXXX
		0x2C880039, // sltiu t0, a0, 0x39
		0x11000003, // beqz t0, 3
		0x2C8800C9, // sltiu t0, a0, 0xC9
		0x1000000A, // b, +0xA
		0x00000000, // nop
		0x11000005, // beqz t0, 5
		0x00000000, // nop
		0x2484FFC7, // addiu a0 -0x39
		0x00042082, // srl a0, 2
		0x10000004, // b, 4
		0x00000000, // nop
		li32_bossStatsEC[0], // lui v0, 0xXXXX
		li32_bossStatsEC[1], // ori v0, 0xXXXX
		0x2484FF37, // addiu a0, -0xC9 
		0x000420C0, // sll a0, 3
		0x00822021, // move a0, v0
		Mips::j(m_game->offset().game.getEntityEnhancement + 0x10),
		0x90820007 // lbu v0, 7(a0)
	};

	auto executable{ m_game->executable() };

	executable.write(m_game->offset().file.executable.bodyEnhancementFn, entityEnhancementFn);
	executable.write(fixEntityEnhancementOffset.file, fixEntityEnhancementFn);
}

void Fixes::setAutumnMoonEffect() const
{
	Equipment::setDamageEffectFromWeaponIdFn(*m_game, true);
}

void Fixes::setHpMpBarsSize(u16 value) const
{
	static constexpr u16 defaultHpMpBars{ 47 };

	if (value != defaultHpMpBars)
	{
		const MipsFn::PreviewHeal previewHealFn
		{
			Mips::addiu(Mips::Register::v0, value % 2 ? 9 : 8),
			0x03E00008, // jr ra
			0x00551021  // addu v0, s5
		};

		auto executable{ m_game->executable() };

		const auto previewHealOffset{ m_game->customCodeOffset(sizeof(MipsFn::PreviewHeal)) };

		const auto li_menuLevantHpBar{ Mips::li(Mips::Register::s2,
			static_cast<u16>(std::round(87 * (static_cast<float>(value) / defaultHpMpBars)))) };

		executable.write(m_game->offset().file.executable.hpMpBars, Mips::li(Mips::Register::s2, value));
		executable.write(m_game->offset().file.executable.hpMpBars + 0x78, li_menuLevantHpBar);
		executable.write(previewHealOffset.file, previewHealFn);
		executable.write(m_game->offset().file.executable.hpMpBars + 0x108, Mips::jal(previewHealOffset.game));
	}
}

void Fixes::setSpecialsModifiers() const
{
	if (m_game->isVersion(Version::PalFr))
	{
		static constexpr u32 puissancePtr{ 0x800746D2 };

		static constexpr std::array<u8, 19> puissance
		{
			0x7D, 0x00, 0x75, 0x00, 0x52, 0x00, 0x15, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x4A, 0x00, 0x4F, 0x00,
			0x25, 0x00, 0x26
		};

		static constexpr std::array<u8, 23> precision
		{
			0x7D, 0x00, 0x75, 0x00, 0x29, 0x00, 0x39, 0x00, 0x25, 0x00, 0x15, 0x00, 0x2A, 0x00, 0x15, 0x00,
			0x28, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00
		};

		static constexpr std::array<u8, 21> critique
		{
			0x7D, 0x00, 0x6E, 0x00, 0x29, 0x00, 0x15, 0x00, 0x17, 0x00, 0x15, 0x00, 0x51, 0x00, 0x52, 0x00,
			0x26, 0x00, 0x00, 0x00, 0x00
		};

		auto executable{ m_game->executable() };

		executable.write(0x00073F20, puissancePtr);
		executable.write(0x00064ED2, puissance);
		executable.write(0x00064EB8, precision);
		executable.write(0x00064EA0, critique);
	}
}