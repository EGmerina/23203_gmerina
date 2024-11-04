#pragma once

template <typename T>
class Vector
{
public:
    Vector();
    Vector(const Vector &copied);
    Vector(Vector &&moved);
    ~Vector();

    void push(const T elem);

    T &operator[](const std::size_t index);

    T &operator[](const std::size_t index) const;

    Vector &operator=(const Vector &v);

    Vector &operator=(Vector &&v);

    T &back();

    std::size_t size() const;

    std::size_t capacity();

    void print();

    void swap(Vector<T> &b);

    void clear();

    void erase(const std ::size_t pos);

    void insert(const std ::size_t pos, const T elem);

    bool empty() const;

    T *begin() const;

    T *end() const;

private:
    static constexpr int INITIAL_VEC_CAPACITY = 4;
    T *ptr = nullptr;
    std::size_t currentAmount = 0;
    std::size_t allocatedAmount = 0;

    void expandMemoryIfNeeded();
};

#include "vector.tpp"