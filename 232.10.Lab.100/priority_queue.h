/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
public:

   //
   // construct
   //
   priority_queue() 
   {
   }
   priority_queue(const priority_queue &  rhs)  
   { 
       //*this = rhs;
   }
   priority_queue(priority_queue && rhs)  
   { 
       //*this = std::move(rhs); 
   }
   template <class Iterator>
   priority_queue(Iterator first, Iterator last) 
   {
       container.reserve(last - first);
       for (Iterator it = first; it != last; it++) 
       {
           push(*it);
       }
   }
   explicit priority_queue (custom::vector<T> && rhs) 
   {
       container = rhs;
   }
   explicit priority_queue (custom::vector<T>& rhs)
   {
       container = rhs;
   }
   ~priority_queue() {  }

   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);     

   //
   // Remove
   //
   void  pop(); 

   //
   // Status
   //
   size_t size()  const 
   { 
      return container.size();   
   }
   bool empty() const 
   { 
      return container.empty();  
   }
   
private:

   void heapify();                            // convert the container in to a heap
   bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

   custom::vector<T> container; 

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
    if (container.size() > 0)
    {
        return container.front();
    }
    else
    {
        return T();
    }
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
    if (!container.empty())
    {
        std::swap(container[0], container[container.size() - 1]);
        container.pop_back();
        percolateDown(1);
    }
    
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue <T> :: push(const T & t)
{
    container.push_back(t); 
    //size_t i = (container.size() % 2 == 0) ? container.size() / 2 : container.size() / 2 - 1; 
    //if it doesn't work then try forcing I into an int, otherwise use the above
    size_t i = container.size() / 2;

    while (i && percolateDown(i)) 
    {
        i = i / 2;
    }
}
template <class T>
void priority_queue <T> :: push(T && t)
{
    container.push_back(t);
    //size_t i = (container.size() % 2 == 0) ? container.size() / 2 : container.size() / 2 - 1; 
    //if it doesn't work then try forcing I into an int, otherwise use the above
    size_t i = container.size() / 2;

    while (i && percolateDown(i))
    {
        i = i / 2;
    }
}

/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue <T> :: percolateDown(size_t indexHeap)
{
    
    size_t indexBigger = indexHeap; 
    size_t indexLeft = indexHeap * 2;
    size_t indexRight = indexLeft + 1;

    //case for checking both children
    if (indexLeft <= container.size() && !container.empty())
    {
        if (indexRight <= container.size()) 
        {
            if (container[indexLeft - 1] < container[indexRight - 1]) 
            {
                indexBigger = indexRight; 
            }
            else
            {
                indexBigger = indexLeft; 
            }
        }
        else
        {
            indexBigger = indexLeft;
        }
    }
    else
    {
        return false;
    }


    if (container[indexHeap - 1] < container[indexBigger - 1])  
    {
        std::swap(container[indexHeap - 1], container[indexBigger - 1]); 
        percolateDown(indexBigger);
        return true;
    }
}

/************************************************
 * P QUEUE :: HEAPIFY
 * Turn the container into a heap.
 ************************************************/
template <class T>
void priority_queue <T> ::heapify()
{
    //size_t i = (container.size() % 2 == 0) ? container.size() / 2 : container.size() / 2 - 1;
    //if it doesn't work then try forcing I into an int, otherwise use the above
    size_t i = container.size() / 2;

    for (i;  i > 0; i--)
    {
        percolateDown(i);
    }

}

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs)
{
    lhs.container.swap(rhs.container); 
}

};

