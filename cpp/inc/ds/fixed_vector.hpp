#ifndef DS_FIXED_VECTOR_H
#define DS_FIXED_VECTOR_H

#include <cstddef>

namespace ds {

class FixedVector {
public:
	FixedVector(size_t capacity = 16);
	~FixedVector();

	FixedVector(FixedVector const& other);
	FixedVector& operator=(FixedVector const& other);

	size_t size() const;
	int& at(size_t index);
	int const& at(size_t index) const;
private:
	size_t m_capacity;
	int* m_elements;
};

} // namespace ds


#endif // DS_FIXED_VECTOR_H
