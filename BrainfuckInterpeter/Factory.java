
import java.security.Key;
import java.util.HashMap;

import commands.Command;

public class Factory {

    HashMap<String,Function<,Command>>creators;

    static Factory f;

    public static Factory getInstance() {

        return f;
    }

    public Command createUnitByName(String name) {
        var th =Thread.currentThread().getContextClassLoader().getResourceAsStream("commandsAndNamesOfClasses.config");
        //var th =ClassLoader.getResourceAsStream("commandsAndNamesOfClasses.config");
        var creator = creators.at(name);

    Command p(creator());
        return p;
    }

    // template<class...ParamTypes>std::unique_ptr<Product> createUnitByName(Key
    // name, ParamTypes &&...parameters)
    // {
    // auto *creator = creators_.at(name);
    // std::unique_ptr<Product> p(creator());
    // return std::move(p);
    // }

    public  boolean  registerCreator(const String name, Product *(*creator)())
    {
        creators[name] = creator;
        return true;
    }
}