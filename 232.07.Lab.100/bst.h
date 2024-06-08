/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Emilio Ordonez, Austin Jesperson, Evan Riker
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

   template <class TT>
   class set;
   template <class KK, class VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set;

   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   // 
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   // 

   iterator erase(iterator& it);
   void   clear() noexcept;

   // 
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements; }
   

private:

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
   BNode* copyBNode(BNode* src, BNode* parent); // Helper Function to help copy nodes from one tree to the next
   void deleteBNode(BNode* node); // Helper function to help delete every node in the bst
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   // 
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}
   BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}
   BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(false) {}

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   // 
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pNode && pNode->pParent && pNode->pParent->pRight == pNode; }
   bool isLeftChild( BNode * pNode) const { return pNode && pNode->pParent && pNode->pParent->pLeft == pNode; }

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestMap;
   friend class ::TestSet;

   template <class KK, class VV>
   friend class map;

   template <class TT>
   friend class set; 
public:
   // constructors and assignment
   iterator(BNode* p = nullptr) : pNode(p) {}
   iterator(const iterator& rhs) : pNode(rhs.pNode) {}
   iterator & operator = (const iterator & rhs)
   {
       if (this != &rhs)
           pNode = rhs.pNode;
       return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
       return pNode == rhs.pNode;
   }
   bool operator != (const iterator & rhs) const
   {
       return pNode != rhs.pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const 
   {
       return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
       iterator temp = *this;
       ++(*this);
       return temp;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
       iterator temp = *this;
       --(*this);
       return temp;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:

    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  * Initialize a tree with no elements and a null root
  ********************************************/
template <typename T>
BST <T> ::BST() : root(nullptr), numElements(0) {}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) 
{
    // Initialize the new tree
    numElements = rhs.numElements;
    root = nullptr;

    // Use copyBNode function to recursively copy the nodes from the rhs
    if (rhs.root != nullptr)
    {
        root = copyBNode(rhs.root, nullptr);
    }
}

/********************************************
 * BST :: COPY BNODE
 * Recursively copy a node and its children
 ********************************************/
template <typename T>
typename BST<T>::BNode* BST<T>::copyBNode(BNode* src, BNode* parent)
{
    // If the src node is empty, there's nothing to copy. Recursive loop will stop.
    if (src == nullptr)
    {
        return nullptr;
    }

    // Create a new node with the same data as the src
    BNode* newNode = new BNode(src->data);
    newNode->pParent = parent;
    newNode->isRed = src->isRed;

    // Using recursion, copy the left and right children
    newNode->pLeft = copyBNode(src->pLeft, newNode);
    newNode->pRight = copyBNode(src->pRight, newNode);

    // Return the newNode, which is now the root of the copied BST
    return newNode;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) 
{
    // Transfer root and numElements from rhs to this BST
    root = rhs.root;
    numElements = rhs.numElements;

    // Clear rhs BST
    rhs.root = nullptr;
    rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/
template <typename T>
BST <T> ::BST(const std::initializer_list<T>& il) : root(nullptr), numElements(0)
{
    // Iterate through each item in the initializer list
    for (const T& element : il)
    {
        // If the root hasn't been initalized yet, intialize it with the first item from the list.
        if (root == nullptr)
        {
            root = new BNode(element);
        }
        else
        {
            // Initialize a pointer to the root
            BNode* currentNode = root;

            // Begin adding nodes to the tree
            while (currentNode != nullptr)
            {
                // A node should be added to the left if the current element in the list is less than the root
                if (element < currentNode->data)
                {
                    // Check if the current node has no node on the left
                    if (currentNode->pLeft == nullptr)
                    {
                        // Create and link a node to the left of the currentNode using the element from the il
                        currentNode->pLeft = new BNode(element);

                        // Link the curerntNode as the parent to the new node that was just created
                        currentNode->pLeft->pParent = currentNode;
                        break;
                    }
                    else
                    {
                        // Advance to the next node on the left
                        currentNode = currentNode->pLeft;
                    }
                }
                else // A node will be added to the right side of the bst
                {
                    // Check if the current node has no node to the right
                    if (currentNode->pRight == nullptr)
                    {
                        // Create and link a node to the right of the currentNode using the element from il
                        currentNode->pRight = new BNode(element);

                        // LInk the currentNode as the parent to the new node that was just created
                        currentNode->pRight->pParent = currentNode;
                        break;
                    }
                    else
                    {
                        // Advance to the next node on the right
                        currentNode = currentNode->pRight;
                    }
                }
            }
        }
        // Update the numElements count after a node is added to the bst
        numElements++;
    }
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
    // Use deleteBnode to delete all nodes
    deleteBNode(root);

    // Reset root and numElements to nullptr and 0
    root = nullptr;
    numElements = 0;
}

/**********************************************
 * BST :: DELETE BNODE
 * Recursively delete nodes in the bst
 **********************************************/
template <typename T>
void BST<T>::deleteBNode(BNode* node)
{
    // If the node to be deleted is already deleted, the recusive loop should stop
    if (node == nullptr)
        return;

    // Recursively delete the right and left children
    deleteBNode(node->pLeft);
    deleteBNode(node->pRight);

    // Delete the current node
    delete node;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   // Check for self-assignment
   if (this != &rhs)
   {
       
       clear();

       numElements = rhs.numElements;

       // Copy the bst from the rhs, as long as it is not null
       if (rhs.root != nullptr)
       {
           // Copy the nodes from the rhs using copyBNode
           root = copyBNode(rhs.root, nullptr);
       }
   }

   // Return the new bst
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
    // Clear the current contents of the tree
    clear();

    // Begin inserting each element from the il into the tree
    for (const T& element : il)
    {
        // Begin by inserting the root
        if (root == nullptr)
        {
            root = new BNode(element);
            ++numElements;
        }
        else
        {
            // Initialize pointers to current and parent nodes, starting at the root
            BNode* current = root;
            BNode* parent = nullptr;

            // Begin traversing the tree to find the right position for the element
            while (current != nullptr)
            {
                // Update the parent pointer
                parent = current;
                if (element < current->data)
                {
                    // Move to the left child
                    current = current->pLeft;
                }
                else
                {
                    // Move to the right child
                    current = current->pRight;
                }
            }

            // Insert the new node as the left or right child of the parent
            if (element > parent->data)
            {
                // Create a new right child node and link it
                parent->pRight = new BNode(element);
                parent->pRight->pParent = parent;
            }
            else
            {
                // Create a new left child node and link it
                parent->pLeft = new BNode(element);
                parent->pLeft->pParent = parent;
            }
            // Increment the number of elements after adding a node
            ++numElements;
        }
    }
    return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    // Check for self assignment
    if (this != &rhs)
    {
        // Clear the current contents of the tree
        clear();

        // Move resources from rhs to this
        root = rhs.root;
        numElements = rhs.numElements;

        // Leave rhs emtpy
        rhs.root = nullptr;
        rhs.numElements = 0;
    }
    return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
    // Swap the root pointers of the two trees
    std::swap(root, rhs.root);

    // Swap the numElements of the two trees
    std::swap(numElements, rhs.numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
    // Initialize the return value as a pair with end() and false]
    std::pair<iterator, bool> pairReturn(end(), false);

    //Initialize flags for inserting left or right
    bool insertRight = false;
    bool insertLeft = false;

    // If the tree is empty, create a new root node
    if (root == nullptr)
    {
        root = new BNode(t);
        ++numElements;

        // Update the return pair
        pairReturn.first = iterator(root);
        pairReturn.second = true;
        return pairReturn;
    }

    // Start at the root and traverse the tree to find the insertion point
    BNode* current = root;
    BNode* parent = nullptr;
    while (current != nullptr)
    {
        parent = current;
        if (t < current->data)
        {
            current = current->pLeft;
            if (insertRight)
            {
                insertRight = false;
            }
            insertLeft = true;
        }
        else if (t > current->data)
        {
            current = current->pRight;
            if (insertLeft)
            {
                insertLeft = false;
            }
            insertRight = true;
        }
        else
        {
            // If duplicates are allowed and and a node with the same value is found
            if (keepUnique)
            {
                // Update the return pair
                pairReturn.first = iterator(current);
                return pairReturn;
            }
            else {

                current = current->pRight; // Duplicates are moved to the right
                if (insertLeft)
                {
                    insertLeft = false;
                }
                insertRight = true;
            }
        }
    }

    // Create a new node and attach it to the parent
    BNode* newNode = new BNode(t);

    if (insertLeft)
    {
        parent->pLeft = newNode;
    }
    else {
        parent->pRight = newNode;
    }

    newNode->pParent = parent;

    ++numElements;
    pairReturn.first = iterator(newNode);
    pairReturn.second = true;
    return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
    // Initialize the return value as a pair with end() and false
    std::pair<iterator, bool> pairReturn(end(), false);

    // If the tree is empty, create a new root node
    if (root == nullptr) {
        root = new BNode(std::move(t));
        ++numElements;
        pairReturn.first = iterator(root);
        pairReturn.second = true;
        return pairReturn;
    }

    // Comparator object
    std::less<T> less;

    // Start at the root and traverse the tree to find the insertion point
    BNode* current = root;
    BNode* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (less(t, current->data)) { // Use std::less for comparison
            current = current->pLeft;
        }
        else if (less(current->data, t)) { // Use std::less for comparison
            current = current->pRight;
        }
        else {
            // If duplicates are not allowed and we find a node with the same value
            if (keepUnique) {
                pairReturn.first = iterator(current);
                return pairReturn;
            }
            else {
                current = current->pRight; // For duplicates, move to the right
            }
        }
    }

    // Create a new node and attach it to the parent
    BNode* newNode = new BNode(std::move(t));
    if (less(t, parent->data)) {
        parent->pLeft = newNode;
    }
    else {
        parent->pRight = newNode;
    }
    newNode->pParent = parent;

    ++numElements;
    pairReturn.first = iterator(newNode);
    pairReturn.second = true;
    return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{  
    return end();
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
    // Use deleteBNode to delete all nodes from the bst
    deleteBNode(root);

    // Reset root and num elements to nullptr and 0
    root = nullptr;
    numElements = 0;

}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
    // Initialize a pointer to the root
    BNode* current = root;

    // Traverse the tree to get to the left most node
    while (current && current->pLeft != nullptr)
    {
        current = current->pLeft;
    }

    // Return an iterator pointing to the left most node
    return iterator(current);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
    // Initialize a pointer to the root
    BNode* current = root;

    // Traverse through the tree to find the node with the value t
    while (current != nullptr)
    {
        // If the current node has the value t, return an iterator to it
        if (t == current->data)
        {
            return iterator(current);
        }
        else if (t < current->data) // If t is less than the current node's value, move to the left child 
        {
            current = current->pLeft;
        }
        else
        {
            current = current->pRight; // If t is greater than the current node's value, move to the right child
        }
    }

    // if the value is not found, return an iterator to nulllptr
    return iterator(nullptr);
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    if (pNode) {
        pNode->pParent = this;
    }
    pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    if (pNode) {
        pNode->pParent = this;
    }
    pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    BNode* pNode = new BNode(t);
    addLeft(pNode);
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    BNode* pNode = new BNode(std::move(t));
    addLeft(pNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    BNode* pNode = new BNode(t);
    addRight(pNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    BNode* pNode = new BNode(std::move(t));
    addRight(pNode);
}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (!pNode)
        return *this;

    if (pNode->pRight) {
        pNode = pNode->pRight;
        while (pNode->pLeft)
            pNode = pNode->pLeft;
    }
    else {
        BNode* pParent = pNode->pParent;
        while (pParent && pNode == pParent->pRight) {
            pNode = pParent;
            pParent = pParent->pParent;
        }
        pNode = pParent;
    }
    return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
    if (!pNode)
        return *this;

    if (pNode->pLeft) {
        pNode = pNode->pLeft;
        while (pNode->pRight)
            pNode = pNode->pRight;
    }
    else {
        BNode* pParent = pNode->pParent;
        while (pParent && pNode == pParent->pLeft) {
            pNode = pParent;
            pParent = pParent->pParent;
        }
        pNode = pParent;
    }
    return *this;
}


} // namespace custom

