package commands;
import MyContext;

public interface Command {
    public void executeCommand(MyContext context);
}
