#ifndef HEADER_FENWICK_TREE_INCLUDED
#define HEADER_FENWICK_TREE_INCLUDED

#include <iostream>
#include <vector>
#include <iterator>

/*
Дерево Фенвика - это структура данных на массиве, позволяющая выполнять следующие операции:
1) Вычислять сумму элементов на некотором отрезке за O(log N)
2) Позволяет изменять значение произвольного элемента за O(log N)
3) Возвращает значение произвольного элемента за O(log N)
*/

template<typename T>
class FenwickTree : private std::vector< T >
{
private:
	T              _prefix_sum  ( int right ) const;
	
public:
    // zero initialization
    FenwickTree				( size_t size );
    // initialization from vector
    FenwickTree             ( const std::vector< T >& );
    // increment some element
    void            inc     ( int index, const T& delta );
    void            inc     ( int index, T&& delta );
    // sum of subarray
    T               sum     ( int left, int right ) const;
    // return element of array
    T               operator [] ( size_t index ) const;
    // set the value of element
    void            set     ( size_t index, const T& value );
    void            set     ( size_t index, T&& value );
};

/* Префиксное дерево: структура данных, реализованная поверх дерева Фенвика, позволяющая выполнять
 * 
 * 

template< typename T >
class PrefixTree
{
private:
    FenwickTree< T > prefix_sums;
    int size;
public:
    PrefixTree(int size) : size(size), prefix_sums(FenwickTree< T >(size)) {}
    void add(int begin_add, int end_add, T value)
    {
        prefix_sums.inc(begin_add, value);
        if (end_add < size - 1)
            prefix_sums.inc(end_add + 1, -value);
    }
    T operator [] (int index)
    {
        return prefix_sums.sum(index);
    }
};

*/

#include "fenwick_tree_methods.hpp"
#endif
