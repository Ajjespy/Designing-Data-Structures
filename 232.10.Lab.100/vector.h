/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
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
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Emilio Ordonez, Austin Jesperson, Evan Riker
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

    /*****************************************
     * VECTOR
     * Just like the std :: vector <T> class
     ****************************************/
    template <typename T>
    class vector
    {
        friend class ::TestVector; // give unit tests access to the privates
        friend class ::TestStack;
        friend class ::TestPQueue;
        friend class ::TestHash;
    public:

        // 
        // Construct
        //

        vector();
        vector(size_t numElements);
        vector(size_t numElements, const T& t);
        vector(const std::initializer_list<T>& l);
        vector(const vector& rhs);
        vector(vector&& rhs);
        ~vector();

        //
        // Assign
        //

        void swap(vector& rhs)
        {
            // Swap numElements and numCapacity
            std::swap(numElements, rhs.numElements);
            std::swap(numCapacity, rhs.numCapacity);

            // Swap the data pointers
            std::swap(data, rhs.data);
        }
        vector& operator = (const vector& rhs);
        vector& operator = (vector&& rhs);

        //
        // Iterator
        //

        class iterator;
        iterator       begin()
        {
            if (numElements == 0)
            {
                return iterator();
            }

            return iterator(&data[0]);
        }

        iterator       end()
        {
            return iterator(&data[numElements]);
        }

        //
        // Access
        //

        T& operator [] (size_t index);
        const T& operator [] (size_t index) const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        //
        // Insert
        //

        void push_back(const T& t);
        void push_back(T&& t);
        void reserve(size_t newCapacity);
        void resize(size_t newElements);
        void resize(size_t newElements, const T& t);

        //
        // Remove
        //

        void clear()
        {
            if (data != nullptr)
            {
                delete[] data;
                numElements = 0;
                data = new T [numCapacity];
            }
        }
        void pop_back()
        {
            if (data == nullptr)
            {
                return;
            }

            ~T(data[numElements - 1]);
            numElements--;
        }

        void shrink_to_fit();

        //
        // Status
        //

        size_t  size()          const { return numElements; }
        size_t  capacity()      const { return numCapacity; }
        bool empty() const
        {
            if (numElements == 0)
            {
                return true;
            }
            return false;
        }

        // adjust the size of the buffer

        // vector-specific interfaces

    private:

        T* data;                 // user data, a dynamically-allocated array
        size_t  numCapacity;       // the capacity of the array
        size_t  numElements;       // the number of items currently used
    };

    /**************************************************
     * VECTOR ITERATOR
     * An iterator through vector.  You only need to
     * support the following:
     *   1. Constructors (default and copy)
     *   2. Not equals operator
     *   3. Increment (prefix and postfix)
     *   4. Dereference
     * This particular iterator is a bi-directional meaning
     * that ++ and -- both work.  Not all iterators are that way.
     *************************************************/
    template <typename T>
    class vector <T> ::iterator
    {
        friend class ::TestVector; // give unit tests access to the privates
        friend class ::TestStack;
        friend class ::TestPQueue;
        friend class ::TestHash;
    public:
        // constructors, destructors, and assignment operator
        iterator() { this->p = nullptr; }
        iterator(T* p) { this->p = p; }
        iterator(const iterator& rhs) { this->p = rhs.p; }
        iterator(size_t index, vector<T>& v) { this->p = v.data + index; }
        iterator& operator = (const iterator& rhs)
        {
            this->p = rhs.p;
            return *this;
        }

        // equals, not equals operator
        bool operator != (const iterator& rhs) const
        {
            if (this->p != rhs.p)
            {
                return true;
            }
            return false;
        }

        bool operator == (const iterator& rhs) const
        {
            if (this->p == rhs.p)
            {
                return true;
            }
            return false;
        }

        // dereference operator
        T& operator * ()
        {
            return *p;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            ++p;
            return *this;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator temp = *this;
            ++p;
            return temp;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            --p;
            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator temp = *this;
            --p;
            return temp;
        }

    private:
        T* p;
    };

    /*****************************************
     * VECTOR :: DEFAULT constructors
     * Default constructor: set the number of elements,
     * construct each element, and copy the values over
     ****************************************/
    template <typename T>
    vector <T> ::vector()
    {
        data = nullptr;
        numElements = 0;
        numCapacity = 0;
    }

    /*****************************************
     * VECTOR :: NON-DEFAULT constructors
     * non-default constructor: set the number of elements,
     * construct each element, and copy the values over
     ****************************************/
    template <typename T>
    vector <T> ::vector(size_t num, const T& t)
    {
        numElements = num;
        numCapacity = num;
        data = new T[num];
        for (size_t i = 0; i < numElements; i++)
        {
            data[i] = t;
        }
    }

    /*****************************************
     * VECTOR :: INITIALIZATION LIST constructors
     * Create a vector with an initialization list.
     ****************************************/
    template <typename T>
    vector <T> ::vector(const std::initializer_list<T>& l)
    {
        numElements = l.size();
        numCapacity = l.size();
        data = new T[numElements];
        int index = 0;
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            data[index] = *it;
            index++;
        }
    }

    /*****************************************
     * VECTOR :: NON-DEFAULT constructors
     * non-default constructor: set the number of elements,
     * construct each element, and copy the values over
     ****************************************/
    template <typename T>
    vector <T> ::vector(size_t num)
    {
        numElements = num;
        numCapacity = num;

        if (num == 0)
        {
            data = nullptr;
        }
        else
        {
            data = new T[num];
            for (size_t i = 0; i < numElements; ++i)
            {
                data[i] = T();
            }

        }
    }

    /*****************************************
     * VECTOR :: COPY CONSTRUCTOR
     * Allocate the space for numElements and
     * call the copy constructor on each element
     ****************************************/
    template <typename T>
    vector <T> ::vector(const vector& rhs)
    {
        numElements = rhs.numElements;

        if (rhs.numElements < rhs.numCapacity)
        {
            numCapacity = rhs.numElements;
        }
        else
        {
            numCapacity = rhs.numCapacity;
        }

        if (rhs.numElements == 0)
        {
            data = nullptr;
        }
        else
        {
            data = new T[numElements];
            for (size_t i = 0; i < numElements; ++i)
            {
                data[i] = rhs.data[i];
            }
        }
    }

    /*****************************************
     * VECTOR :: MOVE CONSTRUCTOR
     * Steal the values from the RHS and set it to zero.
     ****************************************/
    template <typename T>
    vector <T> ::vector(vector&& rhs)
    {
        numElements = rhs.numElements;
        rhs.numElements = 0;
        numCapacity = rhs.numCapacity;
        rhs.numCapacity = 0;

        if (rhs.data == nullptr)
        {
            data = nullptr;
        }
        else
        {
            data = rhs.data;
            rhs.data = nullptr;
        }
    }

    /*****************************************
     * VECTOR :: DESTRUCTOR
     * Call the destructor for each element from 0..numElements
     * and then free the memory
     ****************************************/
    template <typename T>
    vector <T> :: ~vector()
    {
        delete[] data;
    }

    /***************************************
     * VECTOR :: RESIZE
     * This method will adjust the size to newElements.
     * This will either grow or shrink newElements.
     *     INPUT  : newCapacity the size of the new buffer
     *     OUTPUT :
     **************************************/
    template <typename T>
    void vector <T> ::resize(size_t newCapacity)
    {
        if (newCapacity == 0 && data == nullptr)
        {
            return;
        }

        if (newCapacity < numCapacity)
        {
            delete[] data;
            data = new T[numCapacity];
            numElements = 0;

        }
        else
        {
            T* newData = new T[newCapacity];
            for (size_t i = 0; i < std::min(numElements, newCapacity); i++)
            {
                newData[i] = data[i];
            }
            delete[] data;

            // Initialize new elements if newCapacity > numElements
            for (size_t i = numElements; i < newCapacity; i++)
            {
                newData[i] = T();
            }
            numCapacity = newCapacity;
            numElements = newCapacity;
            data = newData;
        }
    }

    template <typename T>
    void vector <T> ::resize(size_t newElements, const T& t)
    {
        T* newData = new T[newElements];
        for (size_t i = 0; i < numElements; i++)
        {
            newData[i] = data[i];
        }
        delete[] data;
        for (size_t i = numElements; i < newElements; i++)
        {
            newData[i] = t;
        }
        numCapacity = newElements;
        numElements = newElements;
        data = newData;
    }

    /***************************************
     * VECTOR :: RESERVE
     * This method will grow the current buffer
     * to newCapacity.  It will also copy all
     * the data from the old buffer into the new
     *     INPUT  : newCapacity the size of the new buffer
     *     OUTPUT :
     **************************************/
    template <typename T>
    void vector <T> ::reserve(size_t newCapacity)
    {
        if (newCapacity <= numCapacity)
        {
            return;
        }

        T* newData = new T[newCapacity];
        for (size_t i = 0; i < numElements; i++)
        {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        numCapacity = newCapacity;
    }

    /***************************************
     * VECTOR :: SHRINK TO FIT
     * Get rid of any extra capacity
     *     INPUT  :
     *     OUTPUT :
     **************************************/
    template <typename T>
    void vector <T> ::shrink_to_fit()
    {
        if (numCapacity == numElements)
        {
            return;
        }

        if (numElements == 0)
        {
            numCapacity = numElements;
            delete[] data;
            data = nullptr;
        }

        if (numCapacity > numElements)
        {
            T* shrinkedData = new T[numElements]; 
            for (size_t i = 0; i < numElements; i++)
            {
                shrinkedData[i] = data[i];
            }
            delete[] data;
            numCapacity = numElements;
            data = shrinkedData;
        }
    }

    /*****************************************
     * VECTOR :: SUBSCRIPT
     * Read-Write access
     ****************************************/
    template <typename T>
    T& vector <T> :: operator [] (size_t index)
    {
        return data[index];
    }

    /******************************************
     * VECTOR :: SUBSCRIPT
     * Read-Write access
     *****************************************/
    template <typename T>
    const T& vector <T> :: operator [] (size_t index) const
    {
        return data[index];
    }

    /*****************************************
     * VECTOR :: FRONT
     * Read-Write access
     ****************************************/
    template <typename T>
    T& vector <T> ::front()
    {
        return data[0];
    }

    /******************************************
     * VECTOR :: FRONT
     * Read-Write access
     *****************************************/
    template <typename T>
    const T& vector <T> ::front() const
    {
        return data[0];
    }

    /*****************************************
     * VECTOR :: FRONT
     * Read-Write access
     ****************************************/
    template <typename T>
    T& vector <T> ::back()
    {
        return data[numElements - 1];
    }

    /******************************************
     * VECTOR :: FRONT
     * Read-Write access
     *****************************************/
    template <typename T>
    const T& vector <T> ::back() const
    {
        return data[numElements - 1];
    }

    /***************************************
     * VECTOR :: PUSH BACK
     * This method will add the element 't' to the
     * end of the current buffer.  It will also grow
     * the buffer as needed to accomodate the new element
     *     INPUT  : 't' the new element to be added
     *     OUTPUT : *this
     **************************************/
    template <typename T>
    void vector <T> ::push_back(const T& t)
    {
        if (data == nullptr)
        {
            data = new T[1];
            data[0] = t;
            numCapacity = 1;
            numElements = 1;
        }
        else if (numCapacity > numElements)
        {
            data[numElements] = t;
            numElements++;
        }
        else if (numCapacity == numElements)
        {
            T* newData = new T[numCapacity * 2];

            for (size_t i = 0; i < numElements; i++)
            {
                newData[i] = data[i];
            }
            newData[numElements] = t;
            numElements++;
            numCapacity *= 2;
            delete[] data;
            data = newData;
        }
    }

    template <typename T>
    void vector <T> ::push_back(T&& t)
    {
        if (data == nullptr)
        {
            data = new T[1];
            data[0] = t;
            numCapacity = 1;
            numElements = 1;
        }
        else if (numCapacity > numElements)
        {
            data[numElements] = t;
            numElements++;
        }
        else if (numCapacity == numElements)
        {
            T* newData = new T[numCapacity * 2];

            for (size_t i = 0; i < numElements; i++)
            {
                newData[i] = data[i];
            }
            newData[numElements] = t;
            numElements++;
            numCapacity *= 2;
            delete[] data;
            data = newData;
        }
    }

    /***************************************
     * VECTOR :: ASSIGNMENT
     * This operator will copy the contents of the
     * rhs onto *this, growing the buffer as needed
     *     INPUT  : rhs the vector to copy from
     *     OUTPUT : *this
     **************************************/
    template <typename T>
    vector <T>& vector <T> :: operator = (const vector& rhs)
    {
        if (rhs.numElements == 0)
        {
            return *this;
        }

        if (this != &rhs) {
            delete[] data;

            numElements = rhs.numElements;
            if (rhs.numCapacity > numCapacity)
            {
                numCapacity = rhs.numCapacity;
            }

            data = new T[numCapacity];
            for (size_t i = 0; i < numElements; ++i) { 
                data[i] = rhs.data[i];
            }
        }
        return *this;
    }
    template <typename T>
    vector <T>& vector <T> :: operator = (vector&& rhs)
    {
        if (rhs.numElements == 0)
        {
            return *this;
        }

        if (this != &rhs) {
            delete[] data;

            numElements = rhs.numElements;
            rhs.numElements = 0;
            numCapacity = rhs.numCapacity;
            rhs.numCapacity = 0;

            data = new T[numCapacity];
            for (size_t i = 0; i < numElements; ++i) { 
                data[i] = rhs.data[i];
            }
            rhs.data = nullptr;
        }
        return *this;
    }




} // namespace custom

