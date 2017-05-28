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
class FenwickTree
{
private:
    std::vector< T > data;
    size_t size;
	T sum(int right) const;
	
public:
    FenwickTree								( size_t size );
    FenwickTree(const std::vector< T >&);
    void inc(int index, T delta);
    T sum(int left, int right);
};

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


#endif
