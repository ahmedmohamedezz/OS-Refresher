public class Cpu {
    // compile using: javac Cpu.java
    // then run: java Cpu Hello

    // run different instances in background (using bash shell)
    // java Cpu A & java Cpu B & java Cpu C &

    public static void spin(int seconds) {
        // busy-wait (burn) the cpu for `seconds`
        long end = System.currentTimeMillis() + seconds * 1000;

        while (System.currentTimeMillis() < end) {
            // burn cpu
        }
    }

    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("usage: java cpu <string message>");
            System.exit(1);
        }

        String msg = args[0];
        while (true) {
            spin(1);
            System.err.println(msg);
        }
    }
}