#ifndef HEADER_FENWICK_TREE_METHODS_INCLUDED
#define HEADER_FENWICK_TREE_METHODS_INCLUDED
#include <iostream>
#include <exception>
#include <stdexcept>

#include "fenwick_tree.hpp"

template< typename T >
FenwickTree< T >::FenwickTree ( size_t size ) {
    this->resize( size );
}

template< typename T >
FenwickTree< T >::FenwickTree ( const std::vector< T >& array ) {
    this->resize( array.size() );
    for ( size_t i = 0; i < this->size(); i++ ) 
        inc( i, array[i] );
}

template< typename T >
void FenwickTree< T >::inc ( int index, const T& delta ) {
    if ( index < 0 || index >= ( int )this->size() ) 
        throw std::range_error( "inc:: Index must be greater then zero and less then size of tree" );
        
    for ( ; index < ( int )this->size(); index = (index | (index + 1)) ) 
        this->at(index) += delta;
}

template< typename T >
void FenwickTree< T >::inc ( int index, T&& delta ) {
    if ( index < 0 || index >= ( int )this->size() ) 
        throw std::range_error( "inc:: Index must be greater then zero and less then size of tree" );
    
    for ( ; index < ( int )this->size(); index = (index | (index + 1)) ) 
        this->at(index) += delta;
}

template< typename T >
T FenwickTree< T >::_prefix_sum(int right) const {    
    T result = 0;
    for(; right >= 0; right = (right & (right + 1)) - 1)
        result += this->at( right );
    return result;
}

template< typename T >
T FenwickTree< T >::sum( int left, int right ) const {
    if ( left > right )
        std::swap( left, right );   
    if ( left < 0 || right >= ( int )this->size() )
        throw std::range_error( "sum:: Index must be greater then zero and less then size of tree" );
    
    return _prefix_sum( right ) - _prefix_sum( left - 1 );
}

template< typename T >
T FenwickTree< T >::operator [] ( size_t index ) const {
    if ( index > this->size() )
        throw std::range_error( "opeartor[]:: Index must be greater then zero and less then size of tree" );
    return sum( index, index );
}

template< typename T >
void FenwickTree< T >::set( size_t index, const T& value) {
    if ( index >= this->size() )
        throw std::range_error( "set:: Index must be greater then zero and less then size of tree" );
    
    T delta = value - sum( index, index );
    inc ( index, delta );
}

template< typename T >
void FenwickTree< T >::set( size_t index, T&& value) {
    if ( index >= this->size() )
        throw std::range_error( "set:: Index must be greater then zero and less then size of tree" );
    
    T delta = value - sum( index, index );
    inc ( index, delta );
}

#endif
