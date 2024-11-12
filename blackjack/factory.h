#pragma once

template <class Key, class Product>

class Factory
{
public:
    static Factory *getInstance()
    {
        static Factory f;
        return &f;
    }

    std::unique_ptr<Product> createUnitByName(Key name)
    {
        auto *creator = creators_.at(name);
        std::unique_ptr<Unit> p(creator());
        return std::move(p);
    }

    bool registerCreator(const Key &name, Unit *(*creator)())
    {
        creators_[name] = creator;
        return true;
    }

private:
    std::map<Key, Unit *(*)()> creators_;
};