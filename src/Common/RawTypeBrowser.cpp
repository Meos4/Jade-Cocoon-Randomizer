#include "RawTypeBrowser.hpp"

#include <stdexcept>

RawTypeBrowser::RawTypeBrowser(void* ptr, std::size_t size)
	: m_begin(ptr), m_ptr(ptr), m_size(size)
{
}

void RawTypeBrowser::resetPtr()
{
	m_ptr = m_begin;
}

void RawTypeBrowser::ptrThrowIfInvalid(void* ptr) const
{
	if (static_cast<std::uintptr_t>((char*)ptr - (char*)m_begin) >= m_size)
	{
		throw std::runtime_error{ "Type overflow" };
	}
}