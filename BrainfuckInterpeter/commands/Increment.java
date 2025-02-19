package commands;

public class Increment implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setCurrentByte((byte) (context.getCurrentByte() + 1));
    }
}
