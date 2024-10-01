#include "vector.h"
#include <iostream>
#include <memory.h>

Vector::Vector()
{
    ptr = new T[INITIAL_VEC_CAPACITY];
    allocatedAmount = INITIAL_VEC_CAPACITY;
}
Vector ::Vector(const Vector &copied)
{
    allocatedAmount = copied.allocatedAmount;
    currentAmount = copied.currentAmount;
    ptr = new T[allocatedAmount];
    memcpy(ptr, copied.ptr, currentAmount * sizeof(T));
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

void Vector ::push(T elem)
{
    expandMemoryIfNeeded();
    ptr[currentAmount] = elem;
    currentAmount += 1;
}

T &Vector ::operator[](int index)
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
    ptr = new T[allocatedAmount];
    memcpy(ptr, v.ptr, currentAmount * sizeof(T));
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

T Vector ::vector_back()
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
    T *newPtr = new T[allocatedAmount];
    memcpy(newPtr, ptr, allocatedAmount * sizeof(T));
    delete (ptr);
    ptr = newPtr;
}

int main()
{
    return 0;
}
