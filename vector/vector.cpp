#include <assert.h>
#include <iostream>
#include <memory.h>

#define INITIAL_VEC_CAPACITY 4

class Vector
{
public:
    Vector()
    {
        ptr = new int[INITIAL_VEC_CAPACITY];
        assert(nullptr == ptr);
        allocatedAmount = INITIAL_VEC_CAPACITY;
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

private:
    int *ptr = nullptr;
    size_t currentAmount = 0;
    size_t allocatedAmount = 0;

    void expandMemoryIfNeeded()
    {
        if (currentAmount < allocatedAmount)
        {
            return;
        }
        allocatedAmount *= 2;
        int *newPtr = new int[allocatedAmount];
        assert(nullptr == newPtr);
        memcpy(newPtr, ptr, allocatedAmount * sizeof(int));
        delete (ptr);
        ptr = newPtr;
    }
};

int main()
{
    Vector v;
    v.push(23203);
}
