/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Emilio Ordonez, Austin Jesperson, Evan Riker
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:

   //
   // Construct
   //

    Node() : pNext(nullptr), pPrev(nullptr) {};
    Node(const T& data) : data(data), pNext(nullptr), pPrev(nullptr) {}
    Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

   //
   // Member variables
   //

   T data;                 // user data
   Node <T>* pNext;       // pointer to next node
   Node <T>* pPrev;        // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node<T>* copy(const Node<T>* pSource)
{
    // if the sorce is null, return null
    if (!pSource){
        return nullptr;
    }

    // store a copy of the sorce thats const to save memory
    const Node<T>* currentSource = pSource;

    // move the data from the copy of sorce to new list.
    Node<T>* newList = new Node<T>(currentSource->data);

    // make a copy of new list
    Node<T>* currentNew = newList;
    
    // move to the next node... not sure why you can update current sorce
    // as its const...
    currentSource = currentSource->pNext;

    // loop until you go beyond the last node in the list
    while (currentSource != nullptr)
    {
        // make a copy of our itorator
        Node<T>* newNode = new Node<T>(*currentSource);

        // update the next/prev value for the newList copy.
        // on the first loop, currentNew is a copy of PSource,
        // and currentSource and newNode are a copy is the next node after pSource
        
        // the next two lines connect newNode and currentNew with newNode at i+1
        currentNew->pNext = newNode;
        newNode->pPrev = currentNew;

        // we advance our current new to the next node in the line 
        // so we can configure its next nodes.
        currentNew = currentNew->pNext;

        // then we advance our itorator.
        currentSource = currentSource->pNext;
    }

    return newList;
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node<T>*& pDestination, const Node<T>* pSource)
{
    Node<T>* currentDest = pDestination;
    Node<T>* prevDest = nullptr;
    const Node<T>* currentSource = pSource;

    while (currentSource)
    {
        if (currentDest)
        {
            currentDest->data = currentSource->data;
            prevDest = currentDest;
            currentDest = currentDest->pNext;
        }
        else
        {
            Node<T>* newNode = new Node<T>(currentSource->data);
            if (prevDest)
                prevDest->pNext = newNode;
            else
                pDestination = newNode;
            prevDest = newNode;
        }
        currentSource = currentSource->pNext;
    }

    while (currentDest)
    {
        Node<T>* temp = currentDest;
        currentDest = currentDest->pNext;
        delete temp;
    }

    if (prevDest)
        prevDest->pNext = nullptr;
}
/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
    std::swap(pLHS, pRHS);
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node<T>* remove(const Node<T>* pRemove)
{
    if (!pRemove) return nullptr;  // If the node to be removed is null, return null

    Node<T>* parent = pRemove->pPrev;  // Get the parent node of pRemove

    if (parent)  // If parent exists
    {
        parent->pNext = pRemove->pNext;  // Update parent's next pointer
    }
    else
    {
        parent = pRemove->pNext;
        parent->pPrev = nullptr;
        delete pRemove;
        return parent;
    }

    if (pRemove->pNext)  // If pRemove has a next node
    {
        pRemove->pNext->pPrev = parent;  // Update next node's previous pointer
    }

    delete pRemove;  // Delete the node to be removed

    return parent;  // Return the pointer to the parent node
}

/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
    Node<T>* pNew = new Node<T>(t);

    if (!pCurrent) return pNew;

    if (after) {
        pNew->pNext = pCurrent->pNext;
        pNew->pPrev = pCurrent;
        if (pCurrent->pNext)
            pCurrent->pNext->pPrev = pNew;
        pCurrent->pNext = pNew;
    }
    else {
        pNew->pPrev = pCurrent->pPrev;
        pNew->pNext = pCurrent;
        if (pCurrent->pPrev)
            pCurrent->pPrev->pNext = pNew;
        pCurrent->pPrev = pNew;
    }

    return pNew;
}

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T>* pHead)
{
    size_t count = 0;  
    const Node<T>* iterator = pHead;  

    while (iterator != nullptr)
    {
        count++; 
        iterator = iterator->pNext; 
    }

    return count; 
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
    const Node<T>* pCurrent = pHead;
    while (pCurrent) {
        out << pCurrent->data << ' ';
        pCurrent = pCurrent->pNext;
    }
    return out;
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T>*& pHead)
{
    Node<T>* current = pHead;

    while (current != nullptr)
    {
        Node<T>* next = current->pNext;

        delete current;

        current = next;
    }

    pHead = nullptr;
}


