#include <mutex>
#include <functional>
#include <thread>
#include <stdexcept>
#include <cstddef>
#include <iostream>

template <typename T>
class priority_queue
{
public:
    explicit
    priority_queue(unsigned int capacity);
    priority_queue(priority_queue const & other); /*strong*/
    auto operator =(priority_queue const & other) /*strong*/ -> priority_queue &;
    ~priority_queue();

    auto empty() const -> bool;
    auto size() const  -> size_t;

    auto enqueue(T const & value)  -> bool;
    auto dequeue(T& value)  -> bool;
    
    void print();

private:
    
    unsigned int capacity_;
    unsigned int size_;
    T*           heap_;           

    mutable std::mutex mutexpriority_queue_;

private:
    auto throw_is_empty() const -> void;
    void filter_up(int index);
    void filter_down(int index);
};


template<typename T>
auto priority_queue<T>::throw_is_empty() const->void
{
    throw std::logic_error("Error!"); 
}

template <typename T>/*noexcept*/
priority_queue<T>::priority_queue(unsigned int capacity) : capacity_(capacity), size_(0)
{
    heap_ =(T*)malloc(sizeof(T)*capacity_);
}

template <typename T>
priority_queue<T>::priority_queue(priority_queue const & st) : mutexpriority_queue_() 
{
    std::lock_guard<std::mutex> lock1(mutexpriority_queue_);
    std::lock_guard<std::mutex> lock2(st.mutexpriority_queue_);
    heap_ = (T*)malloc(sizeof(T)*st.capacity_);
    capacity_ = st.capacity_;
    size_ = st.size_;
    memcpy(heap_, st.heap_, sizeof(st.heap_));
}

template <typename T>
priority_queue<T>::~priority_queue()
{
    delete heap_;
    heap_ = nullptr;
}

template <typename T>
auto priority_queue<T>::operator=(const priority_queue & st)-> priority_queue &
{
    std::lock(mutexpriority_queue_, st.mutexpriority_queue_);
    std::lock_guard<std::mutex> lock1(mutexpriority_queue_, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(st.mutexpriority_queue_, std::adopt_lock);
    heap_ = (T*)malloc(sizeof(T)*st.capacity_);
    capacity_ = st.capacity_;
    size_ = st.size_;
    memcpy(heap_, st.heap_, sizeof(st.heap_));

    return *this;
}

template <typename T>
void priority_queue<T>::print()
{
    for (int i = 0 ; i < size_; i++)
    {
        std::cout << heap_[i]<<"  ";
    }
    std::cout << "\n";
}

template <typename T>
size_t  priority_queue<T>::size() const
{
    std::lock_guard<std::mutex> lock(mutexpriority_queue_);

    return size_;
}

template <typename T>
void priority_queue<T>::filter_up(int index)
{
    T value = heap_[index];
    int current_index = index;
    while(current_index > 0)
    {
        int parent_index = (current_index - 1) / 2;
        if (heap_[parent_index] < value)
        {
            T parent_value = heap_[parent_index];
            heap_[parent_index] = heap_[current_index];
            heap_[current_index] = parent_value;
        }
        else
        {
            break;
        }
        current_index = parent_index;
    }
}

template <typename T>
auto priority_queue<T>::enqueue(T const &value)-> bool
{
    std::lock_guard<std::mutex> lock(mutexpriority_queue_);
    if (size_ == capacity_)
        return false;
    heap_[size_] = value;
    filter_up(size_);
    size_++;
    return true;
}

template <typename T>
void priority_queue<T>::filter_down(int index)
{
    int parent_index = 0;
    heap_[parent_index] = heap_[index];
    int child_index = parent_index * 2 + 1;  // left child
    while(child_index + 1  < index)
    {
        /* select largest one of the two children to exchange with parent  */
        if (heap_[child_index] < heap_[child_index + 1])
        {
            child_index ++;
        }
      
        /* if parent's value less than child's value, exchange them */
        if(heap_[parent_index] < heap_[child_index])
        {
            T value = heap_[parent_index];
            heap_[parent_index] = heap_[child_index];
            heap_[child_index] = value;
        }
        else
        {
            break;
        }
        parent_index = child_index;
        child_index  = parent_index * 2 + 1;  // left child
    }
}

template <typename T>
auto priority_queue<T>::dequeue(T& value)-> bool
{
    std::lock_guard<std::mutex> lock(mutexpriority_queue_);
    if (0 == size_)
        return false;
    value = heap_[0];
    //filter_downi(size_);
    size_ --;
    filter_down(size_);
    return true;
}

template <typename T>/*noexcept*/
auto priority_queue<T>::empty()const->bool 
{
    std::lock_guard<std::mutex> lock(mutexpriority_queue_);
    return(size_ == 0);
}
