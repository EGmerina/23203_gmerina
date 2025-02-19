package commands;

public class LoopBegin implements Command {
    @Override
    public void executeCommand(MyContext context) {
        if (context.getDataPointer() == 0) {
            /*
             * if (context.getInstructionPointer() == context.getLastCommandNumber()) {
             * byte nextcommand = 0;
             * try {
             * nextcommand = (byte) context.getSourceStream().read();
             * } catch (IOException e) {
             * 
             * e.printStackTrace();
             * }
             * Command curCommand = null;
             * try {
             * curCommand = Factory.getInstance().createUnitByName(new String(new byte[] {
             * nextcommand }));
             * } catch (ClassNotFoundException e) {
             * 
             * e.printStackTrace();
             * } catch (NoSuchMethodException e) {
             * 
             * e.printStackTrace();
             * } catch (InstantiationException e) {
             * 
             * e.printStackTrace();
             * } catch (IllegalArgumentException e) {
             * 
             * e.printStackTrace();
             * } catch (IllegalAccessException e) {
             * 
             * e.printStackTrace();
             * } catch (InvocationTargetException e) {
             * 
             * e.printStackTrace();
             * } catch (IOException e) {
             * 
             * e.printStackTrace();
             * }
             * context.add(curCommand);
             * while (nextcommand != -1 || curCommand.getClass().getName() == DoNothing) {
             * // и
             * }
             * } else {
             * context.setInstructionPointer(context.getLastCommandNumber());
             * }
             */
            // TODO просто сделай уже массив команд!!!
        }
        context.mark();
    }
}