package commands;

/**
 * If the byte at the data pointer is zero, then instead of moving the
 * instruction pointer forward to the next command, jump it forward to the
 * command after the matching ] command.
 */
public class LoopBegin implements Command {
    @Override
    public void executeCommand(MyContext context) {
        if (context.getCurrentByte() == 0) {
            context.setInstructionPointer(context.getBorderOfLoop(context.getInstructionPointer()));
        }
    }
}