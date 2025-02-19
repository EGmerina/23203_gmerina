package commands;

public class Next implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setPointer(context.getPointer() + 1);
    }
}
