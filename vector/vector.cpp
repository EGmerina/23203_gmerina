#include <iostream>
#include <memory.h>

#define INITIAL_VEC_CAPACITY 4

class Vector
{
public:
    Vector()
    {
        ptr = new int[INITIAL_VEC_CAPACITY];
        allocatedAmount = INITIAL_VEC_CAPACITY;
    }
    Vector(const Vector &copied) // конструктор копирования
    {
        allocatedAmount = copied.allocatedAmount;
        currentAmount = copied.currentAmount;
        ptr = new int[allocatedAmount];
        memcpy(ptr, copied.ptr, currentAmount * sizeof(int));
    }
    Vector(Vector &&moved) // конструктор перемещения
    {
        ptr = moved.ptr;
        allocatedAmount = moved.allocatedAmount;
        currentAmount = moved.currentAmount;
        moved.ptr = nullptr;
        moved.allocatedAmount = INITIAL_VEC_CAPACITY;
        moved.currentAmount = 0;
        // or delete(moved);   ?????
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

    Vector &operator=(Vector &v) // операция присваивания
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

    Vector &operator=(Vector &&v) // операция перемещения
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
        memcpy(newPtr, ptr, allocatedAmount * sizeof(int));
        delete (ptr);
        ptr = newPtr;
    }
};

int main()
{
    Vector v;
    v.push(23203);
    std ::cout << v[0]; //??????? что распечатается?
    v[0] = 23203 + 1;
    std ::cout << v[0];
}
