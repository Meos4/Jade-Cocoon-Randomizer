#include "CustomCode.hpp"

#include "Backend/MipsFunctionTypes.hpp"

CustomCode::CustomCode(std::shared_ptr<Game> game)
	: m_game(game)
{
}

void CustomCode::write() const
{
	static constexpr u32 customCodeSize
	{
		sizeof(MipsFn::GenerateValidMinion) + // Story
		sizeof(MipsFn::GenerateValidMinion) + // EC

		sizeof(MipsFn::NuzlockeDefinitiveDeath) +
		sizeof(MipsFn::WriteUsedMapEOBData) +
		sizeof(MipsFn::WriteUsedMapEOB) +
		sizeof(MipsFn::WriteUsedMapCapture) +
		sizeof(MipsFn::ReadUsedMap) +
		sizeof(MipsFn::ResetFromNowhere) +

		sizeof(MipsFn::DifficultyModeStats) +

		sizeof(MipsFn::AfterTutorialStateData) +
		sizeof(MipsFn::WriteAfterTutorialState) +

		sizeof(MipsFn::DrawHiddenStats) +
		sizeof(MipsFn::CriticalRateFromStats) +
		sizeof(MipsFn::DrawCriticalRate) +
		sizeof(MipsFn::DrawCriticalRateMerge) +

		sizeof(MipsFn::ToggleX2Framerate) +

		sizeof(MipsFn::FixEntityEnhancement) +
		
		sizeof(MipsFn::PreviewHeal)
	};

	m_game->expandExecutable(m_game->isVersion(Version::NtscJ1) ? customCodeSize + NtscJ1::customCodeShift : customCodeSize);
}

bool CustomCode::isVanilla() const
{
	return m_game->executable().size() == (m_game->isVersion(Version::NtscJ1) ?
		m_game->offset().file.executable.cc_begin - NtscJ1::customCodeShift :
		m_game->offset().file.executable.cc_begin);
}