#pragma once
#include <iostream>
#include <memory.h>

template <typename T>
class Vector
{
public:
    Vector();
    Vector(const Vector &copied);
    Vector(Vector &&moved);
    ~Vector();

    void push(T elem);

    T &operator[](int index);

    Vector &operator=(Vector &v);

    Vector &operator=(Vector &&v);

    T vector_back();

    std ::size_t vector_size() const;

    std::size_t vector_capacity();

    void print_vector();

private:
    static constexpr int INITIAL_VEC_CAPACITY = 4;
    T *ptr = nullptr;
    std::size_t currentAmount = 0;
    std::size_t allocatedAmount = 0;

    void expandMemoryIfNeeded();
};
