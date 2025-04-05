#include "RawTypeNavigator.hpp"

#include "Common/JcrException.hpp"

RawTypeNavigator::RawTypeNavigator(void* ptr, std::size_t size)
	: m_begin(ptr), m_ptr(ptr), m_size(size)
{
}

void RawTypeNavigator::resetPtr()
{
	m_ptr = m_begin;
}

void RawTypeNavigator::ptrThrowIfInvalid(void* ptr) const
{
	if (static_cast<std::uintptr_t>((char*)ptr - (char*)m_begin) >= m_size)
	{
		throw JcrException{ "Type overflow" };
	}
}