import java.io.*;

public class DataGenerator {

    public static void main(String args[]) {
        boolean cont = true;

        try {
            DataInputStream din = new DataInputStream(new FileInputStream("onepop.stt"));
            PrintStream dout = new PrintStream(new FileOutputStream("slow.stt"));

            while(cont) {
                String data = din.readLine();
                if (data == null) {
                    cont = false;
                }
                else {
                    dout.println(data);
                    Thread.currentThread().sleep(1000);
                }
            }
        }
        catch (IOException e) {
            System.out.println("Blah");
        }
        catch (InterruptedException i) {
            System.out.println("Bluh");
        }
    }
}
