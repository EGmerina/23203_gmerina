#pragma once
#include <iostream>
#include <map>
#include <memory>

template <class Key, class Product>

class Factory
{
public:
    Factory(const Factory &b) = delete;
    Factory &operator=(const Factory &b) = delete;

    static Factory *getInstance()
    {
        static Factory f;
        return &f;
    }

    template <class... ParamTypes>
    std::unique_ptr<Product> createUnitByName(Key name, ParamTypes &&...parameters)
    {
        auto *creator = creators_.at(name);
        std::unique_ptr<Product> p(creator());
        return std::move(p);
    }

    bool registerCreator(const Key &name, Product *(*creator)())
    {
        creators_[name] = creator;
        return true;
    }

    bool are_availible_players(std::vector<std::string> &players)
    {
        for (auto name : players)
        {
            try
            {
                auto *creator = creators_.at(name);
            }
            catch (const std::exception &e)
            {
                std::cout << name << " doesn't want to play" << '\n';
                return false;
            }
        }
        return true;
    }

private:
    Factory() = default;
    ~Factory() = default;
    std::map<Key, Product *(*)()> creators_;
};