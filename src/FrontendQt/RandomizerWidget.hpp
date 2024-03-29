#pragma once

#include "Backend/Game.hpp"
#include "Backend/SharedData.hpp"

#include "nlohmann/json.hpp"

#include <QWidget>

#include <memory>

class RandomizerWidget : public QWidget
{
	Q_OBJECT
public:
	RandomizerWidget(QWidget* parent = nullptr);
	virtual ~RandomizerWidget() = default;

	virtual void enableUI(std::shared_ptr<Game> game, std::shared_ptr<SharedData> sharedData) = 0;
	virtual void disableUI() = 0;
	virtual void write() const = 0;
	virtual const char* name() const = 0;
	virtual void loadPresets(const nlohmann::json& json) = 0;
	virtual void savePresets(nlohmann::ordered_json* json) = 0;
};