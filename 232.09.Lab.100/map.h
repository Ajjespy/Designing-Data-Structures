/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map() 
   {
   }
   map(const map &  rhs) 
   { 
       // Assign rhs bst to this
       bst = rhs.bst;
   }
   map(map && rhs) 
   { 
       // Assign rhs bst to this
       bst = std::move(rhs.bst);
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
       // Iterate from first to last
       for (Iterator it = first; it != last; ++it)
       {
           // Insert each iterator using the bst.insert method
           bst.insert(*it);
       }
   }
   map(const std::initializer_list <Pairs>& il) 
   {
       // Use BST intializer list constructor
       bst = custom::BST <Pairs>(il);
   }
  ~map()         
   {
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
       // Assign rhs.bst to this.bst
       bst = rhs.bst;
       return *this;
   }
   map & operator = (map && rhs)
   {
       // Assign rhs.bst to this.bst using std::move 
       bst = std::move(rhs.bst);
       return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
       // Clear the contents of the bst first
       bst.clear();

       // Use initializer list constructor from BST class. 
       bst = custom::BST <Pairs>(il); 
       return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
       // Return bst.begin() method
       return bst.begin();
   }
   iterator end() 
   { 
       // Return bst.end() method
       return bst.end();
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator    find(const K & k)
   {
       // Return bst.find() method
       return bst.find(k);
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
       custom::pair<typename map::iterator, bool> pairReturn(end(), false);
       std::pair<iterator, bool> tempPair = bst.insert(rhs, true);
       
       pairReturn.first = iterator(tempPair.first);
       pairReturn.second = tempPair.second;

       return pairReturn;
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
       custom::pair<typename map::iterator, bool> pairReturn(end(), false);
       std::pair<iterator, bool> tempPair = bst.insert(rhs, true);

       pairReturn.first = iterator(tempPair.first);
       pairReturn.second = tempPair.second;

       return pairReturn;
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       // Iterate from first to last
       for (Iterator it = first; it != last; ++it)
       {
           // Insert each iterator using the bst.insert method
           bst.insert(it, true);
       }
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
       // Iterate through each element in the initializer list
       for (const Pairs& element : il)
       {
           // Insert each element using the bst.insert method, use true because we don't want duplicates
           bst.insert(element, true);
       }
   }

   //
   // Remove
   //
   void clear() noexcept
   {
       // Use bst.clear()
       bst.clear();
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept { return bst.empty(); }
   size_t size() const noexcept { return bst.size(); }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //
   iterator()
   {
       it = nullptr;
   }
   iterator(const typename BST < pair <K, V> > :: iterator & rhs)
   { 
       it = rhs;
   }
   iterator(const iterator & rhs) 
   { 
       it = rhs.it;
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
       it = rhs.it;
       return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const 
   { 
       return it == rhs.it; 
   }
   bool operator != (const iterator & rhs) const 
   { 
       return it != rhs.it; 
   }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
       iterator temp = *this;
       ++(it);
       return temp;
   }
   iterator operator ++ (int postfix)
   {
       ++(*this);
       return *this;
   }
   iterator & operator -- ()
   {
       iterator temp = *this;
       --(it);
       return temp;
   }
   iterator  operator -- (int postfix)
   {
       --(*this);
       return *this;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
    Pairs temp(key, V());
    std::pair<iterator, bool> returnPair = bst.insert(temp, true);

    iterator itBST = returnPair.first;

    return itBST.it.pNode->data.second;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V> 
const V& map <K, V> :: operator [] (const K& key) const 
{
    Pairs temp(key, V());
    iterator itBST = bst.find(temp);

    if (itBST != bst.end())
    {
        return itBST.it.pNode->data.second;
    }
    else
    {
        return temp.second;
    }
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
    Pairs temp(key, V());
    iterator itBST = bst.find(temp);

    if (itBST != bst.end())
    {
        return itBST.it.pNode->data.second;
    }
    else
    {
        throw std::out_of_range("invalid map<K, T> key") ;
    }
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
    Pairs temp(key, V());
    iterator itBST = bst.find(temp);

    if (itBST != bst.end())
    {
        return itBST.it.pNode->data.second;
    }
    else
    {
        throw std::out_of_range("invalid map<K, T> key"); 
    }
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
    // Use std::swap to swap this.bst and rhs.bst
    std::swap(lhs.bst, rhs.bst); 
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
    // Find the element in the set
    iterator it = find(k);

    // If element is found, erase it and return 1 to indicate one element erased
    if (it != end()) {
        erase(it); 
        return 1;
    }

    // If element is not found, return 0 to indicate no elements were erased
    return 0;
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
    //set an iterator to return
    iterator temp = first; 

    // Iterate from itBegin to itEnd
    for (iterator it = first; it != last;) 
    {
        //erase automatically returns an iterator to the next node
        it = erase(it); 
        temp = it; 
    }

    return temp; 
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
    // Use bst.erase method, access the bst iterator within the set iterator
    return bst.erase(it.it); 
}

}; //  namespace custom

