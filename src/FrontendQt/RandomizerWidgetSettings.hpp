#pragma once

#include "Common/Json.hpp"
#include "Common/Types.hpp"
#include "FrontendQt/QtUtility.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QSlider>

#include <functional>

#define SETTINGS(Variable) Variable, UI_NAME(Variable)

template <typename T>
class RandomizerWidgetSettings
{
public:
	RandomizerWidgetSettings(const char* name)
		: m_name(name)
	{
	}

	virtual ~RandomizerWidgetSettings() = default;

	void load(const nlohmann::json& json)
	{
		Json::set<T>(json, m_name, [this](auto&& v) { set(v); });
	}

	void save(nlohmann::ordered_json* json) const
	{
		(*json)[m_name] = get();
	}

	virtual T get() const = 0;
	virtual void set(T val) = 0;
private:
	const char* m_name;
};

class RandomizerQCheckBox final : public RandomizerWidgetSettings<bool>
{
public:
	RandomizerQCheckBox(QCheckBox* widget, const char* name)
		: m_widget(widget), RandomizerWidgetSettings<bool>(name)
	{
		m_widget->setStyleSheet("font-weight: normal;");
	};

	inline bool get() const override
	{
		return m_widget->isChecked();
	}

	inline void set(bool val) override
	{
		m_widget->setChecked(val);
	}
private:
	QCheckBox* m_widget;
};

class RandomizerQSlider final : public RandomizerWidgetSettings<s32>
{
public:
	RandomizerQSlider(QSlider* widget, const char* name)
		: m_widget(widget), RandomizerWidgetSettings<s32>(name)
	{
		m_widget->setStyleSheet("font-weight: normal;");
	};

	inline s32 get() const override
	{
		return m_widget->value();
	}

	inline void set(s32 val) override
	{
		m_widget->setValue(val);
	}
private:
	QSlider* m_widget;
};

class RandomizerQComboBox final : public RandomizerWidgetSettings<s32>
{
public:
	RandomizerQComboBox(QComboBox* widget, const char* name)
		: m_widget(widget), RandomizerWidgetSettings<s32>(name)
	{
		m_widget->setStyleSheet("font-weight: normal;");
	};

	inline s32 get() const override
	{
		return m_widget->currentIndex();
	}

	inline void set(s32 val) override
	{
		const auto count{ m_widget->count() };
		if (val < 0)
		{
			val = 0;
		}
		else if (val >= count)
		{
			val = count - 1;
		}
		m_widget->setCurrentIndex(val);
	}
private:
	QComboBox* m_widget;
};