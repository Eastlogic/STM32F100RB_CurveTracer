/**
  ******************************************************************************
  * @file    my_stack.h 
  * @author  Eastlogic Lab.
  * @version v.1.0
  * @date    25/02/2022
  * @brief   Simple template Dynamically alocated stack implementation
  ******************************************************************************
  */ 

#ifndef MY_STACK_H
#define MY_STACK_H

#include <stdint.h>

#define MAX_STACK_SIZE  1024    // Maximum number of items to be stacked

template <class T>
class Stack
{
    public:
            Stack(uint16_t = 10);
            ~Stack();
            bool push(const T&);
            bool pop(T&);
            uint16_t GetNumber(void);    // Get number of stacked elements
           
    private:
        
            bool IsFull () const;
            bool IsEmpty () const;
            
            T* StackPtr;
            uint16_t size;              // Size of stack
            int16_t top;                // Top pointer of stack          
};



template <class T>
Stack<T>::Stack(uint16_t StackSize)
{
    size = ((StackSize>0) && (StackSize<MAX_STACK_SIZE)) ? StackSize : 10;
    top = -1;
    StackPtr = new T [size];  
}
    
template <class T>
Stack<T>::~Stack()
{
    delete [] StackPtr;
}


template <class T>
bool Stack<T>::IsFull() const
{
    return (top == (size-1));
}

template <class T>
bool Stack<T>::IsEmpty() const
{
    return (top == -1);
}

template <class T>
bool Stack<T>::push(const T& val)
{

    if (!IsFull())
    {
        top++;                      // "Full up-grown" stack
        StackPtr[top] = val;
        return true;
    }
    else
    {return false;}

}


template <class T>
bool Stack<T>::pop(T& val)
{

    if (!IsEmpty())
    {
        val = StackPtr[top];
        top--;
        return true;
    }
    else
    {return false;}
}



// Get the number of stacked items
template <class T>
uint16_t Stack<T>::GetNumber()
{
return (top+1);
}


#endif /* MY_STACK_H */

