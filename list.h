/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Stephen Costigan
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T, typename A = std::allocator<T>>
class list
{
public:
   
   //
   // Construct -- Alex
   //
   
   list(const A& a = A())                                       
   {
       // PG 260
       /*list.default-constructor()
         pHead <- pTail <- NULL
         numElements <- 0
        */

      numElements = 99;
      pHead = pTail = new list <T, A> ::Node();
   }                              // Default constructor 
   list(list <T, A> & rhs, const A& a = A())                    
   {
       /*list.copy-constructor(rhs)
             pHead <- pTail <- NULL
             numElements <- 0
             FOR it <- rhs.begin() … rhs.end()
             push_back(*it)
    */
    /*list.copy-constructor(rhs)
      pHead <- pTail <- NULL
      numElements <- 0
      *this <- rhs */
      numElements = 99;
      pHead = pTail = new list <T, A> ::Node();
   }                              // Copy constructor 
   list(list <T, A>&& rhs, const A& a = A());                   // Move constructor
   list(size_t num, const T & t, const A& a = A());             // Non-default empty fill constructor
   list(size_t num, const A& a = A());                          // Non-default value fill constructor
   list(const std::initializer_list<T>& il, const A& a = A())   // Initializer list constructor
   {
      numElements = 99;
      pHead = pTail = new list <T, A> ::Node();
   }
   template <class Iterator>
   list(Iterator first, Iterator last, const A& a = A())        // Range constructor
   {
      numElements = 99;
      pHead = pTail = new list <T, A> ::Node();
   }
   ~list() { clear(); }                                         // Deconstructor (edited by steve)
   
   //
   // Assign -- Steve
   //
   
   list <T, A> & operator = (list <T, A> &  rhs);               // Copy-assign
   list <T, A> & operator = (list <T, A> && rhs);               // Move-assign
   list <T, A> & operator = (const std::initializer_list<T>& il); // Initializer list assign

   //
   // Iterator
   //
   
   // //// THIS IS DONE
   class iterator;
   iterator begin()  { return iterator (pHead);   }         
   iterator rbegin() { return iterator (pTail);   }
   iterator end()    { return iterator (nullptr); }
   // //// THIS IS DONE
   
   //
   // Access -- Jon
   //
   
   T & front();
   T & back();

   //
   // Insert -- Alex
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove -- Shaun
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return numElements > 0; }
   size_t size() const { return numElements;   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   // nested linked list class
   class Node;

   // member variables
   A    alloc;         // use alloacator for memory allocation
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;       // pointer to the beginning of the list
   Node * pTail;       // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T, typename A>
class list <T, A> :: Node
{
public:
   //
   // Construct
   //
   Node(               ) : pNext(nullptr), pPrev(nullptr), data(               ) { } // Default constructor
   Node(const T &  data) : pNext(nullptr), pPrev(nullptr), data(data           ) { } // Copy Constructor
   Node(      T && data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { } // Move Constructor

   //
   // Member Variables
   //

   T data;             // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T, typename A>
class list <T, A> ::iterator
{
public:
    //
    // Construct -- Steve
    //

    // constructors, destructors, and assignment operator
    iterator()
    {
        p = nullptr;
    }
    iterator(Node* pRHS)
    {
        p = pRHS;
    }
    iterator(const iterator& rhs)
    {
        p = rhs.p;
    }
    iterator& operator = (const iterator& rhs)
    {
        this->p = rhs.p;
        return *this;
    }

    // equals, not equals operator
    bool operator != (const iterator& rhs) const { return (rhs.p != p ? true : false); }
    bool operator == (const iterator& rhs) const { return (rhs.p == p ? true : false); }

    // dereference operator, fetch a node
    T& operator * ()
    {
        return *(new T); // the version from vector doesn't work here
    }

    // postfix increment
    iterator operator ++ (int postfix)
    {
        iterator i = p;
        p++;
        return i;
    }

    // prefix increment
    iterator& operator ++ ()
    {
        p++;
        return *this;
    }

    // postfix decrement
    iterator operator -- (int postfix)
    {
        iterator i = p;
        if (p > 0)
            p--;
        return i;
    }

    // prefix decrement
    iterator& operator -- ()
    {
        if (p > 0)
            p--;
        return *this;
    }
    //
    // Insert -- Jon
    //
    // two friends who need to access p directly
    friend iterator list <T, A> ::insert(iterator it, const T& data);
    friend iterator list <T, A> ::insert(iterator it, T&& data);
    friend iterator list <T, A> ::erase(const iterator& it);

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif

    typename list <T, A> ::Node* p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const T & t, const A& a) 
{
   numElements = 99;
   pHead = pTail = new list <T, A> ::Node();
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(size_t num, const A& a) 
{
   numElements = 99;
   pHead = pTail = new list <T, A> ::Node();

}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T, typename A>
list <T, A> ::list(list <T, A>&& rhs, const A& a) :
   numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail), alloc(a)
{
    /*list.move - constructor(rhs)
        pHead <- rhs.pHead
        pTail <- rhs.pTail
        numElements <- rhs.numElements
        rhs.pHead <- NULL
        rhs.pTail <- NULL
        rhs.numElements <- 0*/


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
template <typename T, typename A> // --Shaun
list <T, A>& list <T, A> :: operator = (list <T, A> && rhs)
{
    /*list.move-assignment(rhs)
     clear()
     swap(rhs)*/
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A> // --Shaun
list <T, A> & list <T, A> :: operator = (list <T, A> & rhs)
{
    
    /*list.copy-assignment(rhs)
         clear()
         FOREACH it IN rhs
         push_back(*it)
         RETURN this*/
    
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A> // --Shaun
list <T, A>& list <T, A> :: operator = (const std::initializer_list<T>& rhs)
{
    /*list.copy-assignment(rhs)
         itRHS <- rhs.begin() Fill existing nodes
         itLHS <- begin()
         WHILE itRHS != rhs.end() AND itLHS != end()
         *itLHS <- *itRHS
         ++itRHS
         ++itLHS
         IF itRHS != rhs.end() Add new nodes
         WHILE itRHS != rhs.end()
         push_back(*itRHS)
         ++itRHS
         ELSE IF rhs.empty()
         clear()
         ELSE IF it LHS != end() Remove the extra nodes
         p <- itLHS.p
         pTail <- p.pPrev
         pNext <- p.pNext
         WHILE p != NULL
         pNext <- p.pNext
         DELETE p
         p <- pNext
         numElements--
         pTail.pNext <- NULL
        */
   return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T, typename A> // -- Alex
void list <T, A> :: clear()
{
    // added from last week, might not work
    Node * pDelete = pHead; // redundant for first loop, but oh well
    while (pHead != nullptr) {
        pDelete = pHead;
        pHead = pHead->pNext;
        delete pDelete;
    }
    delete pHead;
}

/*********************************************
 * LIST :: PUSH BACK                            -- Alex
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A> 
void list <T, A> :: push_back(const T & data)
{

}

template <typename T, typename A>
void list <T, A> ::push_back(T && data)
{

}

/*********************************************
 * LIST :: PUSH FRONT                            -- Steve
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> :: push_front(const T & data)
{

}

template <typename T, typename A>
void list <T, A> ::push_front(T && data)
{

}


/*********************************************
 * LIST :: POP BACK                            -- Jon
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_back()
{

}

/*********************************************
 * LIST :: POP FRONT                            -- Jon
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A>
void list <T, A> ::pop_front()
{

}

/*********************************************
 * LIST :: FRONT                            -- Shaun
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: front()
{
   return *(new T);
}

/*********************************************
 * LIST :: BACK                            -- Shaun
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T, typename A>
T & list <T, A> :: back()
{
   return *(new T);
}


/******************************************
 * LIST :: REMOVE                            -- Alex
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator  list <T, A> :: erase(const list <T, A> :: iterator & it)
{
   return end();
}

/******************************************
 * LIST :: INSERT                            -- Steve
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> :: iterator list <T, A> :: insert(list <T, A> :: iterator it,
                                                 const T & data) 
{
   return end();
}


/******************************************
 * LIST :: INSERT                            -- Jon
 * add several items into the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T, typename A>
typename list <T, A> ::iterator list <T, A> ::insert(list <T, A> ::iterator it,
   T && data)
{
   return end();
}

/**********************************************
 * LIST :: assignment operator - MOVE                -- Shaun
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T, typename A>
void swap(list <T, A> & lhs, list <T, A> & rhs)
{
    /*list.swap(rhs)
     tempHead <- rhs.pHead
     rhs.pHead <- pHead
     pHead <- tempHead
     tempTail <- rhs.pTail
     rhs.pTail <- pTail
     pTail <- tempTail
     tempElements <- rhs.numElements
     rhs.numElements <- numElements
     numElements <- tempElements
    */
}

}; // namespace custom



