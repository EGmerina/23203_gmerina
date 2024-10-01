#include "vector.h"
#include <iostream>
#include <memory.h>

template <typename T>
Vector<T>::Vector()
{
    ptr = new T[INITIAL_VEC_CAPACITY];
    allocatedAmount = INITIAL_VEC_CAPACITY;
}

template <typename T>
Vector<T>::Vector(const Vector &copied)
{
    allocatedAmount = copied.allocatedAmount;
    currentAmount = copied.currentAmount;
    ptr = new T[allocatedAmount];
    memcpy(ptr, copied.ptr, currentAmount * sizeof(T));
}

template <typename T>
Vector<T>::Vector(Vector &&moved)
{
    ptr = moved.ptr;
    allocatedAmount = moved.allocatedAmount;
    currentAmount = moved.currentAmount;
    moved.ptr = nullptr;
    moved.allocatedAmount = INITIAL_VEC_CAPACITY;
    moved.currentAmount = 0;
}

template <typename T>
Vector<T>::~Vector()
{
    delete (ptr);
}

template <typename T>
void Vector<T>::push(T elem)
{
    expandMemoryIfNeeded();
    ptr[currentAmount] = elem;
    currentAmount += 1;
}

template <typename T>
T &Vector<T>::operator[](int index)
{
    return ptr[index];
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector &v)
{
    if (this == &v)
    {
        return *this;
    }
    allocatedAmount = v.allocatedAmount;
    currentAmount = v.currentAmount;
    ptr = new T[allocatedAmount];
    memcpy(ptr, v.ptr, currentAmount * sizeof(T));
    return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector &&v)
{
    if (this == &v)
    {
        return *this;
    }
    ptr = v.ptr;
    allocatedAmount = v.allocatedAmount;
    currentAmount = v.currentAmount;
    v.ptr = nullptr;
    v.allocatedAmount = INITIAL_VEC_CAPACITY;
    v.currentAmount = 0;
    return *this;
}

template <typename T>
T Vector<T>::vector_back()
{
    return ptr[currentAmount - 1];
}

template <typename T>
std ::size_t Vector<T>::vector_size() const
{
    return currentAmount;
}

template <typename T>
std ::size_t Vector<T>::vector_capacity()
{
    return allocatedAmount;
}

template <typename T>
void Vector<T>::print_vector()
{
    for (std ::size_t i = 0; i < currentAmount; ++i)
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

template <typename T>
void Vector<T>::expandMemoryIfNeeded()
{
    if (currentAmount < allocatedAmount)
    {
        return;
    }
    allocatedAmount *= 2;
    T *newPtr = new T[allocatedAmount];
    memcpy(newPtr, ptr, allocatedAmount * sizeof(T));
    delete (ptr);
    ptr = newPtr;
}

