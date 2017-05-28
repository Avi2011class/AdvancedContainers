#ifndef HEADER_FENWICK_TREE_METHODS_INCLUDED
#define HEADER_FENWICK_TREE_METHODS_INCLUDED
#include "fenwick_tree.hpp"

template< typename T >
FenwickTree< T >::FenwickTree( size_t size ) : size(size) {
	data.assign(size, (T)(0));
}

template< typename T >
FenwickTree< T >::FenwickTree(const std::vector< T >& array) : size(array.size()){
	data.assign(size, (T)(0));
	for(int i = 0; i < size; i++)
		inc(i, array[i]);
}

template< typename T >
T FenwickTree< T >::sum(int right) const {
	T result = (T)0;
	for(; right >= 0; right = (right & (right + 1)) - 1)
		result += data[right];
	return result;
}

template< typename T >
void FenwickTree< T >::inc(int index, T delta) {
	for(; index < size; index = (index | (index + 1)))
		data[index] += delta;
}

template< typename T >
T FenwickTree< T >::sum (int left, int right) {
	return sum(right) - sum(left - 1);
}

#endif
