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
Vector<T>::Vector(const Vector &copied) : currentAmount(copied.currentAmount), allocatedAmount(copied.allocatedAmount)
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
T &Vector<T>::operator[](const std::size_t index)
{
    return ptr[index];
}

template <typename T>
T &Vector<T>::operator[](const std::size_t index) const
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
void Vector<T>::swap(Vector<T> &b) noexcept
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

    std::copy(ptr + pos + 1, this->end(), ptr + pos);
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
    std::copy_backward(ptr + pos + 1, ptr + currentAmount, ptr + currentAmount + 1);
    ptr[pos + 1] = buf;
    currentAmount += 1;
}

template <typename T>
bool Vector<T>::empty() const
{
    return currentAmount == 0;
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
    constexpr int EXPANSION_FACTOR = 2;
    if (currentAmount < allocatedAmount)
    {
        return;
    }
    allocatedAmount *= EXPANSION_FACTOR;
    T *newPtr = new T[allocatedAmount];
    std::copy(ptr, ptr + currentAmount, newPtr);
    delete[] ptr;
    ptr = newPtr;
}
