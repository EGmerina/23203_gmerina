#include "flatmap.h"

#include <algorithm>
#include <string>

FlatMap::FlatMap() = default;
FlatMap::FlatMap(const FlatMap &b) : flatmap(b.flatmap) {}
FlatMap::FlatMap(FlatMap &&b) : flatmap(std::move(b.flatmap)) {}

// Обменивает значения двух флетмап.
void FlatMap::swap(FlatMap &b)
{
    flatmap.swap(b.flatmap);
}

FlatMap &FlatMap::operator=(const FlatMap &b)
{
    if (this == &b)
    {
        return *this;
    }
    flatmap = b.flatmap;
    return *this;
}
FlatMap &FlatMap ::operator=(FlatMap &&b)
{
    if (this == &b)
    {
        return *this;
    }
    flatmap = std ::move(b.flatmap);
    return *this;
}

// Очищает контейнер.
void FlatMap::clear()
{
    flatmap.clear();
}
// Удаляет элемент по заданному ключу.
bool FlatMap::erase(const Key &k)
{
    if (contains(k))
    {
        flatmap.erase(position(k));
        return true;
    }
    return false;
}
// Вставка в контейнер. Возвращаемое значение - успешность вставки.
bool FlatMap::insert(const Key &k, const Value &v)
{
    if (contains(k))
    {
        return false;
    }
    Pair new_pair{k, v};
    flatmap.insert(position(k), new_pair);
    return true;
}

// Проверка наличия значения по заданному ключу.
bool FlatMap::contains(const Key &k) const
{
    std::size_t pos = position(k);
    if (pos < flatmap.size())
    {
        return flatmap[pos].first == k;
    }
    return false;
}

// Возвращает значение по ключу. Небезопасный метод.
// В случае отсутствия ключа в контейнере, следует вставить в контейнер
// значение, созданное конструктором по умолчанию и вернуть ссылку на него.
Value &FlatMap::operator[](const Key &k)
{
    unsigned int k_pos = position(k);
    if (contains(k))
    {
        return flatmap[k_pos].second;
    }
    insert(k, value);
    return flatmap[k_pos].second;
}

// Возвращает значение по ключу. Бросает исключение при неудаче.
Value &FlatMap::at(const Key &k)
{
    if (contains(k))
    {
        return flatmap[position(k)].second;
    }
    throw std ::runtime_error("key " + k + " doesn't exist");
}
const Value &FlatMap::at(const Key &k) const
{
    return const_cast<FlatMap &>(*this).at((k));
}

std::size_t FlatMap::size() const
{
    return flatmap.size();
}
bool FlatMap::empty() const
{
    return flatmap.empty();
}

/*
static bool pred(const Pair *a1, const Pair *a2)
{
    return (a1->first == a2->first) && (a1->second == a2->second);
}*/

bool operator==(const FlatMap &a, const FlatMap &b)
{
    if (a.flatmap.size() != b.flatmap.size())
    {
        return false;
    }
    for (std::size_t i = 0; i < a.flatmap.size(); ++i)
    {
        if ((a.flatmap[i].first != b.flatmap[i].first) || (a.flatmap[i].second != b.flatmap[i].second))
        {
            return false;
        }
    }
    return true;
    // return std::equal(&a.flatmap, &a.flatmap + a.flatmap.size(), &b.flatmap, &b.flatmap + b.flatmap.size(), pred);
}
bool operator!=(const FlatMap &a, const FlatMap &b)
{
    return !(a == b);
}

std ::size_t FlatMap::position(const Key &k) const
{
    if (flatmap.size() == 0)
    {
        return 0;
    }

    std::size_t beg = 0;
    std::size_t end = flatmap.size();
    std::size_t mid;
    while (beg < end)
    {
        mid = (end - beg) / 2;
        if (flatmap[beg + mid].first == k)
        {
            return beg + mid;
        }
        else if (flatmap[beg + mid].first < k)
        {
            beg = beg + mid + 1;
        }
        else if (flatmap[beg + mid].first > k)
        {
            end = beg + mid;
        }
    }
    return beg;
}
