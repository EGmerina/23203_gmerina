package src.commands;

public interface Command {
    /**
     * @param context contains nessesary data for execution commands
     */
    public void executeCommand(MyContext context);
}
