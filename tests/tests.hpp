#ifndef HEADER_TESTS_HPP_INCLUDED
#define HEADER_TESTS_HPP_INCLUDED

#include <gtest/gtest.h>
#include "../headers/advanced_vector.hpp"
#include "../headers/fenwick_tree.hpp"

TEST(ADVANCED_VECTOR, CONSTRUCTOR_2) {
	AdvancedVector< int > A(5);
	for (int i = 0; i < 5; i++) {
		ASSERT_EQ(A[i], 0);
	}
}

TEST(ADVANCED_VECTOR, CONSTRUCTOR_3) {
	AdvancedVector< int > A(5, 8);
	for (int i = 0; i < 5; i++ ) {
		ASSERT_EQ(A[i], 8);
	}
}

TEST(ADVANCED_VECTOR, CONSTRUCTOR_COPY) {
	AdvancedVector< int > A(5, 6);
	for (int i = 0; i < 5; i++)
		A[i] = i;
	AdvancedVector< int > B(A);
	for (int i = 0; i < 5; i++)
		ASSERT_EQ( i, B[i] );
}

TEST(ADVANCED_VECTOR, SIZE) {
	AdvancedVector< int > A(132);
	ASSERT_EQ( A.size(), 132 );
}

TEST(ADVANCED_VECTOR, SWAP) {
	AdvancedVector< int > A;
	AdvancedVector< int > B;
	for ( int i = 0; i < 6; i++ ) {
		A.push_back( i );
		B.push_back( i * 2 );
	}
	A.swap( B );
	for (int i = 0; i < 6; i++) {
		ASSERT_EQ( A[i], i * 2 );
		ASSERT_EQ( B[i], i );
	}
}

TEST(ADVANCED_VECTOR, PUSH_BACK_AND_FRONT) {
	AdvancedVector< int > A;
	for (int i = 0; i < 10; i++)
		A.push_back( i );
	for (int i = -1; i > -10; i--)
		A.push_front( i );
	for (int i = 0; i < (int)A.size(); i++)
		ASSERT_EQ( A[i], i - 9 );
}

TEST(ADVANCED_VECTOR, INSERT_ONE) {
	AdvancedVector< int > A;
	for (int i = 1; i < 10; i++) 
		A.insert( 0, i );
	for (int i = 0; i < 10; i++) 
		A.insert( A.size(), -i-1 );
	A.insert( 9, 0 );
	for (int i = 0; i < (int)A.size(); i++)
		ASSERT_EQ( A[i], 9 - i );
}
/*
TEST(ADVANCED_VECTOR, INSERT_GROUP) {
	AdvancedVector< int > A(6, 6);
	AdvancedVector< int > B(3, 3);
}*/
// TODO add tests
#endif

