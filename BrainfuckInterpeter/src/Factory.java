package src;

import src.commands.Command;
import src.myexceptions.FactoryException;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Properties;

/**
 * give object of Command class by name of command string.
 */
public class Factory {

    private final HashMap<String, Class> classes;
    private final Properties properties;

    public Factory() throws IOException {
        classes = new HashMap<>();
        try (InputStream classesStream = ClassLoader.getSystemClassLoader()
                .getResourceAsStream("commandsAndNamesOfClasses.config")) {
            properties = new Properties();
            properties.load(classesStream);
        }
    }

    /**
     * 
     * @param name name(symbol) of command
     * @return
     * @throws FactoryException
     */
    public Command createUnitByName(String name) throws FactoryException {

        Class newClass;
        try {
            if (!classes.containsKey(name)) {
                if (properties.getProperty(name) == null) {
                    return null;
                } else {
                    newClass = Class.forName(properties.getProperty(name));
                    classes.put(name, newClass);
                }
            } else {
                newClass = classes.get(name);
            }
            Object command = newClass.getDeclaredConstructor().newInstance();
            return (Command) command;
        } catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException
                | ClassNotFoundException | NoSuchMethodException e) {

            throw new FactoryException("Factory can't process string and get class object");
        }
    }
}