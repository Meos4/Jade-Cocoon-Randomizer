#include "Model.hpp"

namespace Model
{
	void CompressedThree::recompress(const DecompressedThree& d)
	{
		_2 = d._E;
		_4 = d._0;
		_5 = d._1;
		_6 = d._2;
		_8 = d._4;
		_9 = d._5;
		_A = d._6;
		_C = d._8;
		_D = d._9;
		_E = d._A;
		_10 = static_cast<u8>(d._18);
		_12 = d._1A;
		_14 = d._1C;
		_18 = d._30;
		_1A = d._32;
		_1C = d._34;
		_1E = static_cast<u8>(d._36);
		_1F = static_cast<u8>(d._36 >> 8);
		_20 = d._20;
		_22 = d._22;
		_24 = d._24;
		_28 = d._38;
		_2A = d._3A;
		_2C = d._3C;
		_2E = static_cast<u8>(d._3E);
		_2F = static_cast<u8>(d._3E >> 8);
		_30 = d._28;
		_32 = d._2A;
		_34 = d._2C;
		_38 = d._40;
		_3A = d._42;
		_3C = d._44;
		_3E = static_cast<u8>(d._46);
		_3F = static_cast<u8>(d._46 >> 8);
	}

	void CompressedFour::recompress(const DecompressedFour& d)
	{
		_0 = d._0;
		_1 = d._1;
		_2 = d._2;
		_4 = d._4;
		_6 = d._6;
		_8 = d._8;
		_C = d._C;
		_E = d._E;
		_10 = d._10;
		_14 = d._14;
		_16 = d._16;
		_18 = d._18;
		_1C = d._1C;
		_1E = d._1E;
		_20 = d._20;
		_24 = d._24;
		_26 = d._26;
		_28 = d._28;
	}

	void CompressedShadow::recompress(const DecompressedShadow& d)
	{
		_0 = d._A;
		_2 = d._6;
		_4 = d._10;
		_6 = d._12;
		_8 = d._14;
		_C = d._18;
		_E = d._1A;
		_10 = d._1C;
		_12 = static_cast<u8>(d._1E);
		_13 = static_cast<u8>(d._1E >> 8);
		_14 = d._20;
		_16 = d._22;
		_18 = d._24;
		_1A = static_cast<u8>(d._26);
		_1B = static_cast<u8>(d._26 >> 8);
		_1C = d._28;
		_1E = d._2A;
		_20 = d._2C;
		_22 = static_cast<u8>(d._2E);
		_23 = static_cast<u8>(d._2E >> 8);
		_24 = d._30;
		_26 = d._32;
		_28 = d._34;
		_2A = static_cast<u8>(d._36);
		_2B = static_cast<u8>(d._36 >> 8);
	}

	void CompressedSeven::recompress(const DecompressedSeven& d)
	{
		_0 = d._16;
		_2 = d._12;
		_4 = d._0;
		_5 = d._1;
		_6 = d._2;
		_8 = d._4;
		_9 = d._6;
		_C = d._8;
		_D = d._9;
		_E = d._A;
		_10 = d._C;
		_11 = d._D;
		_12 = d._E;
		_14 = d._1C;
		_16 = d._1E;
		_18 = d._20;
		_1C = d._3C;
		_1E = d._3E;
		_20 = d._40;
		_22 = static_cast<u8>(d._42);
		_23 = static_cast<u8>(d._42 >> 8);
		_24 = d._24;
		_26 = d._26;
		_28 = d._28;
		_2C = d._44;
		_2E = d._46;
		_30 = d._48;
		_32 = static_cast<u8>(d._4A);
		_33 = static_cast<u8>(d._4A >> 8);
		_34 = d._2C;
		_36 = d._2E;
		_38 = d._30;
		_3C = d._4C;
		_3E = d._4E;
		_40 = d._50;
		_42 = static_cast<u8>(d._52);
		_43 = static_cast<u8>(d._52 >> 8);
		_44 = d._34;
		_46 = d._36;
		_48 = d._38;
		_4C = d._54;
		_4E = d._56;
		_50 = d._58;
		_52 = static_cast<u8>(d._5A);
		_53 = static_cast<u8>(d._5A >> 8);
	}

	void CompressedNine::recompress(const DecompressedNine& d)
	{
		_0 = d._4;
		_2 = d._6;
		_4 = d._8;
		_8 = d._C;
		_A = d._E;
		_C = d._10;
		_E = d._12;
		_10 = d._0;
		_11 = d._1;
		_12 = d._2;
	}

	DecompressedThree::DecompressedThree(const CompressedThree& c, u8 _3_2)
	{
		_0 = c._4;
		_1 = c._5;
		_2 = c._6;
		_3 = _3_2;
		_4 = c._8;
		_5 = c._9;
		_6 = c._A;
		_7 = 0xEE;
		_8 = c._C;
		_9 = c._D;
		_A = c._E;
		_B = 0xEE;
		_C = c._1E;
		_D = c._1F;
		_E = c._2;
		_F = 0xEE;
		_10 = c._2E;
		_11 = c._2F;
		_12 = c._0;
		_14 = c._3E;
		_15 = c._3F;
		_16 = 0xEEEE;
		_18 = c._10;
		_1A = c._12;
		_1C = c._14;
		_1E = 0xEEEE;
		_20 = c._20;
		_22 = c._22;
		_24 = c._24;
		_26 = 0xEEEE;
		_28 = c._30;
		_2A = c._32;
		_2C = c._34;
		_2E = 0xEEEE;
		_30 = c._18;
		_32 = c._1A;
		_34 = c._1C;
		_36 = _C + (_D << 8);
		_38 = c._28;
		_3A = c._2A;
		_3C = c._2C;
		_3E = _10 + (_11 << 8);
		_40 = c._38;
		_42 = c._3A;
		_44 = c._3C;
		_46 = _14 + (_15 << 8);
	}

	DecompressedFour::DecompressedFour(const CompressedFour& c, u8 _3_2)
	{
		_0 = c._0;
		_1 = c._1;
		_2 = c._2;
		_3 = _3_2;
		_4 = c._4;
		_6 = c._6;
		_8 = c._8;
		_A = 0xEEEE;
		_C = c._C;
		_E = c._E;
		_10 = c._10;
		_12 = 0xEEEE;
		_14 = c._14;
		_16 = c._16;
		_18 = c._18;
		_1A = 0xEEEE;
		_1C = c._1C;
		_1E = c._1E;
		_20 = c._20;
		_22 = 0xEEEE;
		_24 = c._24;
		_26 = c._26;
		_28 = c._28;
	}

	DecompressedShadow::DecompressedShadow(const CompressedShadow& c, u8 _3_2)
	{
		_0 = 0x80;
		_1 = 0x80;
		_2 = 0x80;
		_3 = _3_2;
		_4 = c._12;
		_5 = c._13;
		_6 = c._2;
		_8 = c._1A;
		_9 = c._1B;
		_A = c._0;
		_C = c._22;
		_D = c._23;
		_E = c._2A;
		_F = c._2B;
		_10 = c._4;
		_12 = c._6;
		_14 = c._8;
		_16 = 0xEEEE;
		_18 = c._C;
		_1A = c._E;
		_1C = c._10;
		_1E = _4 + (_5 << 8);
		_20 = c._14;
		_22 = c._16;
		_24 = c._18;
		_26 = _8 + (_9 << 8);
		_28 = c._1C;
		_2A = c._1E;
		_2C = c._20;
		_2E = _C + (_D << 8);
		_30 = c._24;
		_32 = c._26;
		_34 = c._28;
		_36 = _E + (_F << 8);
	}

	DecompressedSeven::DecompressedSeven(const CompressedSeven& c, u8 _3_2)
	{
		_0 = c._4;
		_1 = c._5;
		_2 = c._6;
		_3 = _3_2;
		_4 = c._8;
		_5 = 0xEE;
		_6 = c._9;
		_7 = 0xEE;
		_8 = c._C;
		_9 = c._D;
		_A = c._E;
		_B = 0xEE;
		_C = c._10;
		_D = c._11;
		_E = c._12;
		_F = 0xEE;
		_10 = c._22;
		_11 = c._23;
		_12 = c._2;
		_14 = c._32;
		_15 = c._33;
		_16 = c._0;
		_18 = c._42;
		_19 = c._43;
		_1A = c._52;
		_1B = c._53;
		_1C = c._14;
		_1E = c._16;
		_20 = c._18;
		_22 = 0xEEEE;
		_24 = c._24;
		_26 = c._26;
		_28 = c._28;
		_2A = 0xEEEE;
		_2C = c._34;
		_2E = c._36;
		_30 = c._38;
		_32 = 0xEEEE;
		_34 = c._44;
		_36 = c._46;
		_38 = c._48;
		_3A = 0xEEEE;
		_3C = c._1C;
		_3E = c._1E;
		_40 = c._20;
		_42 = _10 + (_11 << 8);
		_44 = c._2C;
		_46 = c._2E;
		_48 = c._30;
		_4A = _14 + (_15 << 8);
		_4C = c._3C;
		_4E = c._3E;
		_50 = c._40;
		_52 = _18 + (_19 << 8);
		_54 = c._4C;
		_56 = c._4E;
		_58 = c._50;
		_5A = _1A + (_1B << 8);
	}

	DecompressedNine::DecompressedNine(const CompressedNine& c, u8 _3_2)
	{
		_0 = c._10;
		_1 = c._11;
		_2 = c._12;
		_3 = _3_2;
		_4 = c._0;
		_6 = c._2;
		_8 = c._4;
		_A = 0;
		_C = c._8;
		_E = c._A;
		_10 = c._C;
		_12 = c._E;
	}

	namespace Minion
	{
		namespace Animation
		{
			UnpackedOffsetSize::UnpackedOffsetSize(
				const std::array<std::pair<u16, u16>, Model::Minion::Animation::nbUnpacked>& offSizeSectors)
			{
				for (std::size_t i{}; i < Model::Minion::Animation::nbUnpacked; ++i)
				{
					m_offsetSize[i].first = offSizeSectors[i].first << 0xB;
					m_offsetSize[i].second = offSizeSectors[i].second << 0xB;
				}
			}

			u32 UnpackedOffsetSize::offset(u32 anim) const
			{
				return m_offsetSize.at(anim).first;
			}

			u32 UnpackedOffsetSize::size(u32 anim) const
			{
				return m_offsetSize.at(anim).second;
			}
		}
	}
}