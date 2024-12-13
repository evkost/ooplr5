#ifndef STACK_H
#define STACK_H

#include <vector>
#include <iterator>
#include <cassert>

template <typename T>
class Stack {
public:
    using allocator_type = std::pmr::polymorphic_allocator<T>;
    using value_type = T;
    using size_type = std::size_t;

    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(pointer ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() { return ptr_; }

        iterator& operator++() {
            ++ptr_;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

    private:
        pointer ptr_;
    };

    Stack(const allocator_type& alloc = allocator_type());

    void push(const T& value);
    void push(T&& value);
    void pop();
    T& top();
    const T& top() const;
    size_type size() const;
    bool empty() const;

    iterator begin();
    iterator end();

private:
    std::vector<T, allocator_type> data_;
};


template <typename T>
Stack<T>::Stack(const allocator_type& alloc) : data_(alloc) {}

template <typename T>
void Stack<T>::push(const T& value) {
    data_.push_back(value);
}

template <typename T>
void Stack<T>::push(T&& value) {
    data_.push_back(std::move(value));
}

template <typename T>
void Stack<T>::pop() {
    assert(!data_.empty() && "Pop from empty stack");
    data_.pop_back();
}

template <typename T>
T& Stack<T>::top() {
    assert(!data_.empty() && "Top from empty stack");
    return data_.back();
}

template <typename T>
const T& Stack<T>::top() const {
    assert(!data_.empty() && "Top from empty stack");
    return data_.back();
}

template <typename T>
typename Stack<T>::size_type Stack<T>::size() const {
    return data_.size();
}

template <typename T>
bool Stack<T>::empty() const {
    return data_.empty();
}

template <typename T>
typename Stack<T>::iterator Stack<T>::begin() {
    return iterator(data_.data());
}

template <typename T>
typename Stack<T>::iterator Stack<T>::end() {
    return iterator(data_.data() + data_.size());
}

#endif // STACK_H
