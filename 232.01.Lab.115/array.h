/***********************************************************************
 * Header:
 *    Array
 * Summary:
 *    Our custom implementation of std::array
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *    
 *    This will contain the class definition of:
 *       array             : similar to std::array
 *       array :: iterator : an iterator through the array
 * Author
 *    Evan Riker
 ************************************************************************/

#pragma once

#include <stdexcept>

class TestArray; // forward declaration for unit tests

namespace custom
{

/************************************************
 * ARRAY
 * A class that holds stuff
 ***********************************************/
template <typename T, int N>
struct array{    
public:
    T __elems_[N];
   //
   // Construct
   //

   // No constructors or destructors needed here

   //
   // Assign
   //

   // No assignment operators needed here

   //
   // Iterator
   //

   // Forward declaration for the iterator class
   class iterator;

   // Begin and end methods
   iterator begin() 
   { 
      /* your code goes here; it should return an iterator referring to the first element in the array */
      return iterator(__elems_); 
   }
   iterator end()   
   { 
      /* your code goes here; it should return an iterator referring to the first element off the end of the array */
      return iterator(__elems_ + N); 
   }

   // 
   // Access
   //

   // Subscript
   T& operator [] (size_t index){ 
        /* your code goes here; it should return the i-th element in the array */
        return at(index); 
   }
   const T& operator [] (size_t index) const { 
        /* your code goes here; it should return the i-th element in the array */
        return at(index); 
   }

   // At
   T& at(size_t index){ 
        if (index >= N) {
            std::string errorMsg = "Given index `" + std::to_string(index) +
                "` is out of range. Maximum Index: `" + std::to_string(N) + "`.";
            throw std::out_of_range(errorMsg);
        }
        /* your code goes here; it should return the i-th element in the array */
        return __elems_[index];
   }
   const T& at(size_t index) const { 
       if (index >= N) {
           std::string errorMsg = "Given index `" + std::to_string(index) +
               "` is out of range. Maximum Index: `" + std::to_string(N) + "`.";
           throw std::out_of_range(errorMsg);
       }
       /* your code goes here; it should return the i-th element in the array */
       return __elems_[index];
   }

   // Front and Back
   T& front(){ 
      /* your code goes here; it should return the first element in the array */
      return at(0); 
   }

   T& back(){ 
        /* your code goes here; it should return the last element in the array */
        const int lastIndex = N - 1;
        return at(lastIndex); 
   }

   const T& front() const { 
      /* your code goes here; it should return the first element in the array */
      return at(0); 
   }

   const T& back()  const{ 
        /* your code goes here; it should return the last element in the array */
        const int lastIndex = N - 1;
        
        return at(lastIndex); 
   }

   //
   // Remove

   // No clear or erase here

   //
   // Status
   //

   // number of elements in the array
   size_t size()  const { 
        /* your code goes here, this should not always return 999 */
        return N;           
   }
   bool empty() const 
   { 
        /* your code goes here; this should not always return TRUE */
       bool isEmpty = false;
       if (N == 0) {
           isEmpty = true;
       }
       return isEmpty;
   }
   
   //
   // Member Variable
   //
};

/**************************************************
 * ARRAY ITERATOR
 * An iterator through array
 *************************************************/
template <typename T, int N>
class array <T, N> :: iterator
{
   friend class ::TestArray; // give unit tests access to the privates
public:
   // constructors, destructors, and assignment operator
   iterator(){ 
      /* your code goes here; it should initialize an iterator */
       p = nullptr;
   }

   iterator(T * pInput){
      /* your code goes here; it should point to an element in the array */
       p = pInput;
   }

   iterator(const iterator & rhs){
      /* your code goes here; it should copy an iterator */
       p = rhs.p;
   }

   iterator & operator = (const iterator & rhs){
      /* your code goes here; it should copy an iterator */
      p = rhs.p;
      return *this;
   }
   
   // equals, not equals operator
   bool operator != (const iterator & rhs) const { 
      /* your code goes here; it should not always return TRUE */
      return !(*this == rhs); 
   }

   bool operator == (const iterator & rhs) const 
   { 
      /* your code goes here; it should not always return TRUE */
      return p == rhs.p; 
   }
   
   // dereference operator
   T & operator * () { 
      /* your code goes here; it should fetch the element referred to by p */
      return *p; 
   }

   const T & operator * () const{ 
      /* your code goes here; it should fetch the element referred to by p */
      return *p; 
   }
   
   // prefix increment
   iterator & operator ++ (){
      /* your code goes here; it should advance the iterator by one */
       ++p;
      return *this;
   }
   
   // postfix increment
   iterator operator ++ (int postfix){
      /* your code goes here; it should advance the iterator by one */
       iterator temp = *this;
       ++p;
       return temp;
   }
   
private:
   T * p;
};



}; // namespace custom
