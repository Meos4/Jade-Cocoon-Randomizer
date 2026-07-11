#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

class RawTypeNavigator final
{
public:
	RawTypeNavigator(void* ptr, std::size_t size);

	template <typename T>
	T read() const
	{
		ptrThrowIfInvalid((char*)m_ptr + sizeof(T) - 1);
		T val;
		std::memcpy(&val, m_ptr, sizeof(T));
		return val;
	}

	template <typename T>
	T read(std::intptr_t shift) const
	{
		const auto ptr{ (char*)m_ptr + shift };
		ptrThrowIfInvalid(ptr);
		ptrThrowIfInvalid(ptr + sizeof(T) - 1);
		T val;
		std::memcpy(&val, ptr, sizeof(T));
		return val;
	}

	template <typename T>
	void write(const T& val) const
	{
		ptrThrowIfInvalid((char*)m_ptr + sizeof(T) - 1);
		std::memcpy(m_ptr, &val, sizeof(T));
	}

	template <typename T>
	void write(const T& val, std::intptr_t shift) const
	{
		const auto ptr{ (char*)m_ptr + shift };
		ptrThrowIfInvalid(ptr);
		ptrThrowIfInvalid(ptr + sizeof(T) - 1);
		std::memcpy(ptr, &val, sizeof(T));
	}

	void resetPtr();

	inline auto& operator=(std::intptr_t offset)
	{
		m_ptr = (char*)m_begin + offset;
		ptrThrowIfInvalid(m_ptr);
		return *this;
	}

	inline auto& operator+=(std::intptr_t shift)
	{
		m_ptr = (char*)m_ptr + shift;
		ptrThrowIfInvalid(m_ptr);
		return *this;
	}

	inline auto& operator-=(std::intptr_t shift)
	{
		m_ptr = (char*)m_ptr - shift;
		ptrThrowIfInvalid(m_ptr);
		return *this;
	}
private:
	void ptrThrowIfInvalid(void* ptr) const;

	void* m_begin;
	void* m_ptr;
	std::size_t m_size;
};