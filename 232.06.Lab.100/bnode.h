/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Austin Jesperson, Emilio Ordonez, Evan Riker
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

 /*****************************************************************
  * BNODE
  * A single node in a binary tree.  Note that the node does not know
  * anything about the properties of the tree so no validation can be done.
  *****************************************************************/
template <class T>
class BNode
{
public:
    // 
    // Construct
    //
    BNode()
    {
        pLeft = pRight = pParent = nullptr;
        this->data = T();
    }
    BNode(const T& t)
    {
        pLeft = pRight = pParent = nullptr;
        this->data = t;
    }
    BNode(T&& t)
    {
        pLeft = pRight = pParent = nullptr;
        this->data = t;
    }

    //
    // Data
    //
    BNode <T>* pLeft;
    BNode <T>* pRight;
    BNode <T>* pParent;
    T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T>* p)
{
    if (p)
        return size(p->pLeft) + 1 + size(p->pRight);
    return 0;
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T>* pNode, BNode <T>* pAdd)
{
    //assert that given nodes aren't null
    if (pNode && pAdd)
    {
        //attach nodes
        pAdd->pParent = pNode;
        pNode->pLeft = pAdd;
    }

}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight(BNode <T>* pNode, BNode <T>* pAdd)
{
    //assert that given nodes aren't null
    if (pNode && pAdd)
    {
        //attach nodes
        pAdd->pParent = pNode;
        pNode->pRight = pAdd;
    }
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T>* pNode, const T& t)
{
    //assert that given nodes aren't null
    if (pNode)
    {
        //create a new node with given data
        BNode<T>* pAdd = new BNode<T>(t);

        //attach nodes
        pAdd->pParent = pNode;
        pNode->pLeft = pAdd;
    }
}

template <class T>
inline void addLeft(BNode <T>* pNode, T&& t)
{
    //assert that given nodes aren't null
    if (pNode)
    {
        //create a new node with given data
        BNode<T>* pAdd = new BNode<T>(t);

        //attach nodes
        pAdd->pParent = pNode;
        pNode->pLeft = pAdd;
    }
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight(BNode <T>* pNode, const T& t)
{
    //assert that given nodes aren't null
    if (pNode)
    {
        //create a new node with given data
        BNode<T>* pAdd = new BNode<T>(t);

        //attach nodes
        pAdd->pParent = pNode;
        pNode->pRight = pAdd;
    }
}

template <class T>
void addRight(BNode <T>* pNode, T&& t)
{
    //assert that given nodes aren't null
    if (pNode)
    {
        //create a new node with given data
        BNode<T>* pAdd = new BNode<T>(t);

        //attach nodes
        pAdd->pParent = pNode;
        pNode->pRight = pAdd;
    }
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T>*& pThis)
{
    //assert that given nodes aren't null
    if (pThis)
    {
        //check children first
        clear(pThis->pLeft);
        clear(pThis->pRight);

        //delete curent node and set to null
        delete pThis;
        pThis = nullptr;
    }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
    //set temp node to save data
    BNode<T>* pTemp = copy(pLHS);

    //swap right and left using copy, this preserves data and connections
    pLHS = copy(pRHS);
    pRHS = copy(pTemp);

}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T>* copy(const BNode <T>* pSrc)
{
    //assert that given nodes have data, otherwise return null
    if (pSrc)
    {
        //create a node for pDest
        BNode <T>* pDest = new BNode<T>(pSrc->data);

        //copy left and attach if successful
        pDest->pLeft = copy(pSrc->pLeft);
        if (pDest->pLeft)
        {
            pDest->pLeft->pParent = pDest;
        }

        //copy right and attach if successful
        pDest->pRight = copy(pSrc->pRight);
        if (pDest->pRight)
        {
            pDest->pRight->pParent = pDest;
        }

        return pDest;
    }
    else
    {
        return nullptr;
    }


}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T>*& pDest, const BNode <T>* pSrc)
{
    //first case: pSrc is null and pDest has data (clear doesn't do anything if pDest is empty)
    if (!pSrc)
    {
        clear(pDest);
    }
    else
    {
        //second case: pSrc has data but pDest does not
        if (!pDest)
        {
            //set pDest to an actual node
            pDest = new BNode<T>(pSrc->data);
        }
        // third case: both pSrc and pData have data
        else
        {
            //reassign pDest data to the pSrc data1
            pDest->data = pSrc->data;
        }

        //this happens in both cases 2 and 3

        //assign down the left side, attach nodes when successful
        assign(pDest->pLeft, pSrc->pLeft);
        if (pDest->pLeft)
        {
            pDest->pLeft->pParent = pDest;
        }

        //assign down the right side, attach nodes when successful
        assign(pDest->pRight, pSrc->pRight);
        if (pDest->pRight)
        {
            pDest->pRight->pParent = pDest;
        }
    }
}
