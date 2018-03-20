

#include "stack.hpp"
#include "queue.hpp"
#include "priority_queue.hpp"
#include <iostream>

int main()
{
    stack<int> st;
    for (int i = 0; i < 10; i++)
    {
        st.push(i);
    }

    for (int i = 0; i < 10; i++)
    {
        if (!st.empty()) 
        {
            std::cout << " stack top element value is " << st.pop() << "\n";
        }
    }

    queue<int> mq;
    for (int i = 0; i < 10; i ++)
    {
        mq.enqueue(i);
    }

    for (int i = 0; i < 10; i ++)
    {
        std::cout << "mq element value is " << mq.dequeue()<<"\n";
    }
 
#if 0
    typedef struct Message
    {
        int priority_;
        std::string content_;
        <
        >
    }
#endif

    priority_queue<int> pq(32);

    int a[] = {2, 5, 3, 100, 106, 88, 77, 99, 95};

    for (int i = 0; i < sizeof(a)/sizeof(int); i++)
    {
       pq.enqueue(a[i]);
    }

    pq.print();

    while (!pq.empty())
    {
        int value = 0;
        pq.dequeue(value);
        std::cout <<"dequeue element value is " << value << "\n";
        pq.print();
    }

    return 0;
}


