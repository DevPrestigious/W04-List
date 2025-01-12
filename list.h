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
 *    Stephen Costigan, Alexander Dohms
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
             FOR it <- rhs.begin() � rhs.end()
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
   ~list() { }                                         // Deconstructor (edited by steve)
   
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
   // Access
   //
   
   T & front();
   T & back();

   //
   // Insert
   //
   
   void push_front(const T &  data);
   void push_front(      T && data);
   void push_back (const T &  data);
   void push_back (      T && data);
   iterator insert(iterator it, const T &  data);
   iterator insert(iterator it,       T && data);

   //
   // Remove
   //
   
   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator & it);

   //
   // Status
   //
   
   bool empty()  const { return numElements; } 
   size_t size() const { return numElements > 0 ? numElements : 0 ; } 

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
    iterator(){ p = nullptr; }
    iterator(Node* pRHS) { p = pRHS; }
    iterator(const iterator& rhs) { p = rhs.p; }
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
        return p->data;
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
    // Fill constructor
    /*IF (num)
          pHead <- pPrevious <- pNew <- NEW Node(T)
          pHead.pPrev <- NULL
          FOR i <-1 � num-1
            pNew <- NEW Node(T)
            pNew.pPrev <- pPrev
            pNew.pPrev.pNext <- pNew
            pPrevious <- pNew

          pNew.pNext <- NULL
          pTail <- pNew
          numElements <- num*/


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
    this->clear();

    if (this->capacity() < rhs.size())
        this->resize(rhs.size());

    for (int i = 0; i < rhs.size(); i++)
        this->insert(rhs.data[i]);

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
template <typename T, typename A> // -- Alex (stolen by steve)
void list <T, A> :: clear()
{
    if (numElements > 0) {
        while (pHead != nullptr)
        {
            Node* pDelete = pHead;
            pHead = pHead->pNext;
            if(pDelete)
                delete pDelete;
        }
    }
    // Set list default values
    pHead = pTail = NULL;
    numElements = 0;
}

/*********************************************
 * LIST :: PUSH BACK                            -- Alex (stolen by steve)
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T, typename A> 
void list <T, A> :: push_back(const T & data)
{
    Node* pNew = new Node(data);

    if (numElements == 0)
    {
        pHead = pTail = pNew;
    }
    else
    {
        pTail->pNext = pNew;
        pNew->pPrev = pTail;
        pTail = pNew;
    }

    numElements++;
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
    Node* pNew = new Node(data);
    if (pNew != nullptr) {
        if (numElements == 0)
        {
            pHead = pTail = pNew;
        }
        else
        {
            pHead->pPrev = pNew;
            pNew->pNext = pHead;
            pHead = pNew;
        }
        numElements++;
    }
}

template <typename T, typename A>
void list <T, A> ::push_front(T && data)
{
    Node* pNew = new Node(data);
    if (pNew != nullptr) {
        if (numElements == 0)
        {
            pHead = pTail = std::move(pNew);
        }
        else
        {
            pNew->pNext = pHead;
            pHead->pPrev = std::move(pNew);
            pHead = std::move(pNew);
        }
        numElements++;
    }
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
    // Added by steve, but doesn't seem to change %. Must need to use allocator
    if (!empty())
    {
        pTail = pTail->pPrev;
        numElements--;
    }
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
    // Added by steve, but doesn't seem to change %. Must need to use allocator
    if (!empty())
    {
        pHead = pHead->pNext;
        numElements--;
    }
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
    
    return pHead->data; // Added by steve, seems to work fine
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
    return pTail->data; // Added by steve, seems to work fine
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
//    list.erase(it)
//    itNext <- end()
//    IF it.p.pNext Take care of any nodes after �it� it.p.pNext.pPrev <- it.p.pPrev
//    itNext <- it.p.pNext ELSE
//    pTail <- pTail.pPrev
//    IF it.p.pPrev
//    it.p.pPrev.pNext <- it.p.pNext
//    ELSE
//    pHead <- pHead.pNext
//    DELETE it.p
//    numElements--
//    RETURN itNext
//    Take care of any nodes before �it�
//    Delete then node
    
    
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
    Node* pNew = new Node(data);

    if (numElements == 0)
    {
        pHead = pTail = pNew;
    }

    if (it.p)
    {
        pNew->pNext = it.p;
        pNew->pPrev = it.p->pPrev;
        it.p->pPrev = pNew;

        if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;

        if (it.p == pHead)
            pHead = pNew;
    }
    else
    {
        pTail->pNext = pNew;
        pNew->pPrev = pTail;
        pTail = pNew;
    }

    numElements++;
    return iterator(pNew);
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
    list <T> test = std::move(lhs);
    lhs = std::move(rhs);
    rhs = std::move(test);
}

}; // namespace custom



