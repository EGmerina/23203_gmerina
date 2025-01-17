#pragma once
#include "factory.h"

template <typename Class, typename Subclass, typename... ParamTypes>
Class *creator(ParamTypes... parameters)
{
    return new Subclass(parameters...);
}

template <typename Class, typename Subclass, typename... ParamTypes>
struct RegisterCreator
{
    RegisterCreator() = delete;
    RegisterCreator(std::string name, ParamTypes... parameters)
    {
        Factory<std::string, Class>::getInstance()->registerCreator(name, creator<Class, Subclass, ParamTypes...>);
    }
};
