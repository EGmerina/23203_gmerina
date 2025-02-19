
import commands.Command;
import commands.DoNothing;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Properties;

public class Factory {

    private HashMap<String, Class> classes;

    private static Factory f = new Factory();

    public static Factory getInstance() {

        return f;
    }

    public Command createUnitByName(String name) throws ClassNotFoundException, IOException, NoSuchMethodException,
            InstantiationException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
        Class newClass;
        if (!classes.containsKey(name)) {
            var thread = Thread.currentThread().getContextClassLoader()
                    .getResourceAsStream("commandsAndNamesOfClasses.config");
            Properties properties = new Properties();
            properties.load(thread);
            if (properties.getProperty(name) == null) {
                Object command = DoNothing.getDeclaredConstructor().newInstance();
                return (Command) command;
            } else {
                newClass = Class.forName(properties.getProperty(name));
            }
            thread.close();
        } else {
            newClass = classes.get(name);
        }
        Object command = newClass.getDeclaredConstructor().newInstance();
        return (Command) command;
    }
}