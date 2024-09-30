#include "vector.h"
#include <iostream>
#include <memory.h>

class Vector
{
public:
    Vector()
    {
        ptr = new int[INITIAL_VEC_CAPACITY];
        allocatedAmount = INITIAL_VEC_CAPACITY;
    }
    Vector(const Vector &copied)
    {
        allocatedAmount = copied.allocatedAmount;
        currentAmount = copied.currentAmount;
        ptr = new int[allocatedAmount];
        memcpy(ptr, copied.ptr, currentAmount * sizeof(int));
    }
    Vector(Vector &&moved)
    {
        ptr = moved.ptr;
        allocatedAmount = moved.allocatedAmount;
        currentAmount = moved.currentAmount;
        moved.ptr = nullptr;
        moved.allocatedAmount = INITIAL_VEC_CAPACITY;
        moved.currentAmount = 0;
    }
    ~Vector()
    {
        delete (ptr);
    }

    void push(int elem)
    {
        expandMemoryIfNeeded();
        ptr[currentAmount] = elem;
        currentAmount += 1;
    }

    int &operator[](int index)
    {
        return ptr[index];
    }

    Vector &operator=(Vector &v)
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

    Vector &operator=(Vector &&v)
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

    int vector_back()
    {
        return ptr[currentAmount - 1];
    }

    std ::size_t vector_size() const
    {
        return currentAmount;
    }

    std ::size_t vector_capacity()
    {
        return allocatedAmount;
    }

    void print_vector()
    {
        for (std ::size_t i = 0; i < currentAmount; ++i)
        {
            printf("%d ", ptr[i]);
        }
        printf("\n");
    }

private:
    static constexpr int INITIAL_VEC_CAPACITY = 4;
    int *ptr = nullptr;
    std ::size_t currentAmount = 0;
    std::size_t allocatedAmount = 0;

    void expandMemoryIfNeeded()
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
};
