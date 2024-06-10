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
    iterator(BNode* p = nullptr)
    {
        pNode = p;
    }
    iterator(const iterator& rhs)
    {
        pNode = rhs.pNode;
    }
    iterator& operator = (const iterator& rhs)
    {
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

   BNode* getNode()
   {
       return pNode;
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

    //create a temperary new node
    BNode* newNode = nullptr;

    //make sure that parent isn't null
    if (parent)
    {
        //if src is a left child then set the new node to the left child of parent
        if ((src->pParent)->isLeftChild(src))
        {
            newNode = parent->pLeft;
        }
        //otherwise must be right child
        else
        {
            newNode = parent->pRight;
        }
    }
    

    // if new node is still null then make it a new node with the same data as the src
    if (!newNode)  
    {
        newNode = new BNode(src->data);   
    }
    //otherwise copy the data over
    else
    {
        newNode->data = src->data;
    }
    
    //attach to parent
    newNode->pParent = parent;

    //establish logic to ensure deletion of extraneous nodes if *this is bigger than src
    if (!src->pLeft && newNode->pLeft) 
    {
        deleteBNode(newNode->pLeft);  
    }
    if (!src->pRight && newNode->pLeft)  
    {
        deleteBNode(newNode->pRight);  
    }

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
    if (!node)
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
       //if both roots are allocated then copy the data over
       if (root && rhs.root)
       {
           root->data = rhs.root->data;
       }
       else
       {
           //this catches an empty rhs bst and clears *this
           if (!rhs.root)
           {
               clear();
               return *this;
           }
           //if we get here then *this is empty and a new root must be allocated
           else
           {
               root = new BNode(rhs.root->data);
           }
       }

       //check if rhs has children and delete nodes from *this if necessary
       if (!rhs.root->pLeft && root->pLeft)
       {
           deleteBNode(root->pLeft);
       }
       if (!rhs.root->pRight && root->pRight)
       {
           deleteBNode(root->pRight);
       }

       //start the copy recursion
       root->pLeft = copyBNode(rhs.root->pLeft, root); 
       root->pRight = copyBNode(rhs.root->pRight, root); 

       //update the numElements not the the data is updated
       numElements = rhs.numElements;
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

    // Initialize flags for insertion direction
    bool insertLeft = false;
    bool insertRight = false;

    // Begin inserting each element from the initializer list into the tree
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
            // Initialize pointers to current node starting at the root
            BNode* current = root;
            BNode* parent = nullptr;

            // Begin traversing the tree to find the right position for the element
            while (current != nullptr)
            {
                parent = current;
                if (element < current->data)
                {
                    // Move to the left child
                    current = current->pLeft;
                    
                    // Update flags
                    if (insertRight)
                        insertRight = false;
                    insertLeft = true;
                }
                else // element >= current->data
                {
                    // Move to the right child
                    current = current->pRight;

                    // Update flags
                    if (insertLeft)
                        insertLeft = false;
                    insertRight = true;
                }
            }

            // Determine whether to insert the new node as the left or right child
            BNode* newNode = new BNode(element);
            if (insertLeft)
            {
                parent->pLeft = newNode;
            }
            else
            {
                parent->pRight = newNode;
            }
            newNode->pParent = parent;
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
    // Initialize the return value as a pair with end() and false
    std::pair<iterator, bool> pairReturn(end(), false);

    // Initialize flags for inserting left or right
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

    // If duplicates are allowed
    if (keepUnique == false)
    {
        // Begin traversing through the tree to find insertion point
        while (current != nullptr)
        {
            parent = current;

            // Determine if data should go on the left side of the current node
            if (t < current->data)
            {
                // Advance to the left node
                current = current->pLeft;

                // Update flags
                insertLeft = true;
                insertRight = false;
            }
            else
            {
                // Advance to the right node (including duplicates)
                current = current->pRight;

                // Update flags
                insertRight = true;
                insertLeft = false;
            }
        }
    }
    else // duplicates are not allowed
    {
        // Begin traversing the tree to find the insertion point
        while (current != nullptr)
        {
            parent = current;

            // Determine if t is equal to the data inside the current node
            if (t == current->data)
            {
                // Since duplicates are not allowed, simpl return the current node
                pairReturn.first = iterator(current);
                return pairReturn;
            }
            else if (t < current->data) // Determine if t is less than the data inside the current node
            {
                // Advance to the next node on the left
                current = current->pLeft;

                // Update flags
                insertLeft = true;
                insertRight = false;
            }
            else // t > current->data is now handled by default
            {
                // Advance to the next node on the right
                current = current->pRight;

                // Update flags
                insertRight = true;
                insertLeft = false;
            }
        }
    }

    // Create a new node
    BNode* newNode = new BNode(t);

    // Link it to the parent using the flags
    if (insertLeft)
    {
        parent->pLeft = newNode;
    }
    else
    {
        parent->pRight = newNode;
    }

    // Link the new node to the parent
    newNode->pParent = parent;

    // Increase the numElements by 1 and return the newNode
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

    // Initialize flags for inserting left or right
    bool insertRight = false;
    bool insertLeft = false;

    // If the tree is empty, create a new root node
    if (root == nullptr) {
        root = new BNode(std::move(t));
        ++numElements;
        pairReturn.first = iterator(root);
        pairReturn.second = true;
        return pairReturn;
    }

    // Start at the root and traverse the tree to find the insertion point
    BNode* current = root;
    BNode* parent = nullptr;

    // If duplicates are allowed
    if (!keepUnique)
    {
        // Begin traversing the tree to find the insertion point
        while (current != nullptr)
        {
            parent = current;

            // Determine which side of current t goes on
            if (t < current->data)
            {
                // Advance to the next node on the left
                current = current->pLeft;

                // Update flags
                if (insertRight)
                    insertRight = false;
                insertLeft = true;
            }
            else
            {
                // Advance to the next node on the right (including duplicates)
                current = current->pRight;

                // Update flags
                if (insertLeft)
                    insertLeft = false;
                insertRight = true;
            }
        }
    }
    else // Duplicates are not allowed
    {
        // Begin traversing through the tree
        while (current != nullptr)
        {
            parent = current;

            // Determine if t is equal to the current node's data
            if (t == current->data) 
            {
                // Since duplicates are not allowed, no new node is added simple return the current node
                pairReturn.first = iterator(current);
                return pairReturn;
            }
            else if (t < current->data)  // Determine which side of current t goes on
            {
                // Advance to the node on the left
                current = current->pLeft;

                // Update flags
                if (insertRight)
                    insertRight = false;
                insertLeft = true;
            }
            else
            {
                // Advance to the node on the right
                current = current->pRight;

                // Update flags
                if (insertLeft)
                    insertLeft = false;
                insertRight = true;
            }
        }
    }

    // Create a new node and attach it to the parent
    BNode* newNode = new BNode(std::move(t));

    // Use flags to determine which side of the parent the newNode must be linked to
    if (insertLeft) {
        parent->pLeft = newNode;
    }
    else {
        parent->pRight = newNode;
    }

    // Link the newNode to the parent
    newNode->pParent = parent;

    // Increase the numElements by 1 and return the newNode
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
    // If tree is emtpy or the iterator is empty, return an iterator pointing to a nulllptr
    if (numElements == 0 || it.getNode() == nullptr)
    {
        return end();
    }

    // Initialize pointers pointing to the node to get deleted and its parent
    BNode* nodeToDelete = it.getNode();
    BNode* parent = nodeToDelete->pParent;

    // If the node to delete is a leaf node, simply delete the node
    if (nodeToDelete->pLeft == nullptr && nodeToDelete->pRight == nullptr)
    {
        // nodeToDelete is on the right side of its parent
        if (nodeToDelete->isRightChild(nodeToDelete))
        {
            // Update the parent's right side
            parent->pRight = nullptr;
        }
        else // nodeToDelete is on the left side of its parent
        {
            // Update the parent's left side
            parent->pLeft = nullptr;
        }

        // Delete nodeToDelete, decrease the numElements by 1, and return the parent
        delete nodeToDelete;
        --numElements;
        return iterator(parent);
    }
    else if (nodeToDelete->pLeft != nullptr && nodeToDelete->pRight != nullptr) // Node to delete has children on both sides
    {
        // First traverse through the tree starting at the nodeToDelete to find the in-order successor
        BNode* successor = nodeToDelete->pRight;
        while (successor->pLeft != nullptr)
        {
            successor = successor->pLeft;
        }

        // Determine which side nodeToDelete is on
        if (nodeToDelete->isLeftChild(nodeToDelete))
        {
            // Link the successor to the parent's left side
            nodeToDelete->pParent->pLeft = successor;
        }
        else
        {
            // Link the successor to the parent's right side
            nodeToDelete->pParent->pRight = successor;
        }

        // Link the successor to nodeToDelete's parent and left child
        successor->pParent = nodeToDelete->pParent;
        successor->pLeft = nodeToDelete->pLeft;

        // Link nodeToDelete's left child back to the successor
        successor->pLeft->pParent = successor;

        // Determine if the successor has children on the right
        if (successor->pRight != nullptr)
        {
            // Link successor child with nodeToDelete's right child and vice versa
            nodeToDelete->pRight->pLeft = successor->pRight;
            successor->pRight->pParent = nodeToDelete->pRight;
        }

        // Link the successor to nodeToDelete's right child
        if (successor != nodeToDelete->pRight)
        {
            successor->pRight = nodeToDelete->pRight;

            // Link nodeToDelete's right child back to the successor
            nodeToDelete->pRight->pParent = successor;
        }


        // Delete the node and decrease numElements by 1
        delete nodeToDelete;
        --numElements;

        // Return the successor
        return iterator(successor);
    }
    else // Node to delete has one child
    {
        iterator nextNode = it;
        ++nextNode;

        // Node has its child on the right side
        if (nodeToDelete->pRight != nullptr)
        {
            // Determine if nodeToDelete is right child
            if (nodeToDelete->isRightChild(nodeToDelete))
            {
                // Update parent's right side with nodeToDelete's child
                parent->pRight = nodeToDelete->pRight;
                
                // Update nodeToDelete's child's parent
                nodeToDelete->pRight->pParent = parent;
            }
            else
            {
                // Update parent's left side with nodeToDelete's child
                parent->pLeft = nodeToDelete->pRight;

                // Update nodeToDelete's child's parent
                nodeToDelete->pRight->pParent = parent;
            }
        }
        else // Node has its child on the left side
        {
            // Determine if nodeToDelete is right child
            if (nodeToDelete->isRightChild(nodeToDelete))
            {
                // Update parent's right side with nodeToDelete's child
                parent->pRight = nodeToDelete->pLeft;

                // Update nodeToDelete's child's parent
                nodeToDelete->pLeft->pParent = parent;
            }
            else
            {
                // Update parent's left side with nodeToDelete's child
                parent->pLeft = nodeToDelete->pLeft;

                // Update nodeToDelete's child's parent
                nodeToDelete->pLeft->pParent = parent;
            }
        }

        // Delete node, decrease number of elements by 1, and return the next node in the sequence.
        delete nodeToDelete;
        --numElements;
        return nextNode;
    }
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
    // If pNode is not null, link it to the current node
    if (pNode) {
        pNode->pParent = this;
    }
    
    // Link pNode to the current node's left
    pLeft = pNode;
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    // If pNode is not null, link it to the current node
    if (pNode) {
        pNode->pParent = this;
    }

    // Link pNode to the current node's right
    pRight = pNode;
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    // Create a new node and call addLeft
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
    // Create a new node and call addLeft
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
    // Create a new node and call addRight
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
    // Create a new node and call addRight
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
    // If pNode points to a nullptr, no advancement is performed
    if (!pNode)
        return *this;

    // If the current node has a right child
    if (pNode->pRight) {
        
        // Move to the right child
        pNode = pNode->pRight;

        // Move to the leftmost node in the right subtree
        while (pNode->pLeft)
            pNode = pNode->pLeft;
    }
    else {

        // Move up the tree until a node is found that is the left child of its parent
        BNode* pParent = pNode->pParent;
        while (pParent && pNode == pParent->pRight) {
            pNode = pParent;
            pParent = pParent->pParent;
        }

        // Set pNode to the parent, which will be the next in-order node
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
    // If pNode points to a nulltr, no decrement is done
    if (!pNode)
        return *this;

    // If the current node has a left child
    if (pNode->pLeft) {

        // Move to the left child
        pNode = pNode->pLeft;

        // Move to the rightmost node in the left subtree
        while (pNode->pRight)
            pNode = pNode->pRight;
    }
    else {
        // Move up the tree until a node that is a right child of its parent
        BNode* pParent = pNode->pParent;
        while (pParent && pNode == pParent->pLeft) {
            pNode = pParent;
            pParent = pParent->pParent;
        }

        // Set pNode to the parent, which will be the previous node in-order
        pNode = pParent;
    }
    return *this;
}


} // namespace custom

