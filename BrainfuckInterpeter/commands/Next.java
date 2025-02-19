package commands;

public class Next implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setDataPointer(context.getDataPointer() + 1);
    }
}
