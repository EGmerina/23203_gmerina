#include <iostream>

#define INITIAL_VEC_CAPACITY 4

class Vector
{
public:
    Vector()
    { // конструктор
        ptr = (int *)malloc(sizeof(int) * INITIAL_VEC_CAPACITY);
        allocatedAmount = INITIAL_VEC_CAPACITY;
    }
    ~Vector()
    { // деструктор
        free(ptr);
    }
    void push_back(int elem)
    {
        expandMemoryIfNeeded();
        ptr[currentAmount] = elem; // *(v->ptr + v->currentAmount) = elem;
        currentAmount += 1;
    }

private:
    void expandMemoryIfNeeded()
    {
    }
    int *ptr = nullptr;
    size_t currentAmount = 0;
    size_t allocatedAmount = 0;
};

int main()
{
    Vector v;

    v.push_back(23203);
}
// то же самое что и
/* struct Vector v;
vector_init(&v);
vector_deinit(&v);
*/