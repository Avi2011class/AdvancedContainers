#ifndef HEADER_ADVANCED_VECTOR_METHODS_HPP_INCLUDED
#define HEADER_ADVANCED_VECTOR_METHODS_HPP_INCLUDED

#include "advanced_vector.hpp"


template< typename T >
AdvancedVector< T >::AdvancedVector () : head(NULL) {
	std::random_device rd;
	mt = std::mt19937_64(rd());
}

template< typename T >
AdvancedVector< T >::AdvancedVector (unsigned n) {
	std::random_device rd;
	mt = std::mt19937_64(rd());
	head = NULL;
	for (int i = 0; i < ( int )n; i++)
		head = __node_namespace::_ImplicitNode< T >::Merge(head, new __node_namespace::_ImplicitNode< T >(T(), mt()));
}

template< typename T >
AdvancedVector< T >::AdvancedVector (unsigned n, T&& value) {
	std::random_device rd;
	mt = std::mt19937_64(rd());
	head = NULL;
	for (unsigned i = 0; i < n; i++)
		head = __node_namespace::_ImplicitNode< T >::Merge(head, new __node_namespace::_ImplicitNode< T >(value, mt()));
}

template< typename T >
AdvancedVector< T >::AdvancedVector (AdvancedVector< T >& other) {
    std::random_device rd;
	mt = std::mt19937_64(rd());
	head = __node_namespace::_ImplicitNode< T >::Copy(other.head);
}

template< typename T >
AdvancedVector< T >::AdvancedVector (AdvancedVector< T >&& other) {
    std::random_device rd;
	mt = std::mt19937_64(rd());
	head = __node_namespace::_ImplicitNode< T >::Copy(other.head);
}

template< typename T >
AdvancedVector< T >::AdvancedVector (std::vector< T >& src) {
    head = NULL;
    for (auto & iter: src)
		push_back(iter);
}

template< typename T >
AdvancedVector< T >::~AdvancedVector () {
	__node_namespace::_ImplicitNode< T >::Destroy(head);
}


template< typename T >
unsigned AdvancedVector< T >::size () const {
	return __node_namespace::_ImplicitNode< T >::getSize(head);
}

template< typename T >
void AdvancedVector< T >::swap (AdvancedVector< T >& other) {
    std::swap (head, other.head);
}


template< typename T >
T& AdvancedVector< T >::operator[] (unsigned index) {
    if (index >= size())
		throw std::range_error("index must not be greater than array size");

	index++;

    __node_namespace::_ImplicitNode< T >* iter = head;
    while (true) {
		if ( __node_namespace::_ImplicitNode< T >::getSize(iter->left) + 1 == index )
			return iter->cost;

		if ( __node_namespace::_ImplicitNode< T >::getSize(iter->left) + 1 > index ) {
			iter = iter->left;
		}
		else {
			index -= __node_namespace::_ImplicitNode< T >::getSize(iter->left) + 1;
			iter = iter->right;
		}
    }
}

template< typename T >
void AdvancedVector< T >::operator += (AdvancedVector< T >& other) {
    head = __node_namespace::_ImplicitNode< T >::Merge(head, __node_namespace::_ImplicitNode< T >::Copy(other.head));
}

template< typename T >
void AdvancedVector< T >::operator += (T&& value) {
    insert(size(), value);
}

template< typename T >
void AdvancedVector< T >::operator += (const T& value) {
    insert(size(), value);
}


template< typename T >
void AdvancedVector< T >::push_back (T&& value) {
	head = __node_namespace::_ImplicitNode< T >::Merge(head, new __node_namespace::_ImplicitNode< T >(value, mt()));
}

template< typename T >
void AdvancedVector< T >::push_back (const T& value) {
	head = __node_namespace::_ImplicitNode< T >::Merge(head, new __node_namespace::_ImplicitNode< T >(value, mt()));
}

template< typename T >
void AdvancedVector< T >::push_front (T&& value) {
	head = __node_namespace::_ImplicitNode< T >::Merge(new __node_namespace::_ImplicitNode< T >(value, mt()), head);
}

template< typename T >
void AdvancedVector< T >::push_front (const T& value) {
	head = __node_namespace::_ImplicitNode< T >::Merge(new __node_namespace::_ImplicitNode< T >(value, mt()), head);
}

template< typename T >
void AdvancedVector< T >::insert (unsigned index, T&& value) {
    if (index > size())
		throw std::range_error("index must not be greater then size");
	
    __node_namespace::_ImplicitNode< T >::Insert(head, index, value, mt());
}

template< typename T >
void AdvancedVector< T >::insert (unsigned index, const T& value) {
    if (index > size())
		throw std::range_error("index must not be greater then size");
	
    head = __node_namespace::_ImplicitNode< T >::Insert(head, index, value, mt());
}

template< typename T >
void AdvancedVector< T >::insert (unsigned index, const AdvancedVector< T >& src) {
	if (index > size())
		throw std::range_error("index must not be greater then size");

    __node_namespace::_ImplicitNode< T >* left, *right, *mid;
	__node_namespace::_ImplicitNode< T >::Split (head, index, &left, &right);
	mid = __node_namespace::_ImplicitNode< T >::Copy(src.head);

    head = __node_namespace::_ImplicitNode< T >::Merge(left, __node_namespace::_ImplicitNode< T >::Merge(mid, right));
}

template< typename T >
void AdvancedVector< T >::subarray_cut (unsigned from, unsigned to, AdvancedVector< T >& dest) {
    if (from >= size())
		throw std::range_error("from must be less then size");
	if (to > size())
		throw std::range_error("to must not be greater then size");
    if (from > to)
		throw std::range_error("from must not be greater then to");

    __node_namespace::_ImplicitNode< T > *left, *mid, *right;

    from += 1;
    to += 1;

    __node_namespace::_ImplicitNode< T >::Split(head, from - 1, &left, &mid);
	__node_namespace::_ImplicitNode< T >::Split(mid, to - from, &mid, &right);

	head = __node_namespace::_ImplicitNode< T >::Merge(left, right);

	__node_namespace::_ImplicitNode< T >::Destroy(dest.head);
	dest.head = mid;
}

template< typename T >
void AdvancedVector< T >::subarray_copy (unsigned from, unsigned to, AdvancedVector< T >& dest) {
    if (from >= size())
		throw std::range_error("from must be less then size");
	if (to > size())
		throw std::range_error("to must not be greater then size");
    if (from > to)
		throw std::range_error("from must not be greater then to");

    __node_namespace::_ImplicitNode< T > *left, *mid, *right;

    from += 1;
    to += 1;

    __node_namespace::_ImplicitNode< T >::Split(head, from - 1, &left, &mid);
	__node_namespace::_ImplicitNode< T >::Split(mid, to - from, &mid, &right);

	__node_namespace::_ImplicitNode< T >::Destroy(dest.head);
	dest.head = __node_namespace::_ImplicitNode< T >::Copy(mid);

	head = __node_namespace::_ImplicitNode< T >::Merge(left, __node_namespace::_ImplicitNode< T >::Merge(mid, right));
}

template< typename T >
void AdvancedVector< T >::erase	(unsigned index) {
	if (index > size())
		throw std::range_error("index must be less then size");

	index += 1;
	__node_namespace::_ImplicitNode< T >::Remove(head, index);
}

template< typename T >
void AdvancedVector< T >::erase (unsigned from, unsigned to) {
    if (from >= size())
		throw std::range_error("from must be less then size");
	if (to > size())
		throw std::range_error("to must not be greater then size");
    if (from > to)
		throw std::range_error("from must not be greater then to");

    __node_namespace::_ImplicitNode< T > *left, *mid, *right;

    from += 1;
    to += 1;

    __node_namespace::_ImplicitNode< T >::Split(head, from - 1, &left, &mid);
	__node_namespace::_ImplicitNode< T >::Split(mid, to - from, &mid, &right);
	__node_namespace::_ImplicitNode< T >::Destroy(mid);

	head = __node_namespace::_ImplicitNode< T >::Merge(left, right);
}

template< typename T >
void AdvancedVector< T >::clear () {
	__node_namespace::_ImplicitNode< T >::Destroy(head);
	head = NULL;
}

template< typename T >
void AdvancedVector< T >::operator = (AdvancedVector& other) {
	clear();
	head = __node_namespace::_ImplicitNode< T >::Copy(other.head);
}

template< typename T >
void AdvancedVector< T >::operator = (AdvancedVector&& other) {
	clear();
	head = __node_namespace::_ImplicitNode< T >::Copy(other.head);
}

////////////////////////////////////////////////////////////////////

template< typename T >
typename AdvancedVector< T >::iterator AdvancedVector< T >::begin() {
	return AdvancedVector< T >::iterator(this, 0);
}

template< typename T >
typename AdvancedVector< T >::iterator AdvancedVector< T >::end() {
	return AdvancedVector< T >::iterator(this, size());
}

template< typename T >
typename AdvancedVector< T >::reverse_iterator AdvancedVector< T >::rbegin() {
	return AdvancedVector< T >::reverse_iterator(this, 0);
}

template< typename T >
typename AdvancedVector< T >::reverse_iterator AdvancedVector< T >::rend() {
	return AdvancedVector< T >::reverse_iterator(this, size());
}


#endif
