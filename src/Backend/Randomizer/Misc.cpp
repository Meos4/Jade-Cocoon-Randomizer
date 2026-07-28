#include "Backend/Randomizer.hpp"

#include "Backend/File.hpp"
#include "Backend/Resource.hpp"
#include "Backend/Mips.hpp"
#include "Backend/MipsFn.hpp"
#include "Backend/Version.hpp"
#include "Common/JcrException.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <utility>
#include <vector>

Randomizer::HudColorArray Randomizer::hudColor() const
{
	return m_game->staticExecutable().read<Randomizer::HudColorArray>(m_game->offset().file.executable.hudColors);
}

void Randomizer::miscHudColor() const
{
	Randomizer::HudColorArray hudColor;
	for (auto& color : hudColor)
	{
		color = m_game->random()->generate(std::numeric_limits<u32>::max()) & 0x00FFFFFF;
	}
	miscHudColor(hudColor);
}

void Randomizer::miscHudColor(const Randomizer::HudColorArray& hud) const
{
	m_game->executable().write(m_game->offset().file.executable.hudColors, hud);
}

void Randomizer::miscNPCsVoice(bool anyCharacter) const
{
	struct VoiceBehavior
	{
		u8 dialogueId, subDialogueId;
	};

	struct Voice
	{
		s16 xaPosition, xaPosition2, duration;

		bool operator==(const Voice& other) const = default;
	};

	struct FileVoiceInfo
	{
		std::unique_ptr<RawFile> file;
		u32 offset;
		u32 nbVoices;
	};

	struct SceneSpeaker
	{
		u8 file;
		u8 dialogue;
		u16 speaker;
	};

	static constexpr u16 speakerMuAndRa{ 0x100u };

	static constexpr std::array<SceneSpeaker, 620> sceneSpeakers
	{{
		{  0,   0, 0x2Fu }, {  0,   1, 0x2Cu }, {  0,   2, 0x2Fu }, {  0,   3, 0x2Cu }, {  0,   4, 0x2Fu },
		{  1,   0, 0x2Cu }, {  1,   1, 0x2Cu }, {  2,   0, 0x38u }, {  2,   1, 0x38u }, {  2,   2, 0x38u },
		{  2,   3, 0x38u }, {  3,   0, 0x2Fu }, {  3,   1, 0x2Cu }, {  3,   2, 0x47u }, {  3,   3, 0x47u },
		{  4,   0, 0x38u }, {  4,   1, 0x38u }, {  4,   3, 0x38u }, {  4,   4, 0x38u }, {  4,   5, 0x24u },
		{  5,   0, 0x66u }, {  5,   1, 0x64u }, {  5,   2, 0x16u }, {  5,   3, 0x66u }, {  5,   4, 0x16u },
		{  5,   5, 0x64u }, {  5,   6, 0x16u }, {  5,   7, 0x64u }, {  5,   8, 0x16u }, {  5,   9, 0x64u },
		{  5,  10, 0x66u }, {  5,  11, 0x16u }, {  5,  12, 0x64u }, {  5,  13, 0x16u }, {  5,  14, 0x64u },
		{  5,  15, 0x66u }, {  5,  16, 0x16u }, {  5,  17, 0x66u }, {  6,   0, 0x66u }, {  6,   1, 0x66u },
		{  6,   2, 0x66u }, {  6,   3, 0x16u }, {  6,   4, 0x16u }, {  6,   5, 0x16u }, {  6,   7, 0x16u },
		{  6,   8, 0x16u }, {  6,   9, 0x16u }, {  6,  10, 0x16u }, {  6,  11, 0x16u }, {  7,   0, 0x2Fu },
		{  7,   1, 0x2Fu }, {  7,   2, 0x2Cu }, {  7,   3, 0x2Fu }, {  7,   4, 0x2Cu }, {  7,   5, 0x2Fu },
		{  7,   6, 0x2Fu }, {  8,   0, 0x47u }, {  8,   1, 0x47u }, {  8,   2, 0x47u }, {  9,   0, 0x38u },
		{  9,   1, 0x38u }, {  9,   2, 0x38u }, {  9,   3, 0x38u }, {  9,   4, 0x38u }, {  9,   6, 0x38u },
		{  9,   7, 0x38u }, {  9,   8, 0x38u }, {  9,   9, 0x38u }, {  9,  10, 0x38u }, {  9,  11, 0x38u },
		{  9,  12, 0x38u }, {  9,  13, 0x38u }, { 10,   0, 0x16u }, { 10,   1, 0x16u }, { 10,   4, 0x16u },
		{ 10,   5, 0x2Fu }, { 10,   6, 0x2Fu }, { 11,   0, 0x2Fu }, { 11,   1, 0x2Fu }, { 11,   3, 0x2Fu },
		{ 11,   4, 0x2Fu }, { 12,   0, 0x15u }, { 12,   1, 0x37u }, { 12,   2, 0x37u }, { 12,   3, 0x37u },
		{ 12,   4, 0x37u }, { 12,   5, 0x37u }, { 12,   6, 0x15u }, { 12,   8, 0x15u }, { 12,   9, 0x15u },
		{ 12,  10, 0x15u }, { 12,  11, 0x15u }, { 13,   0, 0x2Cu }, { 13,   1, 0x24u }, { 13,   2, 0x24u },
		{ 13,   3, 0x2Cu }, { 13,   5, 0x2Cu }, { 13,   6, 0x2Cu }, { 13,   7, 0x2Cu }, { 13,   8, 0x24u },
		{ 13,   9, 0x24u }, { 13,  10, 0x24u }, { 13,  11, 0x24u }, { 13,  12, 0x2Cu }, { 13,  13, 0x2Cu },
		{ 14,   0, 0x1Eu }, { 14,   2, 0x1Eu }, { 14,   3, 0x1Eu }, { 14,   5, 0x1Eu }, { 14,   6, 0x1Eu },
		{ 15,   0, 0x2Cu }, { 15,   1, 0x2Cu }, { 15,   2, 0x2Cu }, { 16,   0, 0x26u }, { 16,   1, 0x26u },
		{ 16,   2, 0x26u }, { 16,   3, 0x26u }, { 16,   5, 0x26u }, { 16,   6, 0x26u }, { 16,   7, 0x26u },
		{ 16,   8, 0x26u }, { 16,  10, 0x26u }, { 16,  11, 0x26u }, { 16,  12, 0x26u }, { 16,  13, 0x26u },
		{ 16,  14, 0x26u }, { 16,  15, 0x26u }, { 16,  16, 0x26u }, { 16,  17, 0x26u }, { 16,  18, 0x26u },
		{ 16,  19, 0x26u }, { 16,  20, 0x26u }, { 16,  21, 0x26u }, { 16,  22, 0x26u }, { 16,  23, 0x26u },
		{ 17,   0, 0x16u }, { 17,   3, 0x16u }, { 17,   4, 0x16u }, { 17,   5, 0x16u }, { 17,   6, 0x16u },
		{ 17,   7, 0x16u }, { 17,   8, 0x16u }, { 17,   9, 0x16u }, { 17,  10, 0x16u }, { 17,  11, 0x16u },
		{ 17,  12, 0x16u }, { 17,  13, 0x16u }, { 17,  14, 0x16u }, { 17,  15, 0x16u }, { 17,  16, 0x16u },
		{ 17,  17, 0x16u }, { 17,  18, 0x16u }, { 17,  19, 0x2Fu }, { 17,  21, 0x2Fu }, { 17,  22, 0x2Fu },
		{ 17,  24, 0x2Fu }, { 17,  27, 0x2Fu }, { 17,  29, 0x2Fu }, { 17,  31, 0x2Fu }, { 17,  34, 0x2Fu },
		{ 18,   0, 0x2Cu }, { 18,   1, 0x2Cu }, { 18,   2, 0x2Cu }, { 18,   3, 0x2Cu }, { 18,   4, 0x2Cu },
		{ 19,   0, 0x26u }, { 19,   1, 0x26u }, { 19,   3, 0x26u }, { 19,   4, 0x26u }, { 19,   5, 0x26u },
		{ 19,   6, 0x26u }, { 19,   7, 0x26u }, { 19,   9, 0x26u }, { 19,  10, 0x26u }, { 19,  11, 0x26u },
		{ 19,  12, 0x26u }, { 19,  13, 0x26u }, { 20,   0, 0x16u }, { 20,   2, 0x16u }, { 20,   3, 0x16u },
		{ 20,   4, 0x16u }, { 20,   5, 0x16u }, { 20,   6, 0x16u }, { 20,   7, 0x16u }, { 20,   8, 0x16u },
		{ 20,   9, 0x2Fu }, { 20,  11, 0x2Fu }, { 20,  12, 0x2Fu }, { 20,  13, 0x2Fu }, { 20,  15, 0x2Fu },
		{ 20,  18, 0x2Fu }, { 20,  20, 0x2Fu }, { 20,  22, 0x2Fu }, { 20,  25, 0x2Fu }, { 21,   0, 0x15u },
		{ 21,   1, 0x15u }, { 21,   2, 0x15u }, { 21,   3, 0x15u }, { 21,   4, 0x15u }, { 22,   0, 0x2Cu },
		{ 22,   1, 0x24u }, { 22,   2, 0x2Cu }, { 22,   3, 0x2Cu }, { 22,   4, 0x2Cu }, { 22,   5, 0x2Cu },
		{ 22,   6, 0x24u }, { 22,   7, 0x24u }, { 22,   8, 0x24u }, { 22,   9, 0x24u }, { 22,  11, 0x24u },
		{ 23,   0, 0x1Eu }, { 23,   3, 0x1Eu }, { 23,   4, 0x1Eu }, { 23,   5, 0x1Eu }, { 23,   6, 0x1Eu },
		{ 24,   0, 0x3Eu }, { 24,   1, 0x3Eu }, { 25,   2, 0x25u }, { 25,   3, 0x25u }, { 26,   0, 0x25u },
		{ 26,   1, 0x25u }, { 26,   3, 0x25u }, { 26,   6, 0x25u }, { 26,   7, 0x25u }, { 26,   8, 0x25u },
		{ 26,   9, 0x25u }, { 26,  11, 0x25u }, { 26,  12, 0x25u }, { 26,  13, 0x25u }, { 26,  14, 0x25u },
		{ 26,  15, 0x25u }, { 26,  16, 0x25u }, { 26,  17, 0x25u }, { 26,  18, 0x25u }, { 26,  20, 0x25u },
		{ 26,  21, 0x25u }, { 27,   0, 0x16u }, { 27,   1, 0x16u }, { 27,   2, 0x16u }, { 27,   3, 0x16u },
		{ 27,   5, 0x16u }, { 27,   6, 0x16u }, { 27,   7, 0x16u }, { 27,   8, 0x16u }, { 27,   9, 0x16u },
		{ 27,  10, 0x2Fu }, { 27,  11, 0x2Fu }, { 27,  12, 0x2Fu }, { 27,  14, 0x2Fu }, { 27,  15, 0x2Fu },
		{ 27,  16, 0x16u }, { 27,  18, 0x16u }, { 27,  19, 0x2Fu }, { 27,  20, 0x2Fu }, { 27,  21, 0x2Fu },
		{ 27,  22, 0x2Fu }, { 27,  24, 0x2Fu }, { 27,  27, 0x2Fu }, { 27,  29, 0x2Fu }, { 27,  31, 0x2Fu },
		{ 27,  34, 0x2Fu }, { 28,   0, 0x2Fu }, { 28,   2, 0x2Fu }, { 28,   3, 0x2Fu }, { 28,   4, 0x2Fu },
		{ 28,   5, 0x2Fu }, { 28,   6, 0x2Fu }, { 28,   7, 0x2Fu }, { 28,   8, 0x2Fu }, { 28,   9, 0x2Fu },
		{ 28,  10, 0x2Fu }, { 28,  11, 0x2Fu }, { 28,  12, 0x2Fu }, { 28,  13, 0x2Fu }, { 28,  14, 0x2Fu },
		{ 29,   0, 0x38u }, { 29,   2, 0x38u }, { 29,   3, 0x38u }, { 29,   4, 0x38u }, { 29,   5, 0x38u },
		{ 29,   6, 0x38u }, { 29,   7, 0x38u }, { 29,   8, 0x38u }, { 29,   9, 0x38u }, { 30,   0, 0x15u },
		{ 30,   3, 0x15u }, { 30,   4, 0x15u }, { 30,   5, 0x37u }, { 30,   6, 0x37u }, { 30,   7, 0x15u },
		{ 30,   8, 0x37u }, { 30,   9, 0x37u }, { 30,  10, 0x37u }, { 30,  11, 0x37u }, { 30,  12, 0x37u },
		{ 30,  13, 0x15u }, { 30,  14, 0x15u }, { 30,  15, 0x15u }, { 31,   0, 0x2Cu }, { 31,   1, 0x24u },
		{ 31,   2, 0x2Cu }, { 31,   3, 0x2Cu }, { 31,   4, 0x2Cu }, { 31,   5, 0x24u }, { 31,   6, 0x2Cu },
		{ 31,   7, 0x24u }, { 31,   8, 0x2Cu }, { 31,   9, 0x24u }, { 31,  10, 0x24u }, { 31,  11, 0x24u },
		{ 32,   4, 0x1Eu }, { 32,   6, 0x1Eu }, { 32,   7, 0x1Eu }, { 32,   8, 0x1Eu }, { 33,   0, 0x5Fu },
		{ 33,   2, 0x5Fu }, { 33,   3, 0x5Fu }, { 33,   4, 0x5Fu }, { 33,   5, 0x5Fu }, { 33,   7, 0x5Fu },
		{ 33,   8, 0x5Fu }, { 33,   9, 0x5Fu }, { 33,  10, 0x5Fu }, { 33,  11, 0x5Fu }, { 34,   0, 0x60u },
		{ 34,   2, 0x60u }, { 34,   3, 0x60u }, { 34,   4, 0x60u }, { 34,   5, 0x60u }, { 34,   7, 0x60u },
		{ 34,   8, 0x60u }, { 34,   9, 0x60u }, { 34,  10, 0x60u }, { 34,  11, 0x60u }, { 35,   0, 0x3Eu },
		{ 35,   1, 0x3Eu }, { 36,   0, 0x5Eu }, { 36,   2, 0x5Eu }, { 36,   3, 0x5Eu }, { 36,   4, 0x5Eu },
		{ 36,   5, 0x5Eu }, { 36,   7, 0x5Eu }, { 36,   8, 0x5Eu }, { 36,   9, 0x5Eu }, { 36,  10, 0x5Eu },
		{ 36,  11, 0x5Eu }, { 37,   0, 0x61u }, { 37,   2, 0x61u }, { 37,   3, 0x61u }, { 37,   4, 0x61u },
		{ 37,   5, 0x61u }, { 37,   7, 0x61u }, { 37,   9, 0x61u }, { 37,  10, 0x61u }, { 37,  11, 0x61u },
		{ 37,  12, 0x61u }, { 37,  13, 0x61u }, { 37,  14, 0x61u }, { 37,  15, 0x61u }, { 37,  16, 0x61u },
		{ 38,   0, 0x16u }, { 38,   1, 0x16u }, { 38,   2, 0x16u }, { 38,   4, 0x16u }, { 38,   5, 0x16u },
		{ 38,   6, 0x16u }, { 38,   7, 0x16u }, { 38,   8, 0x16u }, { 38,   9, 0x16u }, { 38,  10, 0x16u },
		{ 38,  11, 0x16u }, { 38,  12, 0x16u }, { 38,  13, 0x16u }, { 38,  14, 0x2Fu }, { 38,  15, 0x2Fu },
		{ 38,  16, 0x2Fu }, { 38,  18, 0x2Fu }, { 38,  20, 0x2Fu }, { 38,  23, 0x2Fu }, { 38,  25, 0x2Fu },
		{ 38,  27, 0x2Fu }, { 38,  30, 0x2Fu }, { 39,   0, 0x64u }, { 39,   1, 0x64u }, { 39,   2, 0x66u },
		{ 39,   3, 0x64u }, { 39,   4, 0x66u }, { 39,   5, 0x64u }, { 39,   6, 0x66u }, { 40,   0, 0x38u },
		{ 40,   2, 0x38u }, { 40,   3, 0x38u }, { 40,   4, 0x38u }, { 40,   5, 0x38u }, { 40,   6, 0x38u },
		{ 40,   7, 0x38u }, { 40,   8, 0x38u }, { 40,   9, 0x38u }, { 40,  10, 0x38u }, { 41,   0, 0x15u },
		{ 41,   2, 0x15u }, { 41,   3, 0x24u }, { 41,   4, 0x24u }, { 41,   5, 0x15u }, { 41,   6, 0x24u },
		{ 41,   7, 0x24u }, { 41,   8, 0x24u }, { 41,   9, 0x15u }, { 41,  10, 0x15u }, { 41,  11, 0x15u },
		{ 42,   0, 0x2Cu }, { 42,   1, 0x2Cu }, { 42,   2, 0x2Cu }, { 42,   3, 0x2Cu }, { 42,   4, 0x2Cu },
		{ 42,   5, 0x2Cu }, { 43,   0, 0x1Eu }, { 43,   2, 0x1Eu }, { 43,   3, 0x1Eu }, { 43,   4, 0x1Eu },
		{ 43,   5, 0x1Eu }, { 43,   6, 0x1Eu }, { 44,   0, 0x5Fu }, { 44,   1, 0x5Fu }, { 44,   2, 0x5Fu },
		{ 44,   3, 0x5Fu }, { 45,   0, 0x60u }, { 45,   1, 0x60u }, { 45,   2, 0x60u }, { 45,   3, 0x60u },
		{ 46,   0, 0x5Eu }, { 46,   1, 0x5Eu }, { 46,   2, 0x5Eu }, { 46,   3, 0x5Eu }, { 47,   0, 0x61u },
		{ 47,   2, 0x61u }, { 47,   3, 0x61u }, { 48,   0, 0x61u }, { 49,   0, 0x2Fu }, { 49,   1, 0x2Fu },
		{ 49,   2, 0x2Fu }, { 49,   4, 0x2Fu }, { 49,   5, 0x2Fu }, { 49,   6, 0x2Fu }, { 49,   8, 0x2Fu },
		{ 49,  11, 0x2Fu }, { 49,  13, 0x2Fu }, { 49,  15, 0x2Fu }, { 49,  18, 0x2Fu }, { 50,   0, 0x15u },
		{ 50,   2, 0x15u }, { 50,   3, 0x24u }, { 50,   4, 0x24u }, { 50,   5, 0x15u }, { 50,   6, 0x24u },
		{ 50,   7, 0x15u }, { 51,   0, 0x64u }, { 51,   1, 0x16u }, { 51,   2, 0x64u }, { 51,   3, 0x64u },
		{ 51,   4, 0x64u }, { 51,   5, 0x64u }, { 52,   1, 0x2Fu }, { 52,   2, 0x2Fu }, { 52,   3, 0x2Fu },
		{ 52,   4, 0x16u }, { 52,   5, 0x16u }, { 52,   6, 0x16u }, { 52,   8, 0x16u }, { 52,   9, 0x16u },
		{ 52,  10, 0x16u }, { 53,   0, 0x2Fu }, { 54,   0, 0x5Fu }, { 55,   0, 0x60u }, { 56,   0, 0x5Eu },
		{ 57,   0, 0x3Eu }, { 57,   1, 0x3Eu }, { 58,   1, 0x2Fu }, { 59,   0, 0x2Fu }, { 60,   0, 0x3Fu },
		{ 60,   1, 0x3Fu }, { 60,   2, 0x3Fu }, { 60,   3, 0x3Fu }, { 60,   4, 0x3Fu }, { 61,   0, 0x3Fu },
		{ 61,   1, 0x3Fu }, { 61,   2, 0x3Fu }, { 61,   3, 0x3Fu }, { 61,   4, 0x3Fu }, { 61,   5, 0x3Fu },
		{ 61,   6, 0x42u }, { 61,   7, 0x41u }, { 61,   8, 0x3Fu }, { 61,   9, 0x3Fu }, { 62,   0, 0x3Fu },
		{ 62,   2, 0x3Fu }, { 62,   3, 0x3Fu }, { 62,   4, 0x3Fu }, { 62,   5, 0x3Fu }, { 62,   6, 0x3Fu },
		{ 62,   7, 0x3Fu }, { 62,   8, 0x3Fu }, { 62,   9, 0x3Fu }, { 62,  10, 0x3Fu }, { 62,  11, 0x3Fu },
		{ 62,  12, 0x3Fu }, { 63,   0, 0x42u }, { 63,   4, 0x42u }, { 63,   5, 0x42u }, { 63,   6, 0x42u },
		{ 63,   8, 0x42u }, { 63,  11, 0x42u }, { 63,  13, 0x42u }, { 63,  15, 0x42u }, { 63,  18, 0x42u },
		{ 64,   0, 0x41u }, { 64,   2, 0x41u }, { 64,   3, 0x41u }, { 64,   4, 0x41u }, { 65,   0, 0x3Fu },
		{ 65,   1, 0x3Fu }, { 65,   2, 0x3Fu }, { 65,   3, 0x3Fu }, { 65,   5, 0x3Fu }, { 65,   6, 0x3Fu },
		{ 65,   7, 0x3Fu }, { 65,   8, 0x3Fu }, { 65,   9, 0x3Fu }, { 65,  10, 0x3Fu }, { 65,  11, 0x3Fu },
		{ 65,  12, 0x3Fu }, { 65,  13, 0x3Fu }, { 65,  14, 0x3Fu }, { 65,  15, 0x3Fu }, { 66,   0, 0x3Fu },
		{ 66,   1, 0x3Fu }, { 66,   2, 0x3Fu }, { 66,   3, 0x3Fu }, { 66,   5, 0x3Fu }, { 66,   6, 0x3Fu },
		{ 66,   7, 0x3Fu }, { 66,   8, 0x3Fu }, { 66,   9, 0x3Fu }, { 66,  10, 0x3Fu }, { 66,  11, 0x3Fu },
		{ 66,  12, 0x3Fu }, { 66,  14, 0x3Fu }, { 67,   0, 0x26u }, { 67,   1, 0x26u }, { 67,   2, 0x02u },
		{ 67,   3, 0x02u }, { 67,   4, 0x26u }, { 67,   6, 0x02u }, { 68,   0, 0x25u }, { 68,   1, 0x24u },
		{ 68,   2, 0x06u }, { 68,   3, 0x06u }, { 68,   4, 0x24u }, { 68,   6, 0x06u }, { 69,   0, 0x5Fu },
		{ 69,   1, 0x04u }, { 69,   2, 0x04u }, { 69,   3, 0x04u }, { 69,   4, 0x04u }, { 70,   0, 0x60u },
		{ 70,   1, 0x04u }, { 70,   2, 0x04u }, { 70,   3, 0x04u }, { 70,   4, 0x04u }, { 71,   0, 0x5Eu },
		{ 71,   1, 0x04u }, { 71,   2, 0x04u }, { 71,   3, 0x04u }, { 71,   4, 0x04u }, { 72,   0, 0x61u },
		{ 72,   1, 0x2Fu }, { 72,   2, 0x2Fu }, { 72,   3, 0x2Fu }, { 72,   4, 0x2Fu }, { 72,   5, 0x04u },
		{ 72,   6, 0x04u }, { 72,   7, 0x04u }, { 72,   8, 0x04u }, { 72,   9, 0x2Fu }, { 72,  10, 0x2Fu },
		{ 72,  11, 0x2Fu }, { 72,  12, 0x04u }, { 73,   0, 0x47u }, { 73,   1, 0x47u }, { 73,   2, 0x47u },
		{ 73,   3, 0x47u }, { 73,   4, 0x47u }, { 73,   5, 0x47u }, { 73,   6, 0x47u }, { 73,   7, 0x47u },
		{ 73,   8, 0x47u }, { 74,   0, 0x42u }, { 74,   1, 0x41u }, { 74,   2, 0x41u }, { 74,   3, 0x42u },
		{ 74,   4, 0x41u }, { 74,   5, 0x42u }, { 74,   6, 0x41u }, { 74,   7, 0x42u }, { 75,   8, speakerMuAndRa },
		{ 75,   9, 0x41u }, { 75,  10, 0x41u }, { 75,  11, 0x42u }, { 75,  12, 0x42u }, { 75,  13, speakerMuAndRa },
		{ 76,   0, 0x41u }, { 76,   2, 0x41u }, { 77,   0, 0x42u }, { 77,   2, 0x42u }, { 77,   4, 0x42u },
		{ 77,   7, 0x42u }, { 77,   9, 0x42u }, { 77,  11, 0x42u }, { 77,  12, 0x42u }, { 77,  14, 0x42u },
	}};

	const auto& offsetF{ m_game->offset().file };

	const std::array<FileVoiceInfo, 78> filesInfo
	{{
		{ m_game->file(File::SCENE_PSYRUS2_LOOKOUT_SCE01_SBH), offsetF.scene_psyrus2_lookout_sce01_sbh.tableOfVoices, 15 },
		{ m_game->file(File::SCENE_PSYRUS2_LOOKOUT_SCE00A_SBH), offsetF.scene_psyrus2_lookout_sce00a_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS2_LEBANT_SCE00_SBH), offsetF.scene_psyrus2_lebant_sce00_sbh.tableOfVoices, 7 },
		{ m_game->file(File::SCENE_OTHER_DREAM_SCE00_SBH), offsetF.scene_other_dream_sce00_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00_SBH), offsetF.scene_psyrus1_lebant_sce00_sbh.tableOfVoices, 16 },
		{ m_game->file(File::SCENE_PSYRUS1_ZOKUCHO_SCE01_SBH), offsetF.scene_psyrus1_zokucho_sce01_sbh.tableOfVoices, 44 },
		{ m_game->file(File::SCENE_PSYRUS1_ZOKUCHO_SCE05_SBH), offsetF.scene_psyrus1_zokucho_sce05_sbh.tableOfVoices, 30 },
		{ m_game->file(File::SCENE_PSYRUS2_LOOKOUT_SCE01A_SBH), offsetF.scene_psyrus2_lookout_sce01a_sbh.tableOfVoices, 18 },
		{ m_game->file(File::SCENE_OTHER_DREAM_SCE00A_SBH), offsetF.scene_other_dream_sce00a_sbh.tableOfVoices, 7 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00A_SBH), offsetF.scene_psyrus1_lebant_sce00a_sbh.tableOfVoices, 37 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE00_SBH), offsetF.scene_psyrus1_garai_sce00_sbh.tableOfVoices, 25 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE03_SBH), offsetF.scene_psyrus1_garai_sce03_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00_SBH), offsetF.scene_psyrus1_kajiya_sce00_sbh.tableOfVoices, 26 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00_SBH), offsetF.scene_psyrus1_lookout_sce00_sbh.tableOfVoices, 28 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00_SBH), offsetF.scene_psyrus1_cemetery_sce00_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD1_GATE_SCE01_SBH), offsetF.scene_field1_gate_sce01_sbh.tableOfVoices, 5 },
		{ m_game->file(File::SCENE_FIELD1_GATE_SCE00A_SBH), offsetF.scene_field1_gate_sce00a_sbh.tableOfVoices, 80 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01_SBH), offsetF.scene_psyrus1_garai_sce01_sbh.tableOfVoices, 92 },
		{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE06A_SBH), offsetF.scene_field1_forest1_sce06a_sbh.tableOfVoices, 7 },
		{ m_game->file(File::SCENE_FIELD1_FOREST1_SCE09_SBH), offsetF.scene_field1_forest1_sce09_sbh.tableOfVoices, 44 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01A_SBH), offsetF.scene_psyrus1_garai_sce01a_sbh.tableOfVoices, 51 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00A_SBH), offsetF.scene_psyrus1_kajiya_sce00a_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00A_SBH), offsetF.scene_psyrus1_lookout_sce00a_sbh.tableOfVoices, 21 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00A_SBH), offsetF.scene_psyrus1_cemetery_sce00a_sbh.tableOfVoices, 22 },
		{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE13A_SBH), offsetF.scene_field1_forest2_sce13a_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE14A_SBH), offsetF.scene_field1_forest2_sce14a_sbh.tableOfVoices, 5 },
		{ m_game->file(File::SCENE_FIELD1_FOREST2_SCE07_SBH), offsetF.scene_field1_forest2_sce07_sbh.tableOfVoices, 81 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01B_SBH), offsetF.scene_psyrus1_garai_sce01b_sbh.tableOfVoices, 56 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE01_SBH), offsetF.scene_psyrus1_lookout_sce01_sbh.tableOfVoices, 40 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00C_SBH), offsetF.scene_psyrus1_lebant_sce00c_sbh.tableOfVoices, 39 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00B_SBH), offsetF.scene_psyrus1_kajiya_sce00b_sbh.tableOfVoices, 33 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00B_SBH), offsetF.scene_psyrus1_lookout_sce00b_sbh.tableOfVoices, 27 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00B_SBH), offsetF.scene_psyrus1_cemetery_sce00b_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE11_SBH), offsetF.scene_field1_forest3_sce11_sbh.tableOfVoices, 12 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE12_SBH), offsetF.scene_field1_forest3_sce12_sbh.tableOfVoices, 13 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE03A_SBH), offsetF.scene_field1_forest3_sce03a_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE13_SBH), offsetF.scene_field1_forest3_sce13_sbh.tableOfVoices, 19 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE14_SBH), offsetF.scene_field1_forest3_sce14_sbh.tableOfVoices, 40 },
		{ m_game->file(File::SCENE_PSYRUS1_GARAI_SCE01C_SBH), offsetF.scene_psyrus1_garai_sce01c_sbh.tableOfVoices, 54 },
		{ m_game->file(File::SCENE_PSYRUS1_ZOKUCHO_SCE01A_SBH), offsetF.scene_psyrus1_zokucho_sce01a_sbh.tableOfVoices, 29 },
		{ m_game->file(File::SCENE_PSYRUS1_LEBANT_SCE00D_SBH), offsetF.scene_psyrus1_lebant_sce00d_sbh.tableOfVoices, 27 },
		{ m_game->file(File::SCENE_PSYRUS1_KAJIYA_SCE00C_SBH), offsetF.scene_psyrus1_kajiya_sce00c_sbh.tableOfVoices, 17 },
		{ m_game->file(File::SCENE_PSYRUS1_LOOKOUT_SCE00C_SBH), offsetF.scene_psyrus1_lookout_sce00c_sbh.tableOfVoices, 8 },
		{ m_game->file(File::SCENE_PSYRUS1_CEMETERY_SCE00C_SBH), offsetF.scene_psyrus1_cemetery_sce00c_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE11A_SBH), offsetF.scene_field1_forest3_sce11a_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE12A_SBH), offsetF.scene_field1_forest3_sce12a_sbh.tableOfVoices, 5 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE13A_SBH), offsetF.scene_field1_forest3_sce13a_sbh.tableOfVoices, 9 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE14A_SBH), offsetF.scene_field1_forest3_sce14a_sbh.tableOfVoices, 9 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE20_SBH), offsetF.scene_field1_forest3_sce20_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_PSYRUS2_GARAI_SCE00_SBH), offsetF.scene_psyrus2_garai_sce00_sbh.tableOfVoices, 20 },
		{ m_game->file(File::SCENE_PSYRUS2_KAJIYA_SCE00_SBH), offsetF.scene_psyrus2_kajiya_sce00_sbh.tableOfVoices, 11 },
		{ m_game->file(File::SCENE_PSYRUS2_ZOKUCHO_SCE03_SBH), offsetF.scene_psyrus2_zokucho_sce03_sbh.tableOfVoices, 19 },
		{ m_game->file(File::SCENE_PSYRUS2_GARAI_SCE03_SBH), offsetF.scene_psyrus2_garai_sce03_sbh.tableOfVoices, 24 },
		{ m_game->file(File::SCENE_FIELD1_GATE_SCE06_SBH), offsetF.scene_field1_gate_sce06_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE32_SBH), offsetF.scene_field1_forest3_sce32_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE33_SBH), offsetF.scene_field1_forest3_sce33_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST3_SCE34_SBH), offsetF.scene_field1_forest3_sce34_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE09A_SBH), offsetF.scene_field1_forest4_sce09a_sbh.tableOfVoices, 6 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE12_SBH), offsetF.scene_field1_forest4_sce12_sbh.tableOfVoices, 4 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE17_SBH), offsetF.scene_field1_forest4_sce17_sbh.tableOfVoices, 2 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE18_SBH), offsetF.scene_field1_forest4_sce18_sbh.tableOfVoices, 33 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE19_SBH), offsetF.scene_field1_forest4_sce19_sbh.tableOfVoices, 17 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE16_SBH), offsetF.scene_field1_forest4_sce16_sbh.tableOfVoices, 36 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE15_SBH), offsetF.scene_field1_forest4_sce15_sbh.tableOfVoices, 16 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE14_SBH), offsetF.scene_field1_forest4_sce14_sbh.tableOfVoices, 14 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE16A_SBH), offsetF.scene_field1_forest4_sce16a_sbh.tableOfVoices, 39 },
		{ m_game->file(File::SCENE_FIELD1_FOREST4_SCE16B_SBH), offsetF.scene_field1_forest4_sce16b_sbh.tableOfVoices, 36 },
		{ m_game->file(File::SCENE_FIELD2_FOREST1_SCE07_SBH), offsetF.scene_field2_forest1_sce07_sbh.tableOfVoices, 22 },
		{ m_game->file(File::SCENE_FIELD2_FOREST2_SCE07_SBH), offsetF.scene_field2_forest2_sce07_sbh.tableOfVoices, 24 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE11A_SBH), offsetF.scene_field2_forest3_sce11a_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE12A_SBH), offsetF.scene_field2_forest3_sce12a_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE13A_SBH), offsetF.scene_field2_forest3_sce13a_sbh.tableOfVoices, 10 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE20_SBH), offsetF.scene_field2_forest3_sce20_sbh.tableOfVoices, 24 },
		{ m_game->file(File::SCENE_FIELD2_FOREST3_SCE22A_SBH), offsetF.scene_field2_forest3_sce22a_sbh.tableOfVoices, 22 },
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE00_SBH), offsetF.scene_other_clear_sce00_sbh.tableOfVoices, 8 },
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE00_SBH), offsetF.scene_other_clear_sce00_sbh.tableOfVoices + 0x48, 7 }, // Multiplayer
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE03_SBH), offsetF.scene_other_clear_sce03_sbh.tableOfVoices, 3 },
		{ m_game->file(File::SCENE_OTHER_CLEAR_SCE04_SBH), offsetF.scene_other_clear_sce04_sbh.tableOfVoices, 14 }
	}};		

	static constexpr auto entrySize{ sizeof(Voice) + sizeof(VoiceBehavior) };

	struct Slot
	{
		std::size_t fileIndex;
		u32 index;
	};

	std::vector<Slot> slots;
	std::map<u32, std::vector<std::size_t>> groups;

	for (std::size_t i{}; i < filesInfo.size(); ++i)
	{
		const auto& [file, offset, nbVoices] = filesInfo[i];

		for (u32 j{}; j < nbVoices; ++j)
		{
			const auto dialogue{ file->read<u8>(offset + j * entrySize) };
			const auto speaker{ std::find_if(sceneSpeakers.begin(), sceneSpeakers.end(),
				[&](const auto& entry) { return entry.file == i && entry.dialogue == dialogue; }) };

			if (speaker == sceneSpeakers.end())
			{
				throw JcrException{ "No known speaker for dialogue {} of scene {}",
					static_cast<u32>(dialogue), i };
			}

			groups[anyCharacter ? 0u : speaker->speaker].emplace_back(slots.size());
			slots.emplace_back(i, j);
		}
	}

	std::vector<Voice> voices;
	voices.reserve(slots.size());

	for (const auto& [fileIndex, index] : slots)
	{
		const auto& [file, offset, nbVoices] = filesInfo[fileIndex];
		voices.emplace_back(file->read<Voice>(offset + index * entrySize + sizeof(VoiceBehavior)));
	}

	const auto slotsAreAdjacent = [&slots](std::size_t left, std::size_t right) -> bool
	{
		return right == left + 1 && slots[left].fileIndex == slots[right].fileIndex;
	};

	std::vector<std::size_t> source(slots.size());

	for (const auto& [group, members] : groups)
	{
		auto order{ members };

		for (auto i{ order.size() }; i > 1; --i)
		{
			std::swap(order[i - 1], order[m_game->random()->generate(i - 1)]);
		}

		for (u32 pass{}; pass < 4; ++pass)
		{
			for (std::size_t i{}; i < order.size(); ++i)
			{
				if (slots[order[i]].fileIndex != slots[members[i]].fileIndex)
				{
					continue;
				}

				for (std::size_t step{ 1 }; step < order.size(); ++step)
				{
					const auto other{ (i + step) % order.size() };

					if (slots[order[other]].fileIndex != slots[members[i]].fileIndex && slots[order[i]].fileIndex != slots[members[other]].fileIndex)
					{
						std::swap(order[i], order[other]);
						break;
					}
				}
			}
		}

		for (u32 pass{}; pass < 2; ++pass)
		{
			for (std::size_t i{}; i < order.size() && order.size() > 1; ++i)
			{
				if (order[i] == members[i])
				{
					std::swap(order[i], order[(i + 1) % order.size()]);
				}
			}
		}

		for (u32 pass{}; pass < 4; ++pass)
		{
			for (std::size_t i{ 1 }; i < order.size(); ++i)
			{
				if (!slotsAreAdjacent(members[i - 1], members[i]) || !(voices[order[i]] == voices[order[i - 1]]))
				{
					continue;
				}

				for (std::size_t step{ 1 }; step < order.size(); ++step)
				{
					const auto other{ (i + step) % order.size() };

					if (voices[order[other]] == voices[order[i - 1]] || order[other] == members[i] || order[i] == members[other])
					{
						continue;
					}

					if (other && slotsAreAdjacent(members[other - 1], members[other]) && voices[order[i]] == voices[order[other - 1]])
					{
						continue;
					}

					std::swap(order[i], order[other]);
					break;
				}
			}
		}

		for (std::size_t i{}; i < members.size(); ++i)
		{
			source[members[i]] = order[i];
		}
	}

	for (std::size_t i{}; i < slots.size(); ++i)
	{
		const auto& [fileIndex, index] = slots[i];
		const auto& [file, offset, nbVoices] = filesInfo[fileIndex];

		file->write(offset + index * entrySize + sizeof(VoiceBehavior), voices[source[i]]);
	}
}

void Randomizer::miscBetaBattleTheme() const
{
	const auto pubtst2{ m_game->file(File::SOUND_PUBTST2_SND) };
	pubtst2->write(0, Resource::bBattleSnd);
	pubtst2->resize(Resource::bBattleSnd.size());

	const auto& game{ m_game->offset().game };
	const auto& exeOff{ m_game->offset().file.executable };
	const auto& battleOff{ m_game->offset().file.over_battle_bin };

	const auto fadeOffset{ m_game->customCodeOffset(sizeof(MipsFn::BattleThemeFade)) };

	const u16 sptrHi{ static_cast<u16>((game.soundStatePtr + 0x8000u) >> 16) };
	const u16 sptrLo{ static_cast<u16>(game.soundStatePtr) };
	const MipsFn::BattleThemeFade fadeFn
	{
		Mips_t(0x27BDFFF8),                          // addiu sp, sp, -8
		Mips_t(0xAFBF0000),                          // sw    ra, 0(sp)
		Mips::jal(game.fadeSlotFn),                  // jal   fadeSlotFn
		Mips_t(0x00000000),                          // nop
		Mips::lui(Mips::Register::v0, sptrHi),       // lui   v0, HI
		Mips_t(0x8C420000u | sptrLo),                // lw    v0, LO(v0)
		Mips_t(0x00000000),                          // nop
		Mips_t(0x8C420024),                          // lw    v0, 0x24(v0)
		Mips_t(0x00000000),                          // nop
		Mips_t(0x8C430004),                          // lw    v1, 4(v0)
		Mips_t(0x00000000),                          // nop
		Mips_t(0x84650004),                          // lh    a1, 4(v1)
		Mips_t(0x8464000C),                          // lh    a0, 0xc(v1)
		Mips_t(0x04A00003),                          // bltz  a1, +3
		Mips::li(Mips::Register::a2, 0x7f),          // addiu a2, zero, 0x7f
		Mips::jal(game.fadeSeqFn),                   // jal   fadeSeqFn
		Mips::li(Mips::Register::a3, 0x78),          // addiu a3, zero, 0x78
		Mips_t(0x8FBF0000),                          // lw    ra, 0(sp)
		Mips_t(0x27BD0008),                          // addiu sp, sp, 8
		Mips_t(0x03E00008),                          // jr    ra
		Mips_t(0x00000000)                           // nop
	};

	auto executable{ m_game->executable() };
	executable.write(exeOff.ostTable + 65u * 8u + 4u, u8{ 1 });
	executable.write(fadeOffset.file, fadeFn);

	const auto battleBin{ m_game->file(File::OVER_BATTLE_BIN) };
	battleBin->write(battleOff.battleMusicPlay, Mips::li(Mips::Register::a0, 65));
	battleBin->write(battleOff.battleMusicPlay + 4u, Mips::jal(game.playOstFn));

	const auto jalFade{ Mips::jal(fadeOffset.game) };
	battleBin->write(battleOff.battleExitFade, jalFade);
	battleBin->write(battleOff.battleExitFade + 0xC0u, jalFade);
	battleBin->write(battleOff.battleExitFade + 0x1E0u, jalFade);
}

void Randomizer::miscSkipPrologue(bool skipKoris) const
{
	static constexpr MipsFn::AfterTutorialStateData afterTutorialStateData
	{
		0xC8, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0E, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x00,
		0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x00,
		0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x53, 0x00, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x02, 0x00, 0x07, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x02, 0x00, 0x09, 0x00, 0x00, 0x00, 0x5E, 0x00, 0x02, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x60, 0x00,
		0x02, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x62, 0x00, 0x02, 0x00, 0x04, 0x00, 0x01, 0x00, 0x50, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0A, 0x00,
		0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x14, 0x00,
		0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x18, 0x00, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x20, 0x00,
		0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x25, 0x00, 0x01, 0x00, 0x07, 0x00, 0x00, 0x00, 0x32, 0x00,
		0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x36, 0x00, 0x01, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3A, 0x00,
		0x01, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x01, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x42, 0x00,
		0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x04, 0x00, 0x01, 0x00, 0x1C, 0x00,
		0x01, 0x00, 0x06, 0x00, 0x03, 0x00, 0x29, 0x00, 0x01, 0x00, 0x06, 0x00, 0x04, 0x00, 0x2D, 0x00,
		0x04, 0x00, 0x01, 0x00, 0x06, 0x00, 0x71, 0x00
	};
	
	const auto afterTutorialStateOffset{ m_game->customCodeOffset(sizeof(MipsFn::AfterTutorialStateData)) };

	const auto
		li32_afterTutorialState{ Mips::li32(Mips::Register::a0, afterTutorialStateOffset.game) },
		li32_gameStateStruct{ Mips::li32(Mips::Register::a1, m_game->offset().game.gameStateStruct) };

	const MipsFn::WriteAfterTutorialState writeAfterTutorialStateFn
	{
		0x27BDFFF0, // addiu sp, -0x10
		0xAFA2000C, // sw v0, 0xC(sp)
		0xAFA30008, // sw v1, 8(sp)
		0xAFA40004, // sw a0, 4(sp)
		0xAFA50000, // sw a1, 0(sp)

		li32_afterTutorialState[0], // lui a0, 0xXXXX
		li32_gameStateStruct[0], // lui a1, 0xXXXX
		li32_afterTutorialState[1], // ori a0, 0xXXXX
		li32_gameStateStruct[1], // ori a1, 0xXXXX

		// Give 200 Yan, 3 Mugwort and 1 Valerian Podwer
		0x00001821, // move v1, zero
		0x8C8B0000, // lw t3, 0(a0)
		0x24630001, // addiu v1, 1
		0xACAB0000, // sw t3, 0(a1)
		0x24840004, // addiu a0, 4
		0x2C680002, // sltiu t0, v1, 2
		0x1500FFFA, // bnez t0, -6
		0x24A50004, // addiu a1, 4

		// Give Dagger
		0x24A5007C, // addiu a1, 0x7C
		0x3C0900FF, // lui t1, 0x00FF
		0x35290101, // ori t1, 0x0101
		0xACA90000, // sw t1, 0(a1)

		// Set Story Flags and Events
		0x24A50200, // addiu a1, 0x200
		0x00001821, // move v1, zero
		0x8C8B0000, // lw t3, 0(a0)
		0x24630001, // addiu v1, 1
		0xACAB0000, // sw t3, 0(a1)
		0x24840004, // addiu a0, 4
		0x2C680058, // sltiu t0, v1, 58
		0x1500FFFA, // bnez t0, -6
		0x24A50004, // addiu a1, 4

		0x8FA2000C, // lw v0, 0xC(sp)
		0x8FA30008, // lw v1, 8(sp)
		0x8FA40004, // lw a0, 4(sp)
		0x8FA50000, // lw a1, 0(sp)
		0x03E00008, // jr ra
		0x27BD0010  // addiu sp, 0x10
	};

	auto executable{ m_game->executable() };

	const auto writeAfterTutorialStateOffset{ m_game->customCodeOffset(sizeof(MipsFn::WriteAfterTutorialState)) };

	executable.write(afterTutorialStateOffset.file, afterTutorialStateData);
	executable.write(writeAfterTutorialStateOffset.file, writeAfterTutorialStateFn);

	if (skipKoris)
	{
		executable.write(afterTutorialStateOffset.file + 0x16, u8(1));

		// Set the FOREST1/SCE00 entry event script as already played, (event bit 2 + base bit 1)
		executable.write(afterTutorialStateOffset.file + 0x14, u8(6));

		// Reset the previous map triple (4,1,0) != (-1,-1,-1) otherwise
		// it considers the zone unchanged and keeps the minions spawn pool empty
		executable.write(afterTutorialStateOffset.file + 0x46, u16(0xFFFF));
		executable.write(afterTutorialStateOffset.file + 0x4A, u16(0xFFFF));
		executable.write(afterTutorialStateOffset.file + 0x4E, u16(0xFFFF));
		executable.write(afterTutorialStateOffset.file + 0x52, u16(0xFFFF));

		// Entry door index = -1 to match the engine "no door transition" state,
		// otherwise the scene object spawner never runs on the arrival map
		// and its doors + Knowledge 1 pickup don't spawn
		executable.write(afterTutorialStateOffset.file + 0x50, u16(0xFFFF));

		executable.write(afterTutorialStateOffset.file + 0xCE, u8(0x0B));
		executable.write(afterTutorialStateOffset.file + 0x126, u8(0x64));
		executable.write(afterTutorialStateOffset.file + 0x12E, u8(0x6A));
		executable.write(afterTutorialStateOffset.file + 0x12F, u8(0));

		// Spawn at the Beetle Forest, (4,1,0)
		m_game->file(File::OVER_CHAPTER_BIN)->write(m_game->offset().file.over_chapter_bin.chapter2StartMapId, u16(0x6A));
	}
	else
	{
		// Spawn at the Gate, (4,0,1)
		m_game->file(File::OVER_CHAPTER_BIN)->write(m_game->offset().file.over_chapter_bin.chapter2StartMapId, u16(0x65));
	}

	static constexpr u32 chapterFsmStatesTable{ 4 };

	const auto
		state0Fn{ m_game->staticFile(File::OVER_CHAPTER_BIN)->read<u32>(chapterFsmStatesTable) },
		lastStateFn{ m_game->staticFile(File::OVER_CHAPTER_BIN)->read<u32>(chapterFsmStatesTable + 0x14) };

	const u32 currentChapter{ m_game->offset().game.gameStateStruct + 0x2B8 };

	const u16
		currentChapterHi{ static_cast<u16>((currentChapter + 0x8000) >> 16) },
		currentChapterLo{ static_cast<u16>(currentChapter) };

	const MipsFn::SkipChapter2Cinematic skipChapter2CinematicFn
	{
		Mips::lui(Mips::Register::v0, currentChapterHi),
		Mips_t(0x84430000 | currentChapterLo), // lh v1, currentChapter(v0)
		Mips::li(Mips::Register::v0, 1),
		Mips_t(0x14620003), // bne v1, v0, +3
		Mips_t(0x00000000), // nop
		Mips::j(lastStateFn),
		Mips_t(0xAE400004), // sw zero, 4(s2), the NTSC-J entry does not initialize the last state timer
		Mips::j(state0Fn),
		Mips_t(0x00000000)  // nop
	};

	const auto skipChapter2CinematicOffset{ m_game->customCodeOffset(sizeof(MipsFn::SkipChapter2Cinematic)) };

	executable.write(skipChapter2CinematicOffset.file, skipChapter2CinematicFn);
	m_game->file(File::OVER_CHAPTER_BIN)->write(chapterFsmStatesTable, skipChapter2CinematicOffset.game);

	const auto over_title_bin{ m_game->file(File::OVER_TITLE_BIN) };

	const auto
		sb_setLevantGarb{ m_game->isNtscJ() ? Mips_t(0xA04007DD) : Mips_t(0xA04007E5)}; // sb zero, 0x7DD/0x7E5(v0)

	over_title_bin->write(m_game->offset().file.over_title_bin.initMapNewGame, Mips::jal(writeAfterTutorialStateOffset.game));
	over_title_bin->write(m_game->offset().file.over_title_bin.initMapNewGame - 8, Mips_t(0));
	over_title_bin->write(m_game->offset().file.over_title_bin.setLevantGarb, sb_setLevantGarb);
}

void Randomizer::miscItemQuantityLimit(u8 limit) const
{
	const auto
		over_game_bin{ m_game->file(File::OVER_GAME_BIN) },
		over_wpnshop_bin{ m_game->file(File::OVER_WPNSHOP_BIN) };

	const auto
		slti_v0_v0{ Mips_t(0x28420000 + limit + 1) },
		slti_v1_v1{ Mips_t(0x28630000 + limit) },
		slti_v0_s0{ Mips_t(0x2A020000 + limit + 1) },
		li_s0{ Mips_t(0x24100000 + limit) },
		li_v0{ Mips_t(0x24020000 + limit) },
		li_v1{ Mips_t(0x24030000 + limit) };

	over_game_bin->write(m_game->offset().file.over_game_bin.isQuantityLimitReachedFn + 8, slti_v0_v0);
	over_game_bin->write(m_game->offset().file.over_game_bin.itemShopBuyFn + 0x74, slti_v1_v1);

	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x58, slti_v0_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x64, li_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x6C, slti_v0_v0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.itemShopQuantityLimitFn + 0x78, li_v0);

	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0x88, slti_v0_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0x94, li_s0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0x9C, slti_v0_v0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0xA8, li_v0);
	over_wpnshop_bin->write(m_game->offset().file.over_wpnshop_bin.equipmentShopQuantityLimitFn + 0xF4, li_v1);

	const MipsFn::SetChestNewItemQuantityLimit setChestNewItemQuantityLimitFn
	{
		// If quantity is > than the limit
		0x2C620000 + limit + 1u, // sltiu v0, v1, limit + 1
		0x14400002, // bnez v0, +2
		0x00000000, // nop

		// Set quantity to limit
		Mips::li(Mips::Register::v1, limit),

		0x03E00008, // jr ra
		0xA0900000  // sb s0, 0(a0)
	};

	const auto setChestNewItemQuantityLimitOffset{ m_game->customCodeOffset(sizeof(MipsFn::SetChestNewItemQuantityLimit)) };

	m_game->executable().write(setChestNewItemQuantityLimitOffset.file, setChestNewItemQuantityLimitFn);
	over_game_bin->write(m_game->offset().file.over_game_bin.setItemQuantityFromChestFn + 0x34, Mips::jal(setChestNewItemQuantityLimitOffset.game));
}

void Randomizer::miscLevelCapEC(u8 levelCap) const
{
	if (levelCap != 26 && levelCap >= 19 && levelCap < 64)
	{
		const auto
			slti_v0_v1{ Mips_t(0x28620000 + levelCap - 18) },
			li_v1{ Mips_t(0x24030000 + levelCap - 19) };

		const auto over_game_bin{ m_game->file(File::OVER_GAME_BIN) };

		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC, slti_v0_v1);
		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC + 0xC, li_v1);

		const auto shift{ m_game->isVersion(Version::NtscJ1) ? 0x238 : 0x250 };

		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC - shift, slti_v0_v1);
		over_game_bin->write(m_game->offset().file.over_game_bin.levelCapEC - shift + 0xC, li_v1);
	}
}

void Randomizer::miscPalToNtsc() const
{
	if (!m_game->isNtsc())
	{
		auto executable{ m_game->executable() };

		static constexpr auto move_a0_zero{ Mips_t(0x00002021) };

		executable.write(m_game->offset().file.executable.setVideoModeArgument, move_a0_zero);
		executable.write(m_game->offset().file.executable.stretchingY, move_a0_zero);
	}
}