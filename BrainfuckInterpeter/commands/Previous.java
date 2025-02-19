package commands;

public class Previous implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setPointer(context.getPointer() - 1);
    }
}