#include <iostream>
#include <memory>
#include <cmath>
#include <iterator>
#include <optional>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include <random>
#include <tuple>
#include <type_traits>
#include <exception>
#include <stdexcept>

#include <nodes.hpp>

template <typename T, class RandomGenerator = std::mt19937_64>
class AdvancedVector {
private:
    nodeptr_t<T, uint64_t> storage_;
    RandomGenerator gen;

    explicit AdvancedVector(nodeptr_t<T, uint64_t> node);

public:
    using value_type = T;

    class iterator;

    AdvancedVector() = default;
    AdvancedVector(const AdvancedVector<T, RandomGenerator>& other);
    AdvancedVector(AdvancedVector<T, RandomGenerator>&& other) noexcept;
    AdvancedVector<T, RandomGenerator>& operator=(const AdvancedVector<T, RandomGenerator>& other);
    AdvancedVector<T, RandomGenerator>& operator=(AdvancedVector<T, RandomGenerator>&& other) noexcept;
    AdvancedVector<T, RandomGenerator>& operator=(const std::initializer_list<T>& data);
    AdvancedVector<T, RandomGenerator>& operator=(std::initializer_list<T>&& data) noexcept;
    AdvancedVector(std::initializer_list<T> list);

    template <typename ... Tail>
    explicit AdvancedVector(AdvancedVector<T, RandomGenerator>&& head, Tail ... tail);
    template <typename ... Tail>
    explicit AdvancedVector(const AdvancedVector<T, RandomGenerator>& head, Tail ... tail);

    template <typename It, typename std::enable_if<
            std::is_convertible<typename std::iterator_traits<It>::value_type, T >::value, int
            >::type = 0>
    AdvancedVector(It first, It last);

    bool operator==(const AdvancedVector<T>& other) const;

    size_t size() const;
    bool empty() const;
    void clear();

    const T& operator[](unsigned index) const;
    T& operator[](unsigned index);

    void push_back(const T& value);
    void push_front(const T& value);

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void erase(unsigned position);
    void erase(unsigned position, unsigned length);
    void insert(unsigned position, const T& value);
    void insert(unsigned position, const AdvancedVector<T, RandomGenerator>& data);
    void insert(unsigned position, AdvancedVector<T, RandomGenerator>&& data);

//    void insert(iterator pos, const T& value);
//    void insert(iterator pos, const AdvancedVector<T, RandomGenerator>& data);
//    void insert(iterator pos, AdvancedVector<T, RandomGenerator>&& data);

    AdvancedVector<T, RandomGenerator> cut_subarray(unsigned position, unsigned length);
    AdvancedVector<T, RandomGenerator> copy_subarray(unsigned position, unsigned length);

    iterator begin() const;
    iterator end() const;

    AdvancedVector<T, RandomGenerator>& operator+=(const AdvancedVector<T, RandomGenerator>& rhs);
    AdvancedVector<T, RandomGenerator>& operator+=(AdvancedVector<T, RandomGenerator>&& rhs);

    AdvancedVector<T, RandomGenerator> operator+(const AdvancedVector<T, RandomGenerator>& rhs);
    AdvancedVector<T, RandomGenerator> operator+(AdvancedVector<T, RandomGenerator>&& rhs);

    AdvancedVector<T, RandomGenerator>& operator*=(size_t multiplier);
    AdvancedVector<T, RandomGenerator> operator*(size_t multiplier);

    class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    private:
        nodeptr_t<T, uint64_t> iterator_node_;
        nodeptr_t<T, uint64_t> const *root_;

    public:
        iterator() : iterator_node_(nullptr), root_(nullptr) {};
        iterator(nodeptr_t<T, uint64_t> node, nodeptr_t<T, uint64_t> const *root) 
            : iterator_node_(node), root_(root) {}
        iterator(const iterator&) = default;
        iterator(iterator&&) noexcept = default;

        iterator& operator=(const iterator&) = default;
        iterator& operator=(iterator&&) noexcept = default;
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        iterator& operator++();
        const iterator operator++(int);
        iterator& operator--();
        const iterator operator--(int);

        iterator operator+(size_t offset) const;
        iterator operator-(size_t offset) const;

        std::ptrdiff_t operator-(const iterator& rhs) const;

        const T& operator*() const;
        T const *operator->() const;

        std::ptrdiff_t GetPosition() const;
    };
};

template <typename T, class RandomGenerator>
bool
AdvancedVector<T, RandomGenerator>::iterator::operator==(
        const AdvancedVector<T, RandomGenerator>::iterator& other) const {
    return iterator_node_ == other.iterator_node_ && root_ == other.root_;
}

template <typename T, class RandomGenerator>
bool
AdvancedVector<T, RandomGenerator>::iterator::operator!=(
        const AdvancedVector<T, RandomGenerator>::iterator& other) const {
    return iterator_node_ != other.iterator_node_ || root_ != other.root_;
}

template <typename T, class RandomGenerator>
typename AdvancedVector<T, RandomGenerator>::iterator&
AdvancedVector<T, RandomGenerator>::iterator::operator++() {
    if (iterator_node_ != nullptr) {
        iterator_node_ = GetNext(iterator_node_);
    }
    return *this;
}

template <typename T, class RandomGenerator>
const typename AdvancedVector<T, RandomGenerator>::iterator
AdvancedVector<T, RandomGenerator>::iterator::operator++(int) {
    auto result = *this;
    if (iterator_node_ != nullptr) {
        iterator_node_ = GetNext(iterator_node_);
    }
    return result;
}

template <typename T, class RandomGenerator>
typename AdvancedVector<T, RandomGenerator>::iterator&
AdvancedVector<T, RandomGenerator>::iterator::operator--() {
    if (iterator_node_ != nullptr) {
        iterator_node_ = GetPrev(iterator_node_);
    } else {
        iterator_node_ = GetLeft(*root_);
    }
    return *this;
}

template <typename T, class RandomGenerator>
const typename AdvancedVector<T, RandomGenerator>::iterator
AdvancedVector<T, RandomGenerator>::iterator::operator--(int) {
    auto result = *this;
    if (iterator_node_ != nullptr) {
        iterator_node_ = GetPrev(iterator_node_);
    } else {
        iterator_node_ = GetLeft(*root_);
    }
    return result;
}

template <typename T, class RandomGenerator>
const T&
AdvancedVector<T, RandomGenerator>::iterator::operator*() const {
    return iterator_node_->GetValue();
}

template <typename T, class RandomGenerator>
T
const *AdvancedVector<T, RandomGenerator>::iterator::operator->() const {
    return &(iterator_node_->GetValue());
}

template <typename T, class RandomGenerator>
typename AdvancedVector<T, RandomGenerator>::iterator
AdvancedVector<T, RandomGenerator>::iterator::operator+(size_t offset) const {
    if (offset < 10) {
        iterator result(*this);
        for (size_t i = 0; i < offset; ++i) {
            ++result;
        }
        return result;
    }
    unsigned position = GetElementsBefore(iterator_node_);
    return iterator(GetByIndex(*root_, position + offset), root_);
}

template <typename T, class RandomGenerator>
typename AdvancedVector<T, RandomGenerator>::iterator
AdvancedVector<T, RandomGenerator>::iterator::operator-(size_t offset) const {
    if (offset < 10) {
        iterator result(*this);
        for (size_t i = 0; i < offset; ++i) {
            --result;
        }
        return result;
    }
    unsigned position = GetElementsBefore(iterator_node_);
    return iterator(GetByIndex(*root_, position - offset), root_);
}

template <typename T, class RandomGenerator>
std::ptrdiff_t AdvancedVector<T, RandomGenerator>::iterator::GetPosition() const {
    if (*root_ == nullptr) {
        return 0;
    } else if (iterator_node_ == nullptr) {
        return (*root_)->GetSubtreeSize();
    } else {
        return GetElementsBefore(iterator_node_);
    }
}

template <typename T, class RandomGenerator>
std::ptrdiff_t
AdvancedVector<T, RandomGenerator>::iterator::operator-(const typename AdvancedVector<T, RandomGenerator>::iterator& rhs) const {
    return GetPosition() - rhs.GetPosition();
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>::AdvancedVector(const AdvancedVector<T, RandomGenerator>& other)
        : storage_(DeepCopy(other.storage_)), gen() {
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>::AdvancedVector(AdvancedVector<T, RandomGenerator>&& other) noexcept
        : storage_(other.storage_), gen() {
    other.storage_ = nullptr;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>&
AdvancedVector<T, RandomGenerator>::operator=(const AdvancedVector<T, RandomGenerator>& other) {
    storage_ = DeepCopy(other.storage_);
    return *this;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>&
AdvancedVector<T, RandomGenerator>::operator=(AdvancedVector<T, RandomGenerator>&& other) noexcept {
    storage_ = other.storage_;
    other.storage_ = nullptr;
    return *this;
}

template <typename T, class RandomGenerator>
bool
AdvancedVector<T, RandomGenerator>::operator==(const AdvancedVector<T, std::mt19937_64>& other) const {
    if (size() != other.size()) {
        return false;
    } else {
        for (auto it = begin(), other_it = other.begin();
                it != end() && other_it != other.end(); ++it, ++other_it) {
            if (*it != *other_it) {
                return false;
            }
        }
        return true;
    }
}

template <typename T, class RandomGenerator>
size_t
AdvancedVector<T, RandomGenerator>::size() const {
    return (storage_ == nullptr) ? 0 : storage_->GetSubtreeSize();
}

template <typename T, class RandomGenerator>
bool
AdvancedVector<T, RandomGenerator>::empty() const {
    return storage_ == nullptr;
}

template <typename T, class RandomGenerator>
const T&
AdvancedVector<T, RandomGenerator>::operator[](unsigned index) const {
    nodeptr_t<T, uint64_t> res = GetByIndex(storage_, index);
    return res->GetValue();
}

template <typename T, class RandomGenerator>
T&
AdvancedVector<T, RandomGenerator>::operator[](unsigned index) {
    nodeptr_t<T, uint64_t> res = GetByIndex(storage_, index);
    return res->GetValue();
}

template <typename T, class RandomGenerator>
void
AdvancedVector<T, RandomGenerator>::push_back(const T& value) {
    storage_ = Insert(storage_, size(), value, gen());
}

template <typename T, class RandomGenerator>
void
AdvancedVector<T, RandomGenerator>::push_front(const T& value) {
    storage_ = Insert(storage_, 0, value, gen());
}

template <typename T, class RandomGenerator>
T&
AdvancedVector<T, RandomGenerator>::front() {
    return operator[](0);
}

template <typename T, class RandomGenerator>
const T&
AdvancedVector<T, RandomGenerator>::front() const {
    return operator[](0);
}

template <typename T, class RandomGenerator>
T&
AdvancedVector<T, RandomGenerator>::back() {
    return operator[](size() - 1);
}

template <typename T, class RandomGenerator>
void
AdvancedVector<T, RandomGenerator>::erase(unsigned position) {
    if (position < size()) {
        storage_ = Erase(storage_, position);
    }
}

template <typename T, class RandomGenerator>
void
AdvancedVector<T, RandomGenerator>::insert(unsigned position, const T& value) {
    storage_ = Insert(storage_, position, value, gen());
}

template <typename T, class RandomGenerator>
void
AdvancedVector<T, RandomGenerator>::insert(unsigned position, const AdvancedVector<T, RandomGenerator>& data) {
    auto[split_first, split_second] = Split(storage_, position);
    storage_ = Merge(split_first, DeepCopy(data.storage_), split_second);
}

template <typename T, class RandomGenerator>
void
AdvancedVector<T, RandomGenerator>::insert(unsigned position, AdvancedVector<T, RandomGenerator>&& data) {
    auto[split_first, split_second] = Split(storage_, position);
    storage_ = Merge(split_first, data.storage_, split_second);
    data.storage_ = nullptr;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>
AdvancedVector<T, RandomGenerator>::cut_subarray(unsigned position, unsigned length) {
    auto[head, subarray_storage, tail] = Split(storage_, position, length);
    storage_ = Merge(head, tail);
    return AdvancedVector<T, RandomGenerator>(subarray_storage);
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>
AdvancedVector<T, RandomGenerator>::copy_subarray(unsigned position, unsigned length) {
    auto[head, subarray_storage, tail] = Split(storage_, position, length);
    auto subarray_storage_copy = DeepCopy(subarray_storage);
    storage_ = Merge(head, subarray_storage, tail);
    return AdvancedVector<T, RandomGenerator>(subarray_storage_copy);
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>::AdvancedVector(std::initializer_list<T> list) : storage_(nullptr) {
    for (const auto& elem : list) {
        push_back(elem);
    }
}

template <typename T, class RandomGenerator>
typename AdvancedVector<T, RandomGenerator>::iterator
AdvancedVector<T, RandomGenerator>::begin() const {
    return iterator(GetLeft(storage_), &storage_);
}

template <typename T, class RandomGenerator>
typename AdvancedVector<T, RandomGenerator>::iterator
AdvancedVector<T, RandomGenerator>::end() const {
    return iterator(nullptr, &storage_);
}

template <typename T, class RandomGenerator>
const T& AdvancedVector<T, RandomGenerator>::back() const {
    return operator[](size() - 1);
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>&
AdvancedVector<T, RandomGenerator>::operator+=(const AdvancedVector<T, RandomGenerator>& rhs) {
    storage_ = Merge(storage_, DeepCopy(rhs.storage_));
    return *this;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>::AdvancedVector(nodeptr_t<T, unsigned long> node) : storage_(node) {
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>&
AdvancedVector<T, RandomGenerator>::operator+=(AdvancedVector<T, RandomGenerator>&& rhs) {
    storage_ = Merge(storage_, rhs.storage_);
    rhs.storage_ = nullptr;
    return *this;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>
AdvancedVector<T, RandomGenerator>::operator+(const AdvancedVector<T, RandomGenerator>& rhs) {
    return AdvancedVector<T, RandomGenerator>(Merge(DeepCopy(storage_), DeepCopy(rhs.storage_)));
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>
AdvancedVector<T, RandomGenerator>::operator+(AdvancedVector<T, RandomGenerator>&& rhs) {
    auto tmp = rhs.storage_;
    rhs.storage_ = nullptr;
    return AdvancedVector<T, RandomGenerator>(Merge(DeepCopy(storage_), tmp));
}

template <typename T, class RandomGenerator>
template <typename... Tail>
AdvancedVector<T, RandomGenerator>::AdvancedVector(AdvancedVector<T, RandomGenerator>&& head, Tail... tail) {
    AdvancedVector<T, RandomGenerator> tail_vector(std::forward<AdvancedVector<T, RandomGenerator>>(tail)...);
    storage_ = Merge(head.storage_, tail_vector.storage_);
    head.storage_ = nullptr;
}

template <typename T, class RandomGenerator>
template <typename... Tail>
AdvancedVector<T, RandomGenerator>::AdvancedVector(const AdvancedVector<T, RandomGenerator>& head, Tail... tail) {
    AdvancedVector<T, RandomGenerator> tail_vector(std::forward<AdvancedVector<T, RandomGenerator>>(tail)...);
    storage_ = Merge(DeepCopy(head.storage_), tail_vector.storage_);
}

template <typename T, class RandomGenerator>
void AdvancedVector<T, RandomGenerator>::clear() {
    storage_ = nullptr;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>&
AdvancedVector<T, RandomGenerator>::operator=(const std::initializer_list<T>& data) {
    storage_ = nullptr;
    for (const auto& elem : data) {
        push_back(elem);
    }
    return *this;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>&
AdvancedVector<T, RandomGenerator>::operator=(std::initializer_list<T>&& data) noexcept {
    storage_ = nullptr;
    for (const auto& elem : data) {
        push_back(elem);
    }
    return *this;
}

template <typename T, class RandomGenerator>
template <typename It, typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<It>::value_type, T >::value, int
        >::type>
AdvancedVector<T, RandomGenerator>::AdvancedVector(It first, It last) : storage_(nullptr) {
    for (It iter = first; iter != last; ++iter) {
        push_back(*iter);
    }
}

template <typename T, class RandomGenerator>
void AdvancedVector<T, RandomGenerator>::erase(unsigned position, unsigned length) {
    auto [first, second, third] = Split(storage_, position, length);
    storage_ = Merge(first, third);
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator>& AdvancedVector<T, RandomGenerator>::operator*=(size_t multiplier) {
    auto tmp_storage = storage_;
    storage_ = nullptr;
    for (size_t iteration = 0; iteration < multiplier; ++iteration) {
        storage_ = Merge(storage_, DeepCopy(tmp_storage));
    }
    return *this;
}

template <typename T, class RandomGenerator>
AdvancedVector<T, RandomGenerator> AdvancedVector<T, RandomGenerator>::operator*(size_t multiplier) {
    nodeptr_t<T, uint64_t> new_storage_ = nullptr;
    for (size_t iteration = 0; iteration < multiplier; ++iteration) {
        new_storage_ = Merge(new_storage_, DeepCopy(storage_));
    }
    return AdvancedVector<T, RandomGenerator>(new_storage_);
}

template <typename T>
std::ostream&
operator<<(std::ostream& output_stream, const AdvancedVector<T>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        output_stream << data[i] << " ";
    }
    return output_stream;
}
