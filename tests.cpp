#include <iostream>
#include <random>
#include <vector>
#include <deque>
#include <iterator>
#include <algorithm>

#include <decartian.hpp>

#include <gtest/gtest.h>

namespace Test {

    TEST(AdvancedVector, Constructor) {
        AdvancedVector<int> a({1, 2, 3, 4, 5});
        auto b = a;
        a[3] = 100;
        EXPECT_EQ(a, AdvancedVector<int>({1, 2, 3, 100, 5}));
        EXPECT_EQ(b, AdvancedVector<int>({1, 2, 3, 4, 5}));

        std::vector<int> v = {1, 2, 3, 4};
        AdvancedVector<int> vv(v.begin(), v.end());

        EXPECT_EQ(vv, AdvancedVector<int>({1, 2, 3, 4}));
    }

    static int n_copies = 0;
    struct StructWithCopyCounter {
        int value_;

        StructWithCopyCounter(int value) : value_(value) { ++n_copies; };
        StructWithCopyCounter(const StructWithCopyCounter& rhs) {
            ++n_copies;
            value_ = rhs.value_;
        };
        StructWithCopyCounter(StructWithCopyCounter&&) = default;

        StructWithCopyCounter& operator=(const StructWithCopyCounter& rhs) {
            ++n_copies;
            value_ = rhs.value_;
            return *this;
        }
        StructWithCopyCounter& operator=(StructWithCopyCounter&&) = default;

        bool operator==(const StructWithCopyCounter& rhs) {
            return value_ == rhs.value_;
        }

        bool operator!=(const StructWithCopyCounter& rhs) {
            return !operator==(rhs);
        }
    };

    std::ostream& operator<<(std::ostream& ostr, const StructWithCopyCounter& data) {
        return (ostr << data.value_);
    }

    TEST(AdvancedVector, CopyMoveConstructors) {
        AdvancedVector<int> a = {1, 2, 3, 4, 5};
        AdvancedVector<int> b(a);
        AdvancedVector<int> c(std::move(a));
        EXPECT_TRUE(a.empty());
        b.push_front(100);
        c.push_front(200);
        EXPECT_EQ(c, AdvancedVector<int>({200, 1, 2, 3, 4, 5}));
        EXPECT_EQ(b, AdvancedVector<int>({100, 1, 2, 3, 4, 5}));
    }

    TEST(AdvancedVector, MergeConstructors) {

        AdvancedVector<StructWithCopyCounter> a({1, 2, 3});
        AdvancedVector<StructWithCopyCounter> b({4, 5, 6});

        EXPECT_EQ(n_copies, 12);
        int ond_n_copies = n_copies;

        AdvancedVector<StructWithCopyCounter> c(a, b);

        EXPECT_EQ(c.size(), 6);
        EXPECT_EQ(ond_n_copies + 6, n_copies);
        ond_n_copies = n_copies;

        AdvancedVector<StructWithCopyCounter> s(std::move(a), b);
        EXPECT_EQ(c.size(), 6);
        EXPECT_EQ(n_copies, ond_n_copies + 3);
    }

    TEST(AdvancedVector, AnotherConstructors) {
        n_copies = 0;
        AdvancedVector<StructWithCopyCounter> a({1, 2, 3});
        EXPECT_EQ(n_copies, 6);

        n_copies = 0;
        auto b = a;
        EXPECT_EQ(n_copies, 3);

        n_copies = 0;
        b = std::move(a);
        EXPECT_EQ(n_copies, 0);

        n_copies = 0;
        a = {1, 2, 3};
        EXPECT_EQ(n_copies, 6);
    }


    TEST(Node, NodeIterations) {
        nodeptr_t<int, int> A = nullptr;

        int n = rand() % 100;
        for (int i = 0; i < n; ++i) {
            A = Insert(A, rand() % (i + 1), rand(), rand());
        }

        auto B = GetLeft(A);
        for (int i = 0; i < n + 10; ++i) {
            EXPECT_EQ(B, GetByIndex(A, i));
            B = GetNext(B);
        }

        auto C = GetRight(A);
        for (int i = n - 1; i > -n; --i) {
            EXPECT_EQ(C, GetByIndex(A, i));
            C = GetPrev(C);
        }
    }

    TEST(AdvancedVector, Deque) {
        std::deque<int> deq;
        AdvancedVector<int> a;

        for (size_t i = 0; i < 1000; ++i) {
            int value = rand();
            if (rand() % 4 == 0) {
                deq.push_front(value);
                a.push_front(value);
            } else if (rand() % 4 == 1) {
                deq.push_back(value);
                a.push_back(value);
            } else if (rand() % 4 == 2) {
                if (deq.size() > 0) {
                    EXPECT_EQ(deq.size(), a.size());
                    EXPECT_EQ(deq.front(), a.front());
                }
            } else if (rand() % 4 == 3) {
                if (deq.size() > 0) {
                    EXPECT_EQ(deq.size(), a.size());
                    EXPECT_EQ(deq.back(), a.back());
                }
            }
        }

        std::deque<int> destination;
        std::copy(a.begin(), a.end(), std::back_inserter(destination));

        EXPECT_EQ(destination, deq);
    }

    TEST(AdvancedVector, Insert) {
        std::vector<int> v;
        AdvancedVector<int> a;

        for (size_t i = 0; i < 1000; ++i) {
            EXPECT_EQ(a.size(), v.size());
            int pos = rand() % (a.size() + 1);
            int value = i;
            a.insert(pos, value);
            v.insert(v.begin() + pos, value);
        }

        std::vector<int> destination;
        std::copy(a.begin(), a.end(), std::back_inserter(destination));
        EXPECT_EQ(destination, v);
    }

    TEST(AdvancedVector, InsertErase) {
        std::vector<int> v;
        AdvancedVector<int> a;

        for (size_t i = 0; i < 1000; ++i) {
            EXPECT_EQ(a.size(), v.size());
            if (rand() % 3 != 0) {
                int pos = rand() % (a.size() + 1);
                int value = i;
                a.insert(pos, value);
                v.insert(v.begin() + pos, value);
            } else if (a.size() > 0) {
                int pos = rand() % a.size();
                a.erase(pos);
                v.erase(v.begin() + pos);
            }
        }

        std::vector<int> destination;
        std::copy(a.begin(), a.end(), std::back_inserter(destination));
        EXPECT_EQ(destination, v);
    }

    TEST(AdvancedVector, Operators) {
        n_copies = 0;
        AdvancedVector<StructWithCopyCounter> A = {1, 2, 3};
        AdvancedVector<StructWithCopyCounter> B = {4, 5, 6};
        AdvancedVector<StructWithCopyCounter> C = {7, 8, 9};

        EXPECT_EQ(n_copies, 18);

        n_copies = 0;
        A += std::move(B);
        EXPECT_EQ(n_copies, 0);

        n_copies = 0;
        A = A + C;
        EXPECT_EQ(n_copies, 9);
        EXPECT_EQ(A.size(), 9);
        EXPECT_EQ(B.size(), 0);
        EXPECT_TRUE(B.empty());
        EXPECT_EQ(C.size(), 3);
    }

    TEST(AdvancedVector, Multiplication) {
        AdvancedVector<int> A = {1, 2, 3};
        AdvancedVector<int> B = {1, 2, 3, 1, 2, 3};
        AdvancedVector<int> C = {};

        EXPECT_EQ(A * 2, B);
        EXPECT_EQ(A * 0, C);
        A *= 2;
        EXPECT_EQ(A, B);
        A *= 0;
        EXPECT_EQ(A, C);
    }

    TEST(AdvancedVector, Subarray) {
        AdvancedVector<int> A = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto B = A.cut_subarray(2, 3);
        EXPECT_EQ(A, AdvancedVector<int>({1, 2, 6, 7, 8, 9, 10}));
        EXPECT_EQ(B, AdvancedVector<int>({3, 4, 5}));

        auto C = A.copy_subarray(3, 3);
        EXPECT_EQ(A, AdvancedVector<int>({1, 2, 6, 7, 8, 9, 10}));
        EXPECT_EQ(C, AdvancedVector<int>({7, 8, 9}));

        A.erase(2, 100);
        EXPECT_EQ(A, AdvancedVector<int>({1, 2}));
    }

    TEST(AdvancedVector, Iterators) {
        AdvancedVector<int> A;
        for (int i = 0; i < 1000; ++i) {
            A.push_back(i);
        }

        auto it = A.begin();
        for (int i = 0; i < 1000; ++i) {
            int t1 = rand() % A.size();
            int t2 = rand() % A.size();

            auto ptr1 = A.begin() + t1;
            auto ptr2 = A.begin() + t2;

            EXPECT_EQ(*ptr1, t1);
            EXPECT_EQ(*ptr2, t2);
            EXPECT_EQ(ptr2 - ptr1, t2 - t1);
            EXPECT_EQ(ptr1 - ptr2, t1 - t2);
        }
    }
}
