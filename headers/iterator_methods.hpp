#ifndef HEADER_ITERATOR_METHODS_HPP_INCLUDED
#define HEADER_ITERATOR_METHODS_HPP_INCLUDED

#include "advanced_vector.hpp"

template< typename T >
AdvancedVector< T >::iterator::iterator (const iterator& other) {
	parent = other.parent;
	index = other.index;
}

template< typename T >
AdvancedVector< T >::iterator::iterator (AdvancedVector< T >* parent_arg) {
	parent = parent_arg;
	index = 0;
}

template< typename T >
AdvancedVector< T >::iterator::iterator (AdvancedVector< T >* parent_arg, unsigned index_arg) {
	parent = parent_arg;
	index = index_arg;
}


template< typename T >
typename AdvancedVector< T >::iterator&  AdvancedVector< T >::iterator::operator++() {
	index++;
	return *this;
}

template< typename T >
typename AdvancedVector< T >::iterator& AdvancedVector< T >::iterator::operator--() {
	index--;
	return *this;
}

template< typename T >
bool AdvancedVector< T >::iterator::operator == (AdvancedVector< T >::iterator& other) {
	return parent == other.parent && index == other.index;
}

template< typename T >
bool AdvancedVector< T >::iterator::operator != (AdvancedVector< T >::iterator& other) {
	return parent != other.parent || index != other.index;
}

template< typename T >
int AdvancedVector< T >::iterator::operator - (AdvancedVector< T >::iterator& other) {
	if (parent != other.parent)
		throw std::logic_error("iterators must have common parent");

	return index - other.index;
}

template< typename T >
T& AdvancedVector< T >::iterator::operator * () {
	return (*parent)[index];
}

template< typename T >
typename AdvancedVector< T >::iterator& AdvancedVector< T >::iterator::operator += (int distance) {
	index += distance;
	return *this;
}

template< typename T >
typename AdvancedVector< T >::iterator& AdvancedVector< T >::iterator::operator -= (int distance) {
	index -= distance;
	return *this;
}


#endif
