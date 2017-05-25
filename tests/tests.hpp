#ifndef HEADER_TESTS_HPP_INCLUDED
#define HEADER_TESTS_HPP_INCLUDED

#include <gtest/gtest.h>
#include "../headers/advanced_vector.hpp"

TEST(ADVANCED_VECTOR, CONSTRUCTOR_2) {
	AdvancedVector< int > A(5);
	for (int i = 0; i < 5; i++) {
		ASSERT_EQ(A[i], 0);
	}
}

TEST(ADVANCED_VECTOR, CONSTRUCTOR_3) {
	AdvancedVector< int > A(5, 8);
	for (int i = 0; i < 5; i++) {
		ASSERT_EQ(A[i], 8);
	}
}

TEST(ADVANCED_VECTOR, CONSTRUCTOR_COPY) {
	AdvancedVector< int > A(5, 6);
	for (int i = 0; i < 5; i++)
		A[i] = i;
	AdvancedVector< int > B(A);
	for (int i = 0; i < 5; i++)
		ASSERT_EQ(i, B[i]);
}

TEST(ADVANCED_VECTOR, SIZE) {
	AdvancedVector< int > A(132);
	ASSERT_EQ(A.size(), 132);
}

#endif

