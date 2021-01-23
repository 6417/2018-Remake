#ifndef STACK_H
#define STACK_H

#include "pch.h"

// include Arduino basic header.
#include <Arduino.h>

// the definition of the stack class.
template <typename T, size_t size>
class Stack
{
public:
    // init the stack (constructor).
    Stack();

    // push an item to the stack.
    void push(const T i);

    // pop an item from the stack.
    T pop();

    // unshift an item to the stack.
    void unshift(const T i);

    // get an item from the stack.
    T peek() const;

    //get the 'index-th' element on the stack with out affectig the stack
    T peek(size_t index);

    // check if the stack is empty.
    bool isEmpty() const;

    // get the number of items in the stack.
    int count() const;

    // check if the stack is full.
    bool isFull() const;

    // makes the stack empty (doesn't actually delete the content) isEmpty will retunr true
    void clear();

private:
    // exit report method in case of error.
    void exit(const char *m) const;

    T contents[size]; // the array of the stack.
    int top;
    Print *printer = NULL; // the printer of the stack.the top index of the stack.
};

template <typename T, size_t size>
T Stack<T, size>::peek(size_t index)
{
    if (index > top)
        exit("Stack::peek: index out of bounds");
    return contents[top - index];
}

// makes the stack empty (doesn't actually delete the content) isEmpty will return true
template <typename T, size_t size>
void Stack<T, size>::clear()
{
    top = 0;
}

// init the stack (constructor).
template <typename T, size_t size>
Stack<T, size>::Stack()
{
    top = 0;  // set the initial top index of the stack.

    #ifdef DEBUG
        printer = &Serial;
    #endif
}

template <typename T, size_t size>
void Stack<T, size>::push(const T i)
{
    // check if the stack is full.
    if (isFull())
    {
        // shifts array and delets last element
        memcpy(contents, &contents[1], size - 1);
        top--;
    }

    // store the item to the array.
    contents[top] = i;
    top++;
}

// pop an item from the stack.
template <typename T, size_t size>
T Stack<T, size>::pop()
{
    // check if the stack is empty.
    if (isEmpty())
        exit("STACK: can't pop item from stack: stack is empty"); // will still retun a value but its undefined behaviour

    // fetch the top item from the array.
    top--;
    T item = contents[top];

    // return the top item from the array.
    return item;
}

// unshift an item to the stack.
template <typename T, size_t size>
void Stack<T, size>::unshift(const T i)
{
    // check if the stack is full.
    if (isFull())
        exit("STACK: can't unshift stack is full");

    // store the item to the array.
    for (int c = size - 1; c > 0; c--)
    {
        contents[c] = contents[c - 1];
    }
    contents[0] = i;
    top++;
}

// get an item from the stack.
template <typename T, size_t size>
T Stack<T, size>::peek() const
{
    // check if the stack is empty.
    if (isEmpty())
        exit("STACK: can't peek item from stack: stack is empty."); // will still return a value but is undefined behaviour

    // get the top item from the array.
    return contents[top - 1];
}

// check if the stack is empty.
template <typename T, size_t size>
bool Stack<T, size>::isEmpty() const
{
    return top == 0;
}

// check if the stack is full.
template <typename T, size_t size>
bool Stack<T, size>::isFull() const
{
    return top == size - 1;
}

// get the number of items in the stack.
template <typename T, size_t size>
int Stack<T, size>::count() const
{
    return top;
}

// exit report method in case of error.
template <typename T, size_t size>
void Stack<T, size>::exit(const char *m) const
{
    // print the message if there is a printer.
    if (printer)
        printer->println(m);
}

#endif // STACK_H