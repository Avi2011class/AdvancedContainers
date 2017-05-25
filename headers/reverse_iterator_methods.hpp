#ifndef HEADER_REVERCE_ITERATOR_METHODS_HPP_INCLUDED
#define HEADER_REVERCE_ITERATOR_METHODS_HPP_INCLUDED

#include "advanced_vector.hpp"

template< typename T >
AdvancedVector< T >::reverse_iterator::reverse_iterator (const reverse_iterator& other) {
	parent = other.parent;
	index = other.index;
}

template< typename T >
AdvancedVector< T >::reverse_iterator::reverse_iterator (AdvancedVector< T >* parent_arg) {
	parent = parent_arg;
	index = (*parent_arg).size() - 1;
}

template< typename T >
AdvancedVector< T >::reverse_iterator::reverse_iterator (AdvancedVector< T >* parent_arg, unsigned index_arg) {
	parent = parent_arg;
	index = index_arg;
}


template< typename T >
typename AdvancedVector< T >::reverse_iterator&  AdvancedVector< T >::reverse_iterator::operator++() {
	index--;
}

template< typename T >
typename AdvancedVector< T >::reverse_iterator& AdvancedVector< T >::reverse_iterator::operator--() {
	index++;
}

template< typename T >
bool AdvancedVector< T >::reverse_iterator::operator == (AdvancedVector< T >::reverse_iterator& other) {
	return parent == other.parent && index == other.index;
}

template< typename T >
bool AdvancedVector< T >::reverse_iterator::operator != (AdvancedVector< T >::reverse_iterator& other) {
	return parent != other.parent || index != other.index;
}

template< typename T >
int AdvancedVector< T >::reverse_iterator::operator - (AdvancedVector< T >::reverse_iterator& other) {
	if (parent != other.parent)
		throw std::logic_error("iterators must have common parent");

	return index - other.index;
}

template< typename T >
T& AdvancedVector< T >::reverse_iterator::operator * () {
	return (*parent)[index];
}

template< typename T >
typename AdvancedVector< T >::reverse_iterator& AdvancedVector< T >::reverse_iterator::operator += (int distance) {
	index -= distance;
	return *this;
}

template< typename T >
typename AdvancedVector< T >::reverse_iterator& AdvancedVector< T >::reverse_iterator::operator -= (int distance) {
	index += distance;
	return *this;
}

#endif
