public class ThreadCounter {
    // compile using: javac ThreadCounter.java
    // then run: java ThreadCounter 100000

    static int counter = 0;
    static int loops;

    static class Worker implements Runnable {
        @Override
        public void run() {
            for (int i = 0; i < loops; ++i)
                counter += 1;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        if (args.length != 1) {
            System.err.println("usage: java ThreadCounter <value>");
            System.exit(1);
        }

        loops = Integer.parseInt(args[0]);
        System.out.println("Initial value: " + counter);

        Thread t1 = new Thread(new Worker());
        Thread t2 = new Thread(new Worker());

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println("Final value: " + counter);
    }
}