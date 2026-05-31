#include "Backend/Randomizer.hpp"

#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"

#include <cmath>

void Randomizer::fixesHpMpBarsSize(u16 value) const
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
