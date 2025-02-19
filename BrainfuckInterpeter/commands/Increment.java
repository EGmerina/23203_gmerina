package commands;

public class Increment implements Command {
    @Override
    public void executeCommand(MyContext context){
        System.out.println("+");
    }
}
