package src.commands;

/**
 * Increment the data pointer by one (to point to the next cell to the right).
 */
public class Next implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setDataPointer(context.getDataPointer() + 1);
    }
}
