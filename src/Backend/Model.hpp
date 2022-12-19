#pragma once

#include "Backend/Entity.hpp"
#include "Backend/File.hpp"
#include "Common/Types.hpp"

#include <array>
#include <utility>
#include <vector>

namespace Model
{
	using Type_t = u8;

	enum : Type_t
	{
		TYPE_THREE = 3,
		TYPE_FOUR = 4,
		TYPE_SHADOW = 6,
		TYPE_SEVEN = 7,
		TYPE_NINE = 9,
		TYPE_COUNT
	};

	struct DecompressedThree;
	struct DecompressedFour;
	struct DecompressedShadow;
	struct DecompressedSeven;
	struct DecompressedNine;

	struct CompressedThree
	{
		u16 _0; u16 _2; u8 _4; u8 _5;
		u8 _6; u8 _7; u8 _8; u8 _9;
		u8 _A; u8 _B; u8 _C; u8 _D;
		u8 _E; u8 _F; u8 _10; u8 _11;
		u16 _12; u16 _14; u16 _16;
		u16 _18; u16 _1A; u16 _1C; u8 _1E;
		u8 _1F; u16 _20; u16 _22; u16 _24;
		u16 _26; u16 _28; u16 _2A; u16 _2C;
		u8 _2E; u8 _2F; u16 _30; u16 _32;
		u16 _34; u16 _36; u16 _38; u16 _3A;
		u16 _3C; u8 _3E; u8 _3F;

		void recompress(const DecompressedThree& d);
	};

	struct CompressedFour
	{
		u8 _0; u8 _1; u8 _2; u8 _3;
		u16 _4; u16 _6; u16 _8; u16 _A;
		u16 _C; u16 _E; u16 _10; u16 _12;
		u16 _14; u16 _16; u16 _18; u16 _1A;
		u16 _1C; u16 _1E; u16 _20; u16 _22;
		u16 _24; u16 _26; u16 _28; u16 _2C;

		void recompress(const DecompressedFour& d);
	};

	struct CompressedShadow
	{
		u16 _0; u16 _2; u16 _4; u16 _6;
		u16 _8; u16 _A; u16 _C; u16 _E;
		u16 _10; u8 _12; u8 _13; u16 _14;
		u16 _16; u16 _18; u8 _1A; u8 _1B;
		u16 _1C; u16 _1E; u16 _20; u8 _22;
		u8 _23; u16 _24; u16 _26; u16 _28;
		u8 _2A; u8 _2B;

		void recompress(const DecompressedShadow& d);
	};

	struct CompressedSeven
	{
		u16 _0; u16 _2; u8 _4; u8 _5;
		u8 _6; u8 _7; u8 _8; u8 _9;
		u8 _A; u8 _B; u8 _C; u8 _D;
		u8 _E; u8 _F; u8 _10; u8 _11;
		u8 _12; u8 _13; u16 _14; u16 _16;
		u16 _18; u16 _1A; u16 _1C; u16 _1E;
		u16 _20; u8 _22; u8 _23; u16 _24;
		u16 _26; u16 _28; u16 _2A; u16 _2C;
		u16 _2E; u16 _30; u8 _32; u8 _33;
		u16 _34; u16 _36; u16 _38; u16 _3A;
		u16 _3C; u16 _3E; u16 _40; u8 _42;
		u8 _43; u16 _44; u16 _46; u16 _48;
		u16 _4A; u16 _4C; u16 _4E; u16 _50;
		u8 _52; u8 _53;

		void recompress(const DecompressedSeven& d);
	};

	struct CompressedNine
	{
		u16 _0; u16 _2; u16 _4; u16 _6;
		u16 _8; u16 _A; u16 _C; u16 _E;
		u8 _10; u8 _11; u8 _12;

		void recompress(const DecompressedNine& d);
	};

	struct DecompressedThree
	{
		u8 _0; u8 _1; u8 _2; u8 _3;
		u8 _4; u8 _5; u8 _6; u8 _7;
		u8 _8; u8 _9; u8 _A; u8 _B;
		u8 _C; u8 _D; u16 _E; u8 _F;
		u8 _10; u8 _11; u16 _12; u8 _14;
		u8 _15; u16 _16; u16 _18; u16 _1A;
		u16 _1C; u16 _1E; u16 _20; u16 _22;
		u16 _24; u16 _26; u16 _28; u16 _2A;
		u16 _2C; u16 _2E; u16 _30; u16 _32;
		u16 _34; u16 _36; u16 _38; u16 _3A;
		u16 _3C; u16 _3E; u16 _40; u16 _42;
		u16 _44; u16 _46;

		DecompressedThree(const CompressedThree& c, u8 _3_2);
	};

	struct DecompressedFour
	{
		u8 _0; u8 _1; u8 _2; u8 _3;
		u16 _4; u16 _6; u16 _8; u16 _A;
		u16 _C; u16 _E; u16 _10; u16 _12;
		u16 _14; u16 _16; u16 _18; u16 _1A;
		u16 _1C; u16 _1E; u16 _20; u16 _22;
		u16 _24; u16 _26; u16 _28;

		DecompressedFour(const CompressedFour& c, u8 _3_2);
	};

	struct DecompressedShadow
	{
		u8 _0; u8 _1; u8 _2; u8 _3;
		u8 _4; u8 _5; u16 _6; u8 _8;
		u8 _9; u16 _A; u8 _C; u8 _D;
		u8 _E; u8 _F; u16 _10; u16 _12;
		u16 _14; u16 _16; u16 _18; u16 _1A;
		u16 _1C; u16 _1E; u16 _20; u16 _22;
		u16 _24; u16 _26; u16 _28; u16 _2A;
		u16 _2C; u16 _2E; u16 _30; u16 _32;
		u16 _34; u16 _36;

		DecompressedShadow(const CompressedShadow& c, u8 _3_2);
	};

	struct DecompressedSeven
	{
		u8 _0; u8 _1; u8 _2; u8 _3;
		u8 _4; u8 _5; u8 _6; u8 _7;
		u8 _8; u8 _9; u8 _A; u8 _B;
		u8 _C; u8 _D; u8 _E; u8 _F;
		u8 _10; u8 _11; u16 _12; u8 _14;
		u8 _15;	u16 _16; u8 _18; u8 _19;
		u8 _1A; u8 _1B; u16 _1C; u16 _1E;
		u16 _20; u16 _22; u16 _24; u16 _26;
		u16 _28; u16 _2A; u16 _2C; u16 _2E;
		u16 _30; u16 _32; u16 _34; u16 _36;
		u16 _38; u16 _3A; u16 _3C; u16 _3E;
		u16 _40; u16 _42; u16 _44; u16 _46;
		u16 _48; u16 _4A; u16 _4C; u16 _4E;
		u16 _50; u16 _52; u16 _54; u16 _56;
		u16 _58; u16 _5A;

		DecompressedSeven(const CompressedSeven& c, u8 _3_2);
	};

	struct DecompressedNine
	{
		u8 _0; u8 _1; u8 _2; u8 _3;
		u16 _4; u16 _6; u16 _8; u16 _A;
		u16 _C; u16 _E; u16 _10; u16 _12;

		DecompressedNine(const CompressedNine& c, u8 _3_2);
	};

	static_assert(sizeof(CompressedThree) == 0x40, "Compressed Three Incomplete Type");
	static_assert(sizeof(CompressedFour) == 0x2C, "Compressed Four Incomplete Type");
	static_assert(sizeof(CompressedShadow) == 0x2C, "Compressed Shadow Incomplete Type");
	static_assert(sizeof(CompressedSeven) == 0x54, "Compressed Seven Incomplete Type");
	static_assert(sizeof(CompressedNine) == 0x14, "Compressed Nine Incomplete Type");

	namespace Minion
	{
		inline constexpr auto
			begin{ 0xA000u }, 
			headerSize{ 0x10u }, 
			nbParts{ 25u }, 
			nbUniqueModel{ 38u };

		struct Compressed
		{
			std::vector<Model::CompressedThree> three;
			std::vector<Model::CompressedFour> four;
			std::vector<Model::CompressedShadow> shadow;
			std::vector<Model::CompressedSeven> seven;
			std::vector<std::vector<Model::CompressedNine>> nine;
		};

		struct Decompressed
		{
			std::vector<Model::DecompressedThree> three;
			std::vector<Model::DecompressedFour> four;
			std::vector<Model::DecompressedShadow> shadow;
			std::vector<Model::DecompressedSeven> seven;
			std::vector<std::vector<Model::DecompressedNine>> nine;
		};

		inline constexpr std::array<s32, Model::Minion::nbUniqueModel> files
		{
			File::MODEL_DLEG_MDL, File::MODEL_DLEG_B_MDL, File::MODEL_DLEG_C_MDL, File::MODEL_DONG_MDL,
			File::MODEL_DONG_B_MDL, File::MODEL_DONG_C_MDL, File::MODEL_HANB_A_MDL, File::MODEL_HANB_B_MDL,
			File::MODEL_HANB_C_MDL, File::MODEL_HIRA_MDL, File::MODEL_HIRA_B_MDL, File::MODEL_HIRA_C_MDL,
			File::MODEL_IMEL_MDL, File::MODEL_IMEL_B_MDL, File::MODEL_IMEL_C_MDL, File::MODEL_LUCH_MDL,
			File::MODEL_LUCHB_MDL, File::MODEL_LUCHC_MDL, File::MODEL_SHABU_MDL, File::MODEL_SHABU_B_MDL,
			File::MODEL_SHABU_C_MDL, File::MODEL_OUMG_MDL, File::MODEL_GINU_MDL, File::MODEL_MAIE_MDL,
			File::MODEL_MUGU_MDL, File::MODEL_NEBUI_MDL, File::MODEL_OJE_MDL, File::MODEL_RIG_MDL,
			File::MODEL_THRE_MDL, File::MODEL_UGURAM_MDL, File::MODEL_YOLK_MDL, File::MODEL_ZIRA_MDL,
			File::MODEL_KAMERE_MDL, File::MODEL_ASHA_MDL, File::MODEL_KANI_MDL, File::MODEL_TORI_MDL,
			File::MODEL_ALPA_MDL, File::MODEL_PDA_MDL
		};

		inline constexpr std::array<s32, Entity::totalECBosses - 2> filesEC
		{
			File::MODEL_TX00_MDL, File::MODEL_TX01_MDL, File::MODEL_TX02_MDL, File::MODEL_TX03_MDL,
			File::MODEL_TX04_MDL, File::MODEL_TX05_MDL, File::MODEL_TX06_MDL, File::MODEL_TX07_MDL
		};

		inline constexpr std::array<Model_t, Model::Minion::nbUniqueModel> models
		{
			MODEL_DLEG, MODEL_DLEG_B, MODEL_DLEG_C, MODEL_DONG,
			MODEL_DONG_B, MODEL_DONG_C, MODEL_HANB_A, MODEL_HANB_B,
			MODEL_HANB_C, MODEL_HIRA, MODEL_HIRA_B, MODEL_HIRA_C,
			MODEL_IMEL, MODEL_IMEL_B, MODEL_IMEL_C, MODEL_LUCH,
			MODEL_LUCHB, MODEL_LUCHC, MODEL_SHABU, MODEL_SHABU_B,
			MODEL_SHABU_C, MODEL_OUMG, MODEL_GINU, MODEL_MAIE,
			MODEL_MUGU, MODEL_NEBUI, MODEL_OJE, MODEL_RIG,
			MODEL_THRE, MODEL_UGURAM, MODEL_YOLK, MODEL_ZIRA,
			MODEL_KAMERE, MODEL_ASHA, MODEL_KANI, MODEL_TORI,
			MODEL_ALPA, MODEL_PDA
		};

		inline constexpr std::array<Model_t, Entity::totalECBosses - 2> modelsEC
		{
			MODEL_TX00, MODEL_TX01, MODEL_TX02, MODEL_TX03,
			MODEL_TX04, MODEL_TX05, MODEL_TX06, MODEL_TX07
		};

		static_assert(Model::Minion::files.size() == Model::Minion::models.size(), "Incompatible model minion size");
		static_assert(Model::Minion::filesEC.size() == Model::Minion::modelsEC.size(), "Incompatible model EC size");

		namespace Texture
		{
			inline constexpr auto
				begin{ 0x182Cu }, 
				size{ 0x87D4u }, 
				clutBegin{ 0x1840u }, 
				clutSize{ 0x100u };
		}

		namespace Animation
		{
			inline constexpr auto
				begin{ 0x12800u }, 
				nbPacked{ 8u }, 
				nbUnpacked{ 21u }, 
				size{ 0xA2u };

			class UnpackedOffsetSize
			{
			public:
				UnpackedOffsetSize(
					const std::array<std::pair<u16, u16>, Model::Minion::Animation::nbUnpacked>& offSizeSectors);

				u32 offset(u32 anim) const;
				u32 size(u32 anim) const;
			private:
				std::array<std::pair<s32, s32>, Model::Minion::Animation::nbUnpacked> m_offsetSize;
			};			
		}
	}
}