#pragma once

#include <format>
#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>

template <typename... Args>
struct JcrException final : std::runtime_error
{
#if JCR_DEBUG
	JcrException(const char* message, Args&&... args, 
		std::source_location location = std::source_location::current()) 
		: std::runtime_error(exception(message, location, std::forward<Args>(args)...)) {};
private:
	std::string exception(const char* message, const std::source_location& location, Args&&... args) const
	{
		const auto debugInfo{ std::format(" -> {}:{} at {}",
			location.file_name(), location.line(), location.function_name()) };

		if constexpr (sizeof...(Args))
		{
			return std::vformat(message + debugInfo, std::make_format_args(args...));
		}
		else
		{
			return message + debugInfo;
		}
	}
#else
	JcrException(const char* message, Args&&... args)
		: std::runtime_error(exception(message, std::forward<Args>(args)...)) {};
private:
	auto exception(const char* message, Args&&... args) const
	{
		if constexpr (sizeof...(Args))
		{
			return std::vformat(message, std::make_format_args(args...));
		}
		else
		{
			return message;
		}
}
#endif
};

template <typename... Args> 
JcrException(const char*, Args&&...) -> JcrException<Args&&...>;