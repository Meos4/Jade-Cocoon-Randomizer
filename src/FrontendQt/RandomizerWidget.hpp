#pragma once

#include "Backend/Randomizer.hpp"

#include "Common/Json.hpp"

#include <QWidget>

#include <memory>

class RandomizerWidget : public QWidget
{
	Q_OBJECT
public:
	RandomizerWidget(QWidget* parent = nullptr);
	virtual ~RandomizerWidget() = default;

	virtual void enableUI(Randomizer* randomizer) = 0;
	virtual void disableUI() = 0;
	virtual const char* name() const = 0;
	virtual void loadPresets(const Json::Read& json) = 0;
	virtual void savePresets(Json::Write* json) = 0;
};