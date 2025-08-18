#include "fixed_vector.hpp"
#include <cstdlib>
#include <cassert>

namespace ds {

FixedVector::FixedVector(size_t capacity)
: m_capacity{capacity}
, m_elements{(int*)::calloc(m_capacity, sizeof(*m_elements))}
{
	assert(m_elements);
}

FixedVector::~FixedVector()
{
	::free(m_elements);
}

FixedVector::FixedVector(FixedVector const& other)
: m_capacity{other.m_capacity}
, m_elements{(int*)::calloc(m_capacity, sizeof(*m_elements))}
{
	for (size_t i = 0; i < m_capacity; ++i) {
		m_elements[i] = other.m_elements[i];
	}
}


FixedVector& FixedVector::operator=(FixedVector const& other)
{
	if (this != &other) {
		::free(m_elements);
		m_capacity = other.m_capacity;
		m_elements = (int*)::calloc(m_capacity, sizeof(*m_elements));
		for (size_t i = 0; i < m_capacity; ++i) {
			m_elements[i] = other.m_elements[i];
		}
	}

	return *this;
}



size_t FixedVector::size() const
{
	return m_capacity;
}

int& FixedVector::at(size_t index)
{
	assert(index < m_capacity);
	return m_elements[index];
}

int const& FixedVector::at(size_t index) const
{
	assert(index < m_capacity);
	return m_elements[index];
}


} // namespace ds
