package commands;

public class Previous implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setDataPointer(context.getDataPointer() - 1);
    }
}