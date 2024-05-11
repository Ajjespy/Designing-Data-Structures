/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include "vector.h"

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T, class Container = custom::vector<T>>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   //
   // Construct
   //
   
   stack()                       { container.resize(defaultSize); }
   stack(const stack <T> &  rhs) {
       container.reserve(rhs.container.size()); // Reserve space to avoid unnecessary reallocations
       for (const T& element : rhs.container) {
           container.push_back(element); // Copy each element from rhs to this container
       }
   }
   stack(      stack <T> && rhs) { container.swap(rhs.container); }
   stack(const Container &  rhs) { container.resize(rhs.size()); }
   stack(      Container && rhs) { container.resize(rhs.size()); }
   ~stack()                      {                      }     
   
   //
   // Assign
   //
   stack <T> & operator = (const stack <T> & rhs)
   {
      return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
      return *this;
   }
   void swap(stack <T>& rhs)
   {

   }

   //
   // Access
   //
   
         T & top()       { return container[container.size() - 1]; }
   const T & top() const { return container[container.size() - 1]; }

   //
   // Insert
   //
   
   void push(const T& t)  { container.push_back(t); }
   void push(      T&& t) { container.push_back(std::move(t)); }

   //
   // Remove
   //
   
   void pop() 
   {  
       if (container.size() > 0)
       {
           container.pop_back();
       }
       else
       {
           return;
       }
   }

   //
   // Status
   //
   
   size_t size () const { return container.size(); }
   bool   empty() const {
       if (container.size() == 0)
       {
           return true;
       }
       return false;
   }
   
private:
   static int defaultSize = 0;
   Container container;  // underlying container (probably a vector)
};



} // custom namespace


