#pragma once

#include <cstddef>
#include <cstdint>

class RawTypeBrowser final
{
public:
	RawTypeBrowser(void* ptr, std::size_t size);

	template <typename T>
	T read() const
	{
		ptrThrowIfInvalid((char*)m_ptr + sizeof(T) - 1);
		return *(T*)m_ptr;
	}

	template <typename T>
	T read(std::intptr_t shift) const
	{
		ptrThrowIfInvalid((char*)m_ptr + shift + sizeof(T) - 1);
		return *(T*)((char*)m_ptr + shift);
	}

	template <typename T>
	void write(const T& val) const
	{
		ptrThrowIfInvalid((char*)m_ptr + sizeof(T) - 1);
		*(T*)m_ptr = val;
	}

	template <typename T>
	void write(const T& val, std::intptr_t shift) const
	{
		ptrThrowIfInvalid((char*)m_ptr + shift + sizeof(T) - 1);
		*(T*)((char*)m_ptr + shift) = val;
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
