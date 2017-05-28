CXX_COMPILER = g++-6
OPT = -Wall -Wextra -Wpedantic -O3 -std=c++14

default: main.cpp tests/tests.hpp
	${CXX_COMPILER} ${OPT} -o containers  main.cpp -lgtest -pthread
