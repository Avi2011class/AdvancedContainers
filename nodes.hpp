#pragma once

#include <memory>
#include <iostream>
#include <iterator>

template <typename ValueT, typename PriorityT>
class Node;

template <typename ValueT, typename PriorityT>
using nodeptr_t = std::shared_ptr<Node<ValueT, PriorityT>>;

template <typename ValueT, typename PriorityT>
using weak_nodeptr_t = std::weak_ptr<Node<ValueT, PriorityT>>;

template <typename ValueT, typename PriorityT>
class Node {
private:
    ValueT value_;
    PriorityT priority_;
    unsigned subtree_size_;

    nodeptr_t<ValueT, PriorityT> left_;
    nodeptr_t<ValueT, PriorityT> right_;

    weak_nodeptr_t<ValueT, PriorityT> parent_;
    weak_nodeptr_t<ValueT, PriorityT> my_shared_block_;

public:
    using value_type = ValueT;
    using priority_type = PriorityT;

    Node(const ValueT& value, const PriorityT& priority,
         nodeptr_t<ValueT, PriorityT> left=nullptr,
         nodeptr_t<ValueT, PriorityT> right=nullptr)
            :
            value_(value),
            priority_(priority),
            subtree_size_(1),
            left_(left),
            right_(right),
            parent_(nodeptr_t<ValueT, PriorityT>(nullptr)),
            my_shared_block_(nodeptr_t<ValueT, PriorityT>(nullptr)) {
    }

    Node(const Node<ValueT, PriorityT>&) = default;
    Node(Node<ValueT, PriorityT>&&) = default;

    Node<ValueT, PriorityT>& operator=(const Node<ValueT, PriorityT>&) = default;
    Node<ValueT, PriorityT>& operator=(Node<ValueT, PriorityT>&&) = default;

    const ValueT& GetValue() const {
        return value_;
    }

    ValueT& GetValue() {
        return value_;
    }

    const PriorityT& GetPriority() const {
        return priority_;
    }

    unsigned GetSubtreeSize() const {
        return subtree_size_;
    }

    nodeptr_t<ValueT, PriorityT> GetLeft() const {
        return left_;
    }

    nodeptr_t<ValueT, PriorityT> GetRight() const {
        return right_;
    }


    void SetValue(const ValueT& value) {
        value_ = value;
    }

    void SetValue(ValueT&& value) {
        value_ = value;
    }

    void SetLeft(nodeptr_t<ValueT, PriorityT> left) {
        left_ = left;
        Update();
    }

    void SetRight(nodeptr_t<ValueT, PriorityT> right) {
        right_ = right;
        Update();
    }

    void SetMySharedBlock(nodeptr_t<ValueT, PriorityT> my_shared_block) {
        my_shared_block_ = my_shared_block;
    }

    nodeptr_t<ValueT, PriorityT> GetMySharedBlock() const {
        return my_shared_block_.lock();
    }

    void SetParent(nodeptr_t<ValueT, PriorityT> parent) {
        parent_ = parent;
    }

    nodeptr_t<ValueT, PriorityT> GetParent() const {
        return parent_.lock();
    }

    void Update() {
        subtree_size_ = 1;
        if (left_ != nullptr) {
            subtree_size_ += left_->GetSubtreeSize();
            left_->SetParent(my_shared_block_.lock());
        }
        if (right_ != nullptr) {
            subtree_size_ += right_->GetSubtreeSize();
            right_->SetParent(my_shared_block_.lock());
        }
    }

    void UpdateUntilRoot() {
        Update();
        auto parent = parent_.lock();
        if (parent != nullptr) {
            parent->UpdateUntilRoot();
        }
    }
};

template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
MakeNodePtrT(const ValueT& value, const PriorityT& priority) {
    nodeptr_t<ValueT, PriorityT> result = std::make_shared<Node<ValueT, PriorityT>>(value, priority);
    result->SetMySharedBlock(result);
    return result;
}

template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
MakeNodePtrT(nodeptr_t<ValueT, PriorityT> node) {
    nodeptr_t<ValueT, PriorityT> result =
            std::make_shared<Node<ValueT, PriorityT>>(node->GetValue(), node->GetPriority());
    result->SetMySharedBlock(result);
    return result;
}

template <typename ValueT, typename PriorityT>
std::ostream& operator<<(std::ostream& output_stream, const Node<ValueT, PriorityT>& node) {
    output_stream << "size: " << node.GetSubtreeSize() << ", value: " << node.GetValue()
                  << ", priority: " << node.GetPriority();
    return output_stream;
}



template <typename ValueT, typename PriorityT>
std::tuple<nodeptr_t<ValueT, PriorityT>, nodeptr_t<ValueT, PriorityT>>
Split(nodeptr_t<ValueT, PriorityT> node, unsigned index) {
    if (node == nullptr) {
        return std::make_tuple(nullptr, nullptr);
    } else {
        unsigned elements_before = 0;
        if (node->GetLeft() != nullptr) {
            elements_before = node->GetLeft()->GetSubtreeSize();
        }
        if (elements_before >= index) {
            auto[split_first, split_second] = Split(node->GetLeft(), index);
            node->SetLeft(split_second);
            return std::make_tuple(split_first, node);
        } else {
            auto[split_first, split_second] = Split(node->GetRight(), index - elements_before - 1);
            node->SetRight(split_first);
            return std::make_tuple(node, split_second);
        }
    }
}

template <typename ValueT, typename PriorityT, typename ... Args>
decltype(auto)
Split(nodeptr_t<ValueT, PriorityT> node, unsigned index, Args ... args) {
    auto [split_left, split_right] = Split(node, index);
    return std::tuple_cat(std::make_tuple(split_left), Split(split_right, args ...));
}



template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
Merge(nodeptr_t<ValueT, PriorityT> left, nodeptr_t<ValueT, PriorityT> right) {
    if (left == nullptr) {
        return right;
    } else if (right == nullptr) {
        return left;
    } else if (left->GetPriority() > right->GetPriority()) {
        left->SetRight(Merge(left->GetRight(), right));
        return left;
    } else {
        right->SetLeft(Merge(left, right->GetLeft()));
        return right;
    }
}

template <typename ValueT, typename PriorityT, typename... OtherT>
nodeptr_t<ValueT, PriorityT>
Merge(nodeptr_t<ValueT, PriorityT> node, OtherT... other) {
    return Merge(node, Merge(other...));
}


template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
Insert(nodeptr_t<ValueT, PriorityT> node,
       unsigned position,
       const typename nodeptr_t<ValueT, PriorityT>::element_type::value_type& value,
       const typename nodeptr_t<ValueT, PriorityT>::element_type::priority_type& priority) {
    if (node == nullptr) {
        return MakeNodePtrT<ValueT, PriorityT>(value, priority);
    } else if (node->GetPriority() < priority) {
        auto [split_left, split_right] = Split(node, position);
        auto new_node = MakeNodePtrT<ValueT, PriorityT>(value, priority);
        return Merge(split_left, Merge(new_node, split_right));
    } else {
        unsigned elements_before = 0;
        if (node->GetLeft() != nullptr) {
            elements_before = node->GetLeft()->GetSubtreeSize();
        }
        if (position <= elements_before) {
            node->SetLeft(Insert(node->GetLeft(), position, value, priority));
            return node;
        } else {
            node->SetRight(Insert(node->GetRight(), position - 1 - elements_before, value, priority));
            return node;
        }
    }
}



template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
Erase(nodeptr_t<ValueT, PriorityT> node, unsigned position) {
    if (node == nullptr) {
        return nullptr;
    }

    unsigned elements_before = 0;
    if (node->GetLeft() != nullptr) {
        elements_before = node->GetLeft()->GetSubtreeSize();
    }

    if (position == elements_before) {
        return Merge(node->GetLeft(), node->GetRight());
    } else if (elements_before > position) {
        node->SetLeft(Erase(node->GetLeft(), position));
        return node;
    } else {
        node->SetRight(Erase(node->GetRight(), position - elements_before - 1));
        return node;
    }
}


template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
GetByIndex(nodeptr_t <ValueT, PriorityT> node, unsigned index) {
    if (node == nullptr) {
        return nullptr;
    } else {
        unsigned elements_before = 0;
        if (node->GetLeft() != nullptr) {
            elements_before = node->GetLeft()->GetSubtreeSize();
        }
        if (elements_before == index) {
            return node;
        } else if (elements_before > index) {
            return GetByIndex(node->GetLeft(), index);
        } else {
            return GetByIndex(node->GetRight(), index - elements_before - 1);
        }
    }
}

template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
DeepCopy(nodeptr_t<ValueT, PriorityT> node) {
    if (node == nullptr) {
        return nullptr;
    } else {
        auto left_copy = DeepCopy(node->GetLeft());
        auto right_copy = DeepCopy(node->GetRight());
        auto new_node = MakeNodePtrT<ValueT, PriorityT>(node);
        new_node->SetLeft(left_copy);
        new_node->SetRight(right_copy);
        return new_node;
    }
}

template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
GetNext(nodeptr_t <ValueT, PriorityT> node) {
    if (node == nullptr) {
        return nullptr;
    } else if (node->GetRight() != nullptr) {
        auto iter = node->GetRight();
        while (iter->GetLeft() != nullptr) {
            iter = iter->GetLeft();
        }
        return iter;
    } else {
        while (node->GetParent() != nullptr) {
            if (node == node->GetParent()->GetLeft()) {
                return node->GetParent();
            } else {
                node = node->GetParent();
            }
        }
        return nullptr;
    }
}


template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
GetPrev(nodeptr_t <ValueT, PriorityT> node) {
    if (node == nullptr) {
        return nullptr;
    } else if (node->GetLeft() != nullptr) {
        auto iter = node->GetLeft();
        while (iter->GetRight() != nullptr) {
            iter = iter->GetRight();
        }
        return iter;
    } else {
        while (node->GetParent() != nullptr) {
            if (node == node->GetParent()->GetRight()) {
                return node->GetParent();
            } else {
                node = node->GetParent();
            }
        }
        return nullptr;
    }
}


template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
GetLeft(nodeptr_t<ValueT, PriorityT> node) {
    if (node == nullptr) {
        return nullptr;
    } else {
        while (node->GetLeft() != nullptr) {
            node = node->GetLeft();
        }
        return node;
    }
}

template <typename ValueT, typename PriorityT>
std::ptrdiff_t
GetElementsBefore(nodeptr_t<ValueT, PriorityT> node) {
    if (node == nullptr) {
        return 0;
    } else {
        std::ptrdiff_t left_child_size = 0;
        if (node->GetLeft() != nullptr) {
            left_child_size = node->GetLeft()->GetSubtreeSize();
        }

        std::ptrdiff_t left_branches_size = 0;
        while (node->GetParent() != nullptr) {
            auto parent = node->GetParent();
            if (node == parent->GetRight()) {
                left_branches_size += 1;
                if (parent->GetLeft() != nullptr) {
                    left_branches_size += parent->GetLeft()->GetSubtreeSize();
                }
            }
            node = parent;
        }

        return left_branches_size + left_child_size;
    }
}

template <typename ValueT, typename PriorityT>
nodeptr_t<ValueT, PriorityT>
GetRight(nodeptr_t<ValueT, PriorityT> node) {
    if (node == nullptr) {
        return nullptr;
    } else {
        while (node->GetRight() != nullptr) {
            node = node->GetRight();
        }
        return node;
    }
}

template <typename ValueT, typename PriorityT>
void
Dump(nodeptr_t<ValueT, PriorityT> node, unsigned offset = 0) {
    if (node != nullptr) {
        Dump(node->GetLeft(), offset + 1);
        for (size_t i = 0; i < offset; ++i) {
            std::cout << "  ";
        }
        std::cout << *node << std::endl;
        Dump(node->GetRight(), offset + 1);
    }
    if (offset == 0) {
        std::cout << "-------------------------------------------" << std::endl;
    }
}