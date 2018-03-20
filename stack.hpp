
#ifndef __DONGNAOEDU_STACK_H__
#define __DONGNAOEDU_STACK_H__

#include <mutex>
#include <functional>
#include <thread>
#include <stdexcept>
#include <cstddef>
#include <iostream>


template <typename T>
class stack
{
public:
    stack();
   // stack(stack const & other);
   //  auto operator = (stack & const other) -> stack &;
    
    auto empty() const ->bool;
    auto size() const -> size_t;   /* unsigned int */
    
    auto push(T const & value)  -> void;
    auto pop()   -> T;

private:
    typedef struct Node
    {
        T     value;
        Node* next;
    };
   
    Node*         top_;  /*  */
    unsigned int  size_;
   
    mutable std::mutex mutexstack_;
    auto throw_is_empty() const -> void;

};

template <typename T>
stack<T>::stack() : top_(nullptr), size_(0)
{

}



template <typename T>
auto stack<T>::push(T const & value) -> void
{
    std::lock_guard<std::mutex> lock(mutexstack_);
    Node* old_top = top_;
    top_ = new Node;
    top_->value = value;
    top_->next = old_top;
    size_ ++;
}

template <typename T>
auto stack<T>::pop()   -> T
{
    std::lock_guard<std::mutex> lock(mutexstack_);
    Node* old_top = top_;
    if (size_ > 0)
    {
        top_ = top_->next;
        T value = old_top->value;
        delete old_top;
        size_ --;
        return value;
    }

    throw_is_empty();
}

template <typename T>
auto stack<T>::throw_is_empty() const -> void
{
    throw std::logic_error("Error!!!");
}

template <typename T>
auto stack<T>::empty() const ->bool
{
    std::lock_guard<std::mutex> lock(mutexstack_);
    return (0 == size_);
}

template <typename T>
auto stack<T>::size() const -> size_t
{
    std::lock_guard<std::mutex> lock(mutexstack_);
    return size_;
}

#endif

