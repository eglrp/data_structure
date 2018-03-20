
#ifndef __DONGNAOEDU_QUEUE_H__
#define __DONGNAOEDU_QUEUE_H__

#include <mutex>
#include <functional>
#include <thread>
#include <stdexcept>
#include <cstddef>
#include <iostream>

template <typename T>
class queue
{
public:
    explicit
    queue();
    queue(queue const & other); /*strong*/
    auto operator =(queue const & other) /*strong*/ -> queue &;

    auto empty() const /*noexcept*/ -> bool;
    auto size() const /*noexcept*/ -> size_t;

    auto enqueue(T const & value) /*strong*/ -> void;
    auto dequeue() /*strong*/ -> T ;

private:
    typedef struct Node
    {
        T value;
        Node* next;
    };
        
    Node* head_;
    Node* tail_;
    unsigned int size_;

    mutable std::mutex mutexqueue_;
    auto throw_is_empty() const -> void;              /*strong*/
};


template<typename T>
auto queue<T>::throw_is_empty() const->void
{
    throw std::logic_error("Error!"); 
}

template <typename T>/*noexcept*/
queue<T>::queue() : head_(nullptr), tail_(nullptr), size_(0)
{

}

template <typename T>
queue<T>::queue(queue const & st) : mutexqueue_() 
{
    std::lock_guard<std::mutex> lock1(mutexqueue_);
    std::lock_guard<std::mutex> lock2(st.mutexqueue_);

    Node* node = st.head_;
    size_ = 0;
    while( nullptr != node )
    {
        Node* old_tail = tail_;
        tail_ = new Node;
        tail_->value = node->value;
        tail_->next = nullptr;
        if ( 0 == size_ )
        {
            head_ = tail_;
        }
        else
        {
            old_tail->next = tail_;
        }
        size_ ++;
        node = node->next;
    }
}

template <typename T>
auto queue<T>::operator=(const queue & st)-> queue &
{
    std::lock(mutexqueue_, st.mutexqueue_);
    std::lock_guard<std::mutex> lock1(mutexqueue_, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(st.mutexqueue_, std::adopt_lock);
    if (this != &st)
    {
        Node* node = st.head_;
        size_ = 0;
        while( nullptr != node )
        {
            Node* old_tail = tail_;
            tail_ = new Node;
            tail_->value = node->value;
            tail_->next = nullptr;
            if ( 0 == size_ )
            {
                head_ = tail_;
            }
            else
            {
                old_tail->next = tail_;
            }
            size_ ++;
            node = node->next;
        }
    }
    return *this;
}

template <typename T>
size_t  queue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mutexqueue_);

    return size_;
}

template <typename T>
void queue<T>::enqueue(T const &value)
{
    std::lock_guard<std::mutex> lock(mutexqueue_);
    Node* old_tail = tail_;
    tail_ = new Node;
    tail_->value = value;
    tail_->next = nullptr;
    if ( 0 == size_)
    {
        head_ = tail_;
    }
    else
    {
        old_tail->next = tail_;
    }
    size_ ++;
}

template <typename T>
auto queue<T>::dequeue()-> T
{
    std::lock_guard<std::mutex> lock(mutexqueue_);
    if (size_ > 0)
    {
        T value = head_->value;
        Node* old_head = head_;
        head_ = head_->next;
        delete old_head;
        size_ --;
        return value;
    }
    
    this->throw_is_empty();
}

template <typename T>/*noexcept*/
auto queue<T>::empty()const->bool 
{
    std::lock_guard<std::mutex> lock(mutexqueue_);
    return(size_ == 0);
}

#endif

