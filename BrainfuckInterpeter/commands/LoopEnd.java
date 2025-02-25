package commands;

public class LoopEnd implements Command {
    @Override
    public void executeCommand(MyContext context) {
        if (context.getCurrentByte() != 0) {
            context.setInstructionPointer(context.getBorderOfLoop(context.getInstructionPointer()));
        }
    }
}
