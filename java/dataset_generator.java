import java.io.*;
import java.util.*;

public class dataset_generator {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter dataset size: ");
        int size = scanner.nextInt();
        String outputFile = "dataset_" + size + ".csv";
        scanner.close();

        try {
            generateDataset(size, outputFile);
            System.out.println("Dataset generated: " + outputFile);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    static void generateDataset(int size, String filename) throws IOException {
        int maxValue = 1_000_000_000;

        if (size > maxValue) {
            throw new IllegalArgumentException("Size too large. Must be ≤ 1 billion.");
        }

        Set<Integer> uniqueIntegers = new LinkedHashSet<>();
        Random rand = new Random();

        while (uniqueIntegers.size() < size) {
            uniqueIntegers.add(rand.nextInt(maxValue) + 1);
        }

        BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
        for (int number : uniqueIntegers) {
            String randomStr = generateRandomString(5 + rand.nextInt(2));
            bw.write(number + "," + randomStr);
            bw.newLine();
        }
        bw.close();
    }

    static String generateRandomString(int length) {
        Random rand = new Random();
        StringBuilder sb = new StringBuilder(length);
        for (int i = 0; i < length; i++) {
            char c = (char) ('a' + rand.nextInt(26));
            sb.append(c);
        }
        return sb.toString();
    }
}
