package commands;

public class LoopEnd implements Command {
    @Override
    public void executeCommand(MyContext context) {
        if (context.getDataPointer() == 0) {
            context.deleteLoop();
            return;
        }
        context.reset();
    }
}
