#include "vector.h"
#include <iostream>
#include <memory.h>

Vector::Vector()
{
    ptr = new int[INITIAL_VEC_CAPACITY];
    allocatedAmount = INITIAL_VEC_CAPACITY;
}
Vector ::Vector(const Vector &copied)
{
    allocatedAmount = copied.allocatedAmount;
    currentAmount = copied.currentAmount;
    ptr = new int[allocatedAmount];
    memcpy(ptr, copied.ptr, currentAmount * sizeof(int));
}
Vector ::Vector(Vector &&moved)
{
    ptr = moved.ptr;
    allocatedAmount = moved.allocatedAmount;
    currentAmount = moved.currentAmount;
    moved.ptr = nullptr;
    moved.allocatedAmount = INITIAL_VEC_CAPACITY;
    moved.currentAmount = 0;
}
Vector ::~Vector()
{
    delete (ptr);
}

void Vector ::push(int elem)
{
    expandMemoryIfNeeded();
    ptr[currentAmount] = elem;
    currentAmount += 1;
}

int &Vector ::operator[](int index)
{
    return ptr[index];
}

Vector &Vector ::operator=(Vector &v)
{
    if (this == &v)
    {
        return *this;
    }
    allocatedAmount = v.allocatedAmount;
    currentAmount = v.currentAmount;
    ptr = new int[allocatedAmount];
    memcpy(ptr, v.ptr, currentAmount * sizeof(int));
    return *this;
}

Vector &Vector ::operator=(Vector &&v)
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

int Vector ::vector_back()
{
    return ptr[currentAmount - 1];
}

std ::size_t Vector ::vector_size() const
{
    return currentAmount;
}

std ::size_t Vector ::vector_capacity()
{
    return allocatedAmount;
}

void Vector ::print_vector()
{
    for (std ::size_t i = 0; i < currentAmount; ++i)
    {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

void Vector ::expandMemoryIfNeeded()
{
    if (currentAmount < allocatedAmount)
    {
        return;
    }
    allocatedAmount *= 2;
    int *newPtr = new int[allocatedAmount];
    memcpy(newPtr, ptr, allocatedAmount * sizeof(int));
    delete (ptr);
    ptr = newPtr;
}
