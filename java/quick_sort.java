import java.io.*;
import java.util.*;

public class quick_sort {

    // A cutoff for switching to Insertion Sort. A typical value is between 5 and 15.
    private static final int INSERTION_SORT_CUTOFF = 10;

    static class Record {
        int number;
        String text;

        Record(int number, String text) {
            this.number = number;
            this.text = text;
        }

        @Override
        public String toString() {
            return number + "," + text;
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter dataset filename (e.g. dataset_1000.csv): ");
        String inputFile = sc.nextLine().trim();
        sc.close();

        try {
            List<Record> records = readCsv(inputFile);
            int n = records.size();
            String outputFile = "quick_sort_" + n + ".csv";
            
            // Define the sorting logic using a Comparator (this makes the sort reusable)
            Comparator<Record> byNumber = Comparator.comparingInt(r -> r.number);

            long startTime = System.nanoTime();
            quickSort(records, 0, n - 1, byNumber);
            long endTime = System.nanoTime();
            
            double durationInMs = (endTime - startTime) / 1e6;

            writeCsv(records, outputFile);

            System.out.printf("Sorted %d records using Quick Sort in %.6f seconds.\n", n, durationInMs);
            System.out.println("Output file: " + outputFile);

        } catch (IOException e) {
            System.err.println("Error processing file: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Error: CSV file contains invalid number format. " + e.getMessage());
        }
    }

    // This now uses try-with-resources for automatic closing and better error handling
    public static List<Record> readCsv(String filename) throws IOException {
        List<Record> records = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",", 2); // Split into exactly 2 parts
                records.add(new Record(Integer.parseInt(parts[0].trim()), parts[1].trim()));
            }
        }
        return records;
    }

    public static void writeCsv(List<Record> records, String filename) throws IOException {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(filename))) {
            for (Record r : records) {
                bw.write(r.toString());
                bw.newLine();
            }
        }
    }

    /**
     * The main Quick Sort driver method. It's now generic thanks to the Comparator.
     */
    public static <T> void quickSort(List<T> list, int low, int high, Comparator<? super T> c) {
        if (low + INSERTION_SORT_CUTOFF > high) {
            // For small subarrays, Insertion Sort is more efficient.
            insertionSort(list, low, high, c);
        } else {
            // Using median-of-three pivot selection to avoid worst-case O(n^2)
            int middle = (low + high) / 2;
            if (c.compare(list.get(middle), list.get(low)) < 0) Collections.swap(list, low, middle);
            if (c.compare(list.get(high), list.get(low)) < 0) Collections.swap(list, low, high);
            if (c.compare(list.get(high), list.get(middle)) < 0) Collections.swap(list, middle, high);

            // Place pivot at position high - 1
            Collections.swap(list, middle, high - 1);
            T pivot = list.get(high - 1);

            // Begin partitioning
            int i = low, j = high - 1;
            while (true) {
                while (c.compare(list.get(++i), pivot) < 0) {}
                while (c.compare(list.get(--j), pivot) > 0) {}
                if (i >= j) break;
                Collections.swap(list, i, j);
            }
            // Restore pivot
            Collections.swap(list, i, high - 1);

            quickSort(list, low, i - 1, c);
            quickSort(list, i + 1, high, c);
        }
    }
    
    /**
     * A simple insertion sort for small subarrays.
     */
    private static <T> void insertionSort(List<T> list, int low, int high, Comparator<? super T> c) {
        for (int p = low + 1; p <= high; p++) {
            T tmp = list.get(p);
            int j;
            for (j = p; j > low && c.compare(tmp, list.get(j - 1)) < 0; j--) {
                list.set(j, list.get(j-1));
            }
            list.set(j, tmp);
        }
    }
}