/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Emilio Ordonez, Austin Jesperson, Evan Riker
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
#include <stdexcept>

class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

    /**************************************************
     * LIST
     * Just like std::list
     **************************************************/
    template <typename T>
    class list
    {
        friend class ::TestList; // give unit tests access to the privates
        friend class ::TestHash;
        friend void swap(list& lhs, list& rhs);
    public:
        // 
        // Construct
        //

        list();
        list(list <T>& rhs);
        list(list <T>&& rhs);
        list(size_t num, const T& t);
        list(size_t num);
        list(const std::initializer_list<T>& il);
        template <class Iterator>
        list(Iterator first, Iterator last);
        ~list()
        {
            Node* current = pHead;

            while (current != nullptr)
            {
                Node* next = current->pNext;
                delete current;
                current = next;
            }

            pHead = nullptr;
            pTail = nullptr;
            numElements = 0;
        }

        // 
        // Assign
        //

        list <T>& operator = (list& rhs);
        list <T>& operator = (list&& rhs);
        list <T>& operator = (const std::initializer_list<T>& il);
        void swap(list <T>& rhs);

        //
        // Iterator
        //

        class  iterator;
        iterator begin() { return iterator(pHead); }
        iterator rbegin() { return iterator(pTail); }
        iterator end() 
        { 
            if (pTail)
            {
                return iterator(pTail->pNext);
            }
            return iterator(pTail); 
        }

        //
        // Access
        //

        T& front();
        T& back();

        //
        // Insert
        //

        void push_front(const T& data);
        void push_front(T&& data);
        void push_back(const T& data);
        void push_back(T&& data);
        iterator insert(iterator it, const T& data);
        iterator insert(iterator it, T&& data);

        //
        // Remove
        //

        void pop_back();
        void pop_front();
        void clear();
        iterator erase(const iterator& it);

        // 
        // Status
        //

        bool empty()  const { return numElements == 0; }
        size_t size() const { return numElements; }


    private:
        // nested linked list class
        class Node;

        // member variables
        size_t numElements; // though we could count, it is faster to keep a variable
        Node* pHead;    // pointer to the beginning of the list
        Node* pTail;    // pointer to the ending of the list
    };

    /*************************************************
     * NODE
     * the node class.  Since we do not validate any
     * of the setters, there is no point in making them
     * private.  This is the case because only the
     * List class can make validation decisions
     *************************************************/
    template <typename T>
    class list <T> ::Node
    {
    public:
        //
        // Construct
        //
        Node()
        {
            pNext = pPrev = nullptr;
        }
        Node(const T& data)
        {
            pNext = pPrev = nullptr;
            this->data = data;
        }
        Node(T&& data)
        {
            pNext = pPrev = nullptr;
            this->data = data;
        }

        //
        // Data
        //

        T data;                 // user data
        Node* pNext;       // pointer to next node
        Node* pPrev;       // pointer to previous node
    };

    /*************************************************
     * LIST ITERATOR
     * Iterate through a List, non-constant version
     ************************************************/
    template <typename T>
    class list <T> ::iterator
    {
        friend class ::TestList; // give unit tests access to the privates
        friend class ::TestHash;
        template <typename TT>
        friend class custom::list;
    public:
        // constructors, destructors, and assignment operator
        iterator() : p(nullptr) {}
        iterator(Node* p) : p(p) {}
        iterator(const iterator& rhs) : p(rhs.p) {}
        iterator& operator = (const iterator& rhs)
        {
            p = rhs.p;
            return *this;
        }

        // equals, not equals operator
        bool operator == (const iterator& rhs) const { return p == rhs.p; }
        bool operator != (const iterator& rhs) const { return p != rhs.p; }

        // dereference operator, fetch a node
        T& operator * ()
        {
            return p->data;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator tmp = *this;
            if (p)
                p = p->pNext;
            return tmp;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            if (p)
                p = p->pNext;
            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator tmp = *this;
            if (p)
                p = p->pPrev;
            return tmp;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            if (p)
                p = p->pPrev;
            return *this;
        }

        // two friends who need to access p directly
        friend iterator list <T> ::insert(iterator it, const T& data);
        friend iterator list <T> ::insert(iterator it, T&& data);
        friend iterator list <T> ::erase(const iterator& it);

    private:

        typename list <T> ::Node* p;
    };

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T>
    list <T> ::list(size_t num, const T& t) : numElements(num), pHead(nullptr), pTail(nullptr)
    {
        if (num == 0)
            return;

        pHead = new Node(t);
        Node* current = pHead;

        for (size_t i = 1; i < num; ++i)
        {
            current->pNext = new Node(t);
            current->pNext->pPrev = current;
            current = current->pNext;
        }

        pTail = current;
    }

    /*****************************************
     * LIST :: ITERATOR constructors
     * Create a list initialized to a set of values
     ****************************************/
    template <typename T>
    template <class Iterator>
    list<T>::list(Iterator first, Iterator last) : numElements(0), pHead(nullptr), pTail(nullptr)
    {
        if (first == last)
            return;

        pHead = new Node(*first);
        Node* current = pHead;
        ++first;
        ++numElements;

        while (first != last)
        {
            current->pNext = new Node(*first);
            current->pNext->pPrev = current;
            current = current->pNext;
            ++first;
            ++numElements;
        }

        pTail = current;
    }


    /*****************************************
     * LIST :: INITIALIZER constructors
     * Create a list initialized to a set of values
     ****************************************/
    template <typename T>
    list <T> ::list(const std::initializer_list<T>& il)
    {
        if (il.size() == 0)
            return;

        auto it = il.begin();
        pHead = new Node(*it);
        Node* current = pHead;
        ++it;
        ++numElements;

        for (; it != il.end(); ++it)
        {

            current->pNext = new Node(*it);
            current->pNext->pPrev = current;
            current = current->pNext;
            ++numElements;
        }

        pTail = current;
    }

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T>
    list <T> ::list(size_t num) : numElements(num), pHead(nullptr), pTail(nullptr)
    {
        if (num == 0)
            return;

        pHead = new Node(T());
        Node* current = pHead;

        for (size_t i = 1; i < num; ++i)
        {
            current->pNext = new Node(T());
            current->pNext->pPrev = current;
            current = current->pNext;
        }

        pTail = current;
    }

    /*****************************************
     * LIST :: DEFAULT constructors
     ****************************************/
    template <typename T>
    list <T> ::list()
    {
        pHead = pTail = nullptr;
        numElements = 0;
    }

    /*****************************************
     * LIST :: COPY constructors
     ****************************************/
    template <typename T>
    list <T> ::list(list& rhs)
    {
        if (rhs.pHead == nullptr)
            return;

        pHead = new Node(rhs.pHead->data);
        Node* current = pHead;
        Node* rhsCurrent = rhs.pHead->pNext;

        while (rhsCurrent != nullptr)
        {
            current->pNext = new Node(rhsCurrent->data);
            current->pNext->pPrev = current;
            current = current->pNext;
            rhsCurrent = rhsCurrent->pNext;
        }

        pTail = current;
        numElements = rhs.numElements;
    }

    /*****************************************
     * LIST :: MOVE constructors
     * Steal the values from the RHS
     ****************************************/
    template <typename T>
    list <T> ::list(list <T>&& rhs)
    {
        numElements = rhs.numElements;
        pHead = rhs.pHead;
        pTail = rhs.pTail;
        rhs.pHead = rhs.pTail = nullptr;
        rhs.numElements = 0;
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS 
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (list <T>&& rhs)
    {
        // Self-assignment check
        if (this == &rhs)
            return *this;

        // Clear the current list
        clear();

        // Transfer ownership of resources from rhs to this
        pHead = rhs.pHead;
        pTail = rhs.pTail;
        numElements = rhs.numElements;

        // Reset rhs
        rhs.pHead = nullptr;
        rhs.pTail = nullptr;
        rhs.numElements = 0;

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (list <T>& rhs)
    {
        if (rhs.empty())
        {
            this->clear();
            return *this;
        }

        if (!this->empty())
        {
            this->clear();
        }
        
        Node* currentNode = rhs.pHead;
        pHead = currentNode;

        while (currentNode) {
            push_back(currentNode->data);
            currentNode = currentNode->pNext;
        }

        numElements = rhs.numElements;
        return *this;
    }

    /**********************************************
     * LIST :: assignment operator
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
    {
        if (rhs.size() == 0)
        {
            this->clear();
            return *this;
        }

        if (!this->empty())
        {
            this->clear();
        }

        auto it = rhs.begin();
        pHead = new Node(*it);
        Node* current = pHead;
        ++it;

        for (; it != rhs.end(); ++it)
        {

            current->pNext = new Node(*it);
            current->pNext->pPrev = current;
            current = current->pNext;
        }

        pTail = current;
        numElements = rhs.size();
        return *this;
    }

    /**********************************************
     * LIST :: CLEAR
     * Remove all the items currently in the linked list
     *     INPUT  :
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    void list <T> ::clear()
    {
        // Stragagy: loop though all the nodes, grab the next one and delete the current one.
        Node* currentNode = pHead;

        Node* TempNode;

        while (currentNode) {
            // Save the node behind currentNode so we can delete 
            // it after we advance current node.
            TempNode = currentNode;

            // advance currentNode to the next one in the list.
            currentNode = currentNode->pNext;

            // delete that node we saved.
            delete TempNode;
        }

        // reset the head and tail.
        numElements = 0;
        pHead = nullptr;
        pTail = nullptr;
    }

    /*********************************************
     * LIST :: PUSH BACK
     * add an item to the end of the list
     *    INPUT  : data to be added to the list
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::push_back(const T& data)
    {

        // create the new node of data
        Node* newNode = new Node(data);

        // if the list is currently empty, add the new data as the only node.
        if (numElements == 0) {
            pHead = newNode;
            pTail = newNode;

            newNode->pPrev = nullptr;
            newNode->pNext = nullptr;
        }

        // otherwise, insert it at the tail. 
        else {
            // link the old tail to the new node
            pTail->pNext = newNode;

            // link the new data to the old head
            newNode->pPrev = pTail;

            // make sure the new node has nothing before it.
            newNode->pNext = nullptr;

            // change the head.
            pTail = newNode;
        }

        // add an element to the number of elements.
        numElements++;

       
    }

    template <typename T>
    void list <T> ::push_back(T&& data)
    {
        // create the new node of data
        Node* newNode = new Node(data);

        // if the list is currently empty, add the new data as the only node.
        if (numElements == 0) {
            pHead = newNode;
            pTail = newNode;

            newNode->pPrev = nullptr;
            newNode->pNext = nullptr;
        }

        // otherwise, insert it at the tail. 
        else {
            // link the old tail to the new node
            pTail->pNext = newNode;

            // link the new data to the old head
            newNode->pPrev = pTail;

            // make sure the new node has nothing before it.
            newNode->pNext = nullptr;

            // change the head.
            pTail = newNode;
        }

        // add an element to the number of elements.
        numElements++;
    }

    /*********************************************
     * LIST :: PUSH FRONT
     * add an item to the head of the list
     *     INPUT  : data to be added to the list
     *     OUTPUT :
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::push_front(const T& data)
    {
        
        // create the new node of data
        Node* newNode = new Node(data);

        // if the list is currently empty, add the new data as the only node.
        if (numElements == 0) {
            pHead = newNode;
            pTail = newNode;

            newNode->pPrev = nullptr;
            newNode->pNext = nullptr;
        }

        // otherwise, insert it at the head. 
        else {
            // link the old head to the new node
            pHead->pPrev = newNode;

            // link the new data to the old head
            newNode->pNext = pHead;

            // make sure the new node has nothing before it.
            newNode->pPrev = nullptr;

            // change the head.
            pHead = newNode;
        }
        
        // add an element to the number of elements.
        numElements++;

    }

    template <typename T>
    void list <T> ::push_front(T&& data)
    {
        // create the new node of data
        Node* newNode = new Node(data);

        // if the list is currently empty, add the new data as the only node.
        if (numElements == 0) {
            pHead = newNode;
            pTail = newNode;

            newNode->pPrev = nullptr;
            newNode->pNext = nullptr;
        }

        // otherwise, insert it at the head. 
        else {
            // link the old head to the new node
            pHead->pPrev = newNode;

            // link the new data to the old head
            newNode->pNext = pHead;

            // make sure the new node has nothing before it.
            newNode->pPrev = nullptr;

            // change the head.
            pHead = newNode;
        }

        // add an element to the number of elements.
        numElements++;
    }


    /*********************************************
     * LIST :: POP BACK
     * remove an item from the end of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::pop_back()
    {
        // only do this if the list isnt alrady empty
        if (numElements != 0) {
            // save a pointer to the old tail so we can delete it later.
            Node* oldTail = pTail;

            // if there is only one element, just delete it, no need to meigntaign connections.
            if (numElements == 1) {
                pTail = nullptr;
                pHead = nullptr;
            }
            else {
                // change the new Tail
                pTail = pTail->pPrev;

                // remove the new Tail referance to the old Tail.
                pTail->pNext = nullptr;
            }

            // delete the old Tail.
            delete oldTail;

            // count down our number of elements.
            numElements = numElements - 1;
        }

        
    }

    /*********************************************
     * LIST :: POP FRONT
     * remove an item from the front of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::pop_front()
    {

        // only do this if the list isnt alrady empty
        if (numElements != 0) {
            // save a pointer to the old head so we can delete it later.
            Node* oldHead = pHead;

            // if there is only one element, just delete it, no need to meigntaign connections.
            if (numElements == 1){
                pTail = nullptr;
                pHead = nullptr;
            }
            else {
                // change the new Head
                pHead = pHead->pNext;

                // remove the new Head referance to the old Head.
                pHead->pPrev = nullptr;
            }

            // delete the old Head.
            delete oldHead;

            // count down our number of elements.
            numElements = numElements - 1;
        }
    }

    /*********************************************
     * LIST :: FRONT
     * retrieves the first element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    T& list <T> ::front()
    {
        if (!pHead) {
            static T default_instance{};
            return default_instance;
        }
        else {
            return pHead->data;
        }
        
    }

    /*********************************************
     * LIST :: BACK
     * retrieves the last element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    T& list <T> ::back()
    {
        if (!pTail) {
            static T default_instance{};
            return default_instance;
        }
        else {
            return pTail->data;
        }
        
    }

    /******************************************
     * LIST :: REMOVE
     * remove an item from the middle of the list
     *     INPUT  : an iterator to the item being removed
     *     OUTPUT : iterator to the new location
     *     COST   : O(1)
     ******************************************/
    template <typename T>
    typename list <T> ::iterator  list <T> ::erase(const list <T> ::iterator& it)
    {
        // Check if the iterator is valid
        if (!it.p)
            return it;
             
        // Get the node to erase
        Node* nodeToDelete = it.p;
        // Create an iterator to the next node
        Node* nodeToReturn = nodeToDelete->pNext;


        // Update pointers
        if (nodeToDelete->pPrev)
            nodeToDelete->pPrev->pNext = nodeToDelete->pNext;
        else
        {
            pHead = nodeToDelete->pNext; // Node is head
        }
        if (nodeToDelete->pNext)
            nodeToDelete->pNext->pPrev = nodeToDelete->pPrev;
        else
        {
            pTail = nodeToDelete->pPrev; // Node is tail
        }

        
        // Delete the node
        delete nodeToDelete;
        --numElements;

        return iterator(nodeToReturn);
    }

    /******************************************
     * LIST :: INSERT
     * add an item to the middle of the list
     *     INPUT  : data to be added to the list
     *              an iterator to the location where it is to be inserted
     *     OUTPUT : iterator to the new item
     *     COST   : O(1)
     ******************************************/
    template <typename T>
    typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
        const T& data)
    {
        // we start by creating a new node with the data

        Node* newNode = new Node(data);

        //set up the nodes before and after where we want the new node to be
        Node* targetNode = it.p;
        Node* previousNode;
        

        // Update pointers
        if (targetNode)
        {
            if (targetNode->pPrev)
            {
                //set previous
                previousNode = targetNode->pPrev;

                //set nodes before and after to point to the newNode
                previousNode->pNext = newNode;
                targetNode->pPrev = newNode;

                //set newNode to point to the ones before and after
                newNode->pPrev = previousNode;
                newNode->pNext = targetNode;
            }
            else
            {
                newNode->pNext = pHead;
                pHead->pPrev = newNode;
                pHead = newNode; // Node is head
            }
        }

        else
        {
            //nullptr with elements in the list defaults to the end
            if (numElements > 0)
            {
                pTail->pNext = newNode;
                newNode->pPrev = pTail;
                pTail = newNode;
            }
            //if given an empty list this becomes the only node
            else
            {
                pHead = newNode;
                pTail = newNode;
            }
            
        }

        numElements++;

        return iterator(newNode);
    }

    template <typename T>
    typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
        T&& data)
    {
        // we start by creating a new node with the data

        Node* newNode = new Node(data);

        Node* targetNode = it.p;
        Node* previousNode;


        // Update pointers
        if (targetNode)
        {
            if (targetNode->pPrev)
            {
                //set previous
                previousNode = targetNode->pPrev;

                //set nodes before and after to point to the newNode
                previousNode->pNext = newNode;
                targetNode->pPrev = newNode;

                //set newNode to point to the ones before and after
                newNode->pPrev = previousNode;
                newNode->pNext = targetNode;
            }
            else
            {
                newNode->pNext = pHead;
                pHead->pPrev = newNode;
                pHead = newNode; // Node is head
            }
        }

        else
        {
            //nullptr with elements in the list defaults to the end
            if (numElements > 0)
            {
                pTail->pNext = newNode;
                newNode->pPrev = pTail;
                pTail = newNode;
            }
            //if given an empty list
            else
            {
                pHead = newNode;
                pTail = newNode;
            }

        }

        numElements++;

        return iterator(newNode);
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/
    template <typename T>
    void swap(list <T>& lhs, list <T>& rhs)
    {
       // no need to rewrite this code.
        lhs.swap(rhs);
    }

    template <typename T>
    void list<T>::swap(list <T>& rhs)
    {
        //create placeholders
        Node* tempHead = rhs.pHead;
        Node* tempTail = rhs.pTail; 
        size_t tempElem = rhs.numElements;

        //move left to right
        rhs.pHead = this->pHead;
        rhs.pTail = this->pTail;
        rhs.numElements = this->numElements;

        //move temp to left
        this->pHead = tempHead;
        this->pTail = tempTail;
        this->numElements = tempElem;
}

    //#endif
}; // namespace custom
