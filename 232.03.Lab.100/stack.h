/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    Austin Jesperson, Emilio Ordonez, Evan Riker
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

   stack()                            { container.resize(0); }
   stack(const stack <T> &  rhs)      { container.resize(7); }
   stack(      stack <T> && rhs)      { container.resize(7); }
   stack(const std::vector<T> &  rhs) { container.resize(7); }
   stack(      std::vector<T> && rhs) { container.resize(7); }
   ~stack()                           {                      }

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

         T& top()       { return *(new T); }
   const T& top() const { return *(new T); }

   // 
   // Insert
   // 

   void push(const T&  t) 
   {
       container.push_back(t);
   }
   void push(      T&& t) 
   {
       container.push_back(std::move(t));
   }

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
   size_t  size() const { return container.size(); }
   bool empty   () const 
   { 
       if (container.size() == 0)
       {
           return true;
       }
       return false;
   }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace


