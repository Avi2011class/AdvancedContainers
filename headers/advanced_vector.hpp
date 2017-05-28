#ifndef HEADER_ADVANCED_VECTOR_HPP_INCLUDED
#define HEADER_ADVANCED_VECTOR_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <ctime>
#include <random>
#include <ctime>
#include <stdexcept>

#include "nodes.hpp"

template< typename T >
class AdvancedVector {

private:
	__node_namespace::_ImplicitNode< T >* 	head;
	std::mt19937_64 		mt;

public:
	// Default constructor O(1)
	AdvancedVector			();
	
	// Advanced constructors like std::vector, O(n * logn)
	AdvancedVector			(unsigned n);
	AdvancedVector			(unsigned n, T&&);

	AdvancedVector			(AdvancedVector&);
	AdvancedVector			(AdvancedVector&&);
	AdvancedVector			(std::vector< T >&);
	
	// Operator = O(n * logn)
	void		operator = 	(AdvancedVector& other);
	void		operator = 	(AdvancedVector&& other);
	
	// Default destructor
	~AdvancedVector			();
	
	// Size of vector, O(1)
	unsigned 	size		() const;
	
	// Exchange content, O(1)!!!
	void 		swap		(AdvancedVector< T >&);

	// Push element to front or back, O(log n)
	void 		push_back	(T&&);
	void		push_back	(const T&);

	void		push_front	(T&&);
	void		push_front	(const T&);

	// Insert element into fixed position, O(log n)
	void		insert		(unsigned index, T&&);
	void		insert		(unsigned index, const T&);
	// Insert vector into fixed position, O(log(m + n)) where m is size of subarray
	void		insert		(unsigned index, const AdvancedVector< T >&);

	// Cut subarray to the other vector, O(log n)
	void	subarray_cut	(unsigned from, unsigned to, AdvancedVector< T >&);
	
	// Cut subarray to the other vector, O(m + (log n)) where m is size of subarray
	void	subarray_copy	(unsigned from, unsigned to, AdvancedVector< T >&);

	// Erase element or range from array, O(log n)
	void		erase		(unsigned index);
	void		erase		(unsigned from, unsigned to);

	// Clear array, O(n * (log n))
	void		clear		();
	
	// Split array and move right part to the other array, O(log n)
	void		split		(AdvancedVector&, unsigned index);

	// Reference to some element, O(log n)
	T& 			operator [] 	(unsigned index);
	
	// Concatenate two arrays, O(log(m + n))
	void 		operator += 	(AdvancedVector&);
	
	// ~push_back
	void		operator += 	(T&&);
	void		operator += 	(const T&);
	
	class iterator : public std::iterator< std::random_access_iterator_tag, T > {
	private:
		int index;
        AdvancedVector< T >* parent;
	public:
        iterator 				(const iterator&);
        iterator 				(AdvancedVector< T >* parent);
        iterator 				(AdvancedVector< T >* parent, unsigned index);

        iterator& operator++	();
        iterator& operator--	();
		
		iterator& operator += 	(int distance);
		iterator& operator -=	(int distance);
		
		iterator& operator +	(int distance);

        int operator -			(iterator& other);
        bool operator == 		(iterator& other);
        bool operator != 		(iterator& other);
		T& operator* 			();
	};

	class reverse_iterator : public std::iterator< std::random_access_iterator_tag, T >  {
	private:
		int index;
        AdvancedVector< T >* parent;
	public:
        reverse_iterator 		(const reverse_iterator&);
        reverse_iterator 		(AdvancedVector< T >* parent);
        reverse_iterator 		(AdvancedVector< T >* parent, unsigned index);

        reverse_iterator& operator++();
		reverse_iterator& operator--();
		
		reverse_iterator& operator += 	(int distance);
		reverse_iterator& operator -=	(int distance);
		
		reverse_iterator& operator + 	(int distance);
		
		int operator -			(reverse_iterator& other);
        bool operator == 		(reverse_iterator& other);
        bool operator != 		(reverse_iterator& other);
		T& operator* 			();
	};
	
	typename AdvancedVector< T >::iterator			begin	();
	typename AdvancedVector< T >::iterator			end		();
	typename AdvancedVector< T >::reverse_iterator	rbegin	();
	typename AdvancedVector< T >::reverse_iterator	rend	();
	
};

#include "reverse_iterator_methods.hpp"
#include "iterator_methods.hpp"
#include "advanced_vector_methods.hpp"

#endif
