package src;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Properties;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import src.commands.Command;
import src.myexceptions.FactoryException;

/**
 * give object of Command class by name of command string.
 */
public class Factory {

    private static final Logger logger = LogManager.getLogger(Factory.class);

    private final HashMap<String, Class> classes = new HashMap<>();
    private final Properties properties;

    public Factory() throws IOException {
        try (InputStream classesStream = ClassLoader.getSystemClassLoader()
                .getResourceAsStream("resources/commandsAndNamesOfClasses.config")) {
            properties = new Properties();
            properties.load(classesStream);
        }
        logger.info("Factory initialized successfully");
    }

    /**
     * 
     * @param name name(symbol) of command
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