#include "vector.h"

#include <algorithm>
#include <cassert>
#include <iostream>

template <typename T>
Vector<T>::Vector() : allocatedAmount(INITIAL_VEC_CAPACITY)
{
    ptr = new T[INITIAL_VEC_CAPACITY];
}

template <typename T>
Vector<T>::Vector(const Vector &copied) : allocatedAmount(copied.allocatedAmount), currentAmount(copied.currentAmount)
{
    ptr = new T[allocatedAmount];
    std::copy(copied.ptr, copied.ptr + copied.currentAmount, ptr);
}

template <typename T>
Vector<T>::Vector(Vector &&moved) : allocatedAmount(INITIAL_VEC_CAPACITY)
{
    ptr = new T[INITIAL_VEC_CAPACITY];
    (*this).swap(moved);
}

template <typename T>
Vector<T>::~Vector()
{
    delete[] ptr;
}

template <typename T>
void Vector<T>::push(const T elem)
{
    expandMemoryIfNeeded();
    ptr[currentAmount] = elem;
    currentAmount += 1;
}

template <typename T>
T &Vector<T>::operator[](const int index)
{
    return ptr[index];
}

template <typename T>
T &Vector<T>::operator[](const int index) const
{
    return ptr[index];
}

template <typename T>
Vector<T> &Vector<T>::operator=(const Vector &v)
{
    if (this == &v)
    {
        return *this;
    }
    allocatedAmount = v.allocatedAmount;
    currentAmount = v.currentAmount;
    delete[] ptr;
    ptr = new T[allocatedAmount];
    std::copy(v.ptr, v.ptr + v.currentAmount, ptr);
    return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator=(Vector &&v)
{
    if (this == &v)
    {
        return *this;
    }
    delete[] ptr;
    allocatedAmount = INITIAL_VEC_CAPACITY;
    currentAmount = 0;
    ptr = new T[INITIAL_VEC_CAPACITY];
    (*this).swap(v);
    return *this;
}

template <typename T>
T &Vector<T>::back()
{
    assert((void("no elements in flatmap"), currentAmount > 0));
    return ptr[currentAmount - 1];
}

template <typename T>
std ::size_t Vector<T>::size() const
{
    return currentAmount;
}

template <typename T>
std ::size_t Vector<T>::capacity()
{
    return allocatedAmount;
}

template <typename T>
void Vector<T>::swap(Vector<T> &b)
{
    std::swap(b.ptr, this->ptr);
    std::swap(b.allocatedAmount, this->allocatedAmount);
    std::swap(b.currentAmount, this->currentAmount);
}

template <typename T>
void Vector<T>::clear()
{
    delete[] ptr;
    allocatedAmount = INITIAL_VEC_CAPACITY;
    currentAmount = 0;
    ptr = new T[allocatedAmount];
}

template <typename T>
void Vector<T>::erase(const std ::size_t pos)
{
    for (size_t i = pos; i < currentAmount - 1; ++i)
    {
        ptr[i] = ptr[i + 1];
    }
    currentAmount -= 1;
}

template <typename T>
void Vector<T>::insert(const std ::size_t pos, const T elem)
{
    expandMemoryIfNeeded();
    T buf = ptr[pos];
    ptr[pos] = elem;
    if (pos == allocatedAmount - 1)
    {
        currentAmount += 1;
        return;
    }
    for (size_t i = currentAmount; i > pos + 1; --i)
    {
        ptr[i] = ptr[i - 1];
    }
    ptr[pos + 1] = buf;
    currentAmount += 1;
}

template <typename T>
bool Vector<T>::empty() const
{
    if (currentAmount == 0)
    {
        return true;
    }
    return false;
}

template <typename T>
void Vector<T>::print()
{
    for (std ::size_t i = 0; i < currentAmount; ++i)
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

template <typename T>
T *Vector<T>::begin() const
{
    return ptr;
}

template <typename T>
T *Vector<T>::end() const
{
    return ptr + size();
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
    for (int i = 0; i < currentAmount; i++)
    {
        newPtr[i] = ptr[i];
    }
    delete[] ptr;
    ptr = newPtr;
}
