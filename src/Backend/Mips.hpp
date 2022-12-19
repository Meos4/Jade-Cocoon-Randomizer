#pragma once

#include "Common/Types.hpp"

#include <array>

using Mips_t = u32;

namespace Mips
{
	enum class Register
	{
		pc, at, v0, v1,
		a0, a1, a2, a3,
		t0, t1, t2, t3,
		t4, t5, t6, t7,
		s0, s1, s2, s3,
		s4, s5, s6, s7,
		t8, t9, k0, k1,
		gp, sp, fp, ra
	};

	constexpr Mips_t li(Mips::Register rgt, u16 value)
	{
		return 0x24000000 + (static_cast<u8>(rgt) << 16) + value;
	}

	constexpr Mips_t lui(Mips::Register rgt, u16 value)
	{
		return 0x3C000000 + (static_cast<u8>(rgt) << 16) + value;
	}

	constexpr Mips_t ori(Mips::Register rgt, u16 value)
	{
		return 0x34000000 + ((static_cast<u8>(rgt) * 33) << 16) + value;
	}

	constexpr Mips_t addiu(Mips::Register rgt, u16 value)
	{
		return 0x24000000 + ((static_cast<u8>(rgt) * 33) << 16) + value;
	}

	constexpr std::array<Mips_t, 2> li32(Mips::Register rgt, u32 value)
	{
		return
		{ 
			lui(rgt, static_cast<u16>(value >> 16)), 
			ori(rgt, static_cast<u16>(value))
		};
	}

	constexpr Mips_t jal(u32 offset)
	{
		return 0x0C000000 + ((offset & ~0x80000000) >> 2);
	}

	constexpr Mips_t j(u32 offset)
	{
		return 0x08000000 + ((offset & ~0x80000000) >> 2);
	}
}