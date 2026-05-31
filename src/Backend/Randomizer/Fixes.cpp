#include "Backend/Randomizer.hpp"

#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"

void Randomizer::fixesHpMpBars() const
{
	auto executable{ m_game->executable() };

	const auto shift{ m_game->gameToFileTextSectionShift() };
	const auto hpMpBarsFile{ m_game->offset().file.executable.hpMpBars };
	const u32 hpMpBarsGame{ hpMpBarsFile + shift + 0x80000000 };
	const auto drawBlock{ executable.read<std::array<Mips_t, 15>>(hpMpBarsFile + 0x34C) };
	const auto hpMpBarsFixOffset{ m_game->customCodeOffset(sizeof(MipsFn::HpMpBarsFix)) };

	MipsFn::HpMpBarsFix hpMpBarsFixFn{};

	hpMpBarsFixFn[0] = 0x00108242;                      // srl   s0, s0, 9
	hpMpBarsFixFn[1] = 0x24080100;                      // addiu t0, zero
	hpMpBarsFixFn[2] = 0xAFA80010;                      // sw    t0, 0x10(sp)

	for (std::size_t i{ 0 }; i < drawBlock.size(); ++i)
	{
		hpMpBarsFixFn[3 + i] = (i == 10) ? Mips_t{ 0 } : drawBlock[i]; // nop the 'sw s0,0x10(sp)'
	}

	hpMpBarsFixFn[18] = 0x86220000;                     // lh    v0, (s1)
	hpMpBarsFixFn[19] = 0x00000000;                     // nop
	hpMpBarsFixFn[20] = 0x24420001;                     // addiu v0, v0, 1
	hpMpBarsFixFn[21] = 0xA6220000;                     // sh    v0, (s1)
	hpMpBarsFixFn[22] = 0x2610FFFF;                     // addiu s0, s0, -1
	hpMpBarsFixFn[23] = 0x1E00FFEB;                     // bgtz  s0, loop
	hpMpBarsFixFn[24] = 0x00000000;                     // nop
	hpMpBarsFixFn[25] = Mips::j(hpMpBarsGame + 0x388);  // j
	hpMpBarsFixFn[26] = 0x00000000;                     // nop

	executable.write(hpMpBarsFixOffset.file, hpMpBarsFixFn);
	executable.write(hpMpBarsFile + 0x34C, Mips::j(hpMpBarsFixOffset.game));
}
