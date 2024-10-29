#include "flatmap.h"

#include <algorithm>
#include <string>

FlatMap::FlatMap() = default;
FlatMap::FlatMap(const FlatMap &b) : flatmap(b.flatmap) {}
FlatMap::FlatMap(FlatMap &&b) : flatmap(std::move(b.flatmap)) {}

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

void FlatMap::clear()
{
    flatmap.clear();
}

bool FlatMap::erase(const Key &k)
{
    if (contains(k))
    {
        flatmap.erase(position(k));
        return true;
    }
    return false;
}

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

bool FlatMap::contains(const Key &k) const
{
    const std::size_t pos = position(k);
    if (pos < flatmap.size())
    {
        return flatmap[pos].first == k;
    }
    return false;
}

Value &FlatMap::operator[](const Key &k)
{
    const std::size_t k_pos = position(k);
    if (contains(k))
    {
        return flatmap[k_pos].second;
    }
    insert(k, value);
    return flatmap[k_pos].second;
}

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

bool operator==(const FlatMap &a, const FlatMap &b)
{
    return std::equal(a.flatmap.begin(), a.flatmap.end(), b.flatmap.begin(), b.flatmap.end());
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
