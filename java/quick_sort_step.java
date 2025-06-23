import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class quick_sort_step {

    private static class Record {
        final int number;
        final String text;

        Record(int number, String text) {
            this.number = number;
            this.text = text;
        }

        @Override
        public String toString() {
            // Changed separator from ':' to '/' as requested
            return number + "/" + text;
        }
    }

    private static class Visualizer {
        private final BufferedWriter writer;

        Visualizer(BufferedWriter writer) {
            this.writer = writer;
        }

        public void logInitialState(List<Record> records) throws IOException {
            // Removed "Initial Array: " prefix and extra newLine as requested
            writer.write(formatRecords(records));
            writer.newLine(); // Keep one new line after initial state
        }

        public void logPartition(int pivotIndex, List<Record> records) throws IOException {
            // Modified to match C++ output format "pi=X " without extra prefixes
            writer.write("pi=" + pivotIndex + " ");
            writer.write(formatRecords(records));
            writer.newLine();
        }

        private String formatRecords(List<Record> records) {
            return records.stream()
                          .map(Record::toString)
                          .collect(Collectors.joining(", ", "[", "]"));
        }
    }

    public static void main(String[] args) {
        // Use try-with-resources for the Scanner to ensure it's closed automatically
        try (Scanner sc = new Scanner(System.in)) {
            System.out.print("Enter dataset filename (e.g. dataset_1000.csv): ");
            String inputFile = sc.nextLine().trim(); // Get filename interactively

            System.out.print("Enter start row: ");
            int startRow = sc.nextInt();
            System.out.print("Enter end row: ");
            int endRow = sc.nextInt();

            if (startRow <= 0 || endRow < startRow) {
                System.err.println("Error: Invalid row range. Please provide positive, sequential rows.");
                return;
            }

            // Automatically generate the output filename based on startRow and endRow
            String outputFile = "quick_sort_step_" + startRow + "_" + endRow + ".txt";

            try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
                Visualizer visualizer = new Visualizer(bw);
                List<Record> subset = readCsvSubset(inputFile, startRow, endRow);

                if (subset.isEmpty()) {
                    System.err.println("Error: No data read from the specified range in the input file, or file is empty/not found.");
                    return;
                }

                visualizer.logInitialState(subset);
                quickSort(subset, 0, subset.size() - 1, visualizer);

                System.out.println("Successfully wrote visualization to: " + outputFile);
            }
        } catch (InputMismatchException e) {
            System.err.println("Error: Please enter valid integer numbers for rows. " + e.getMessage());
        } catch (FileNotFoundException e) {
            System.err.println("Error: Input file not found: " + e.getMessage());
        } catch (IOException e) {
            System.err.println("An unexpected I/O error occurred: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Error: CSV file contains invalid number format in the ID column. " + e.getMessage());
        }
    }

    public static List<Record> readCsvSubset(String filename, int startRow, int endRow) throws IOException {
        List<Record> result = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            for (int i = 1; (line = br.readLine()) != null; i++) { // Loop through all lines
                if (i >= startRow && i <= endRow) { // Process only lines within the specified range
                    String[] parts = line.split(",", 2);
                    if (parts.length < 2) {
                        System.err.println("Warning: Skipping malformed line " + i + ": " + line);
                        continue; // Skip malformed lines gracefully
                    }
                    try {
                        result.add(new Record(Integer.parseInt(parts[0].trim()), parts[1].trim()));
                    } catch (NumberFormatException e) {
                        throw new NumberFormatException("Invalid number format in row " + i + ": " + parts[0].trim());
                    }
                }
                if (i > endRow) { // Stop reading once past the endRow
                    break;
                }
            }
        }
        return result;
    }

    public static void quickSort(List<Record> arr, int low, int high, Visualizer visualizer) throws IOException {
        if (low < high) {
            final int INSERTION_SORT_CUTOFF = 1; 
            
            if (high - low + 1 < INSERTION_SORT_CUTOFF) {
                // This block will now only be entered for sub-arrays of size 0 or 1
                insertionSort(arr, low, high); // Insertion sort handles these base cases effectively
            } else {
                int pi = partition(arr, low, high);
                visualizer.logPartition(pi, arr); // Log the array after partition and pivot placement

                // Recursive calls for sub-arrays
                quickSort(arr, low, pi - 1, visualizer);
                quickSort(arr, pi + 1, high, visualizer);
            }
        }
    }

    // Corrected to use Lomuto partition scheme with last element as pivot
    public static int partition(List<Record> arr, int low, int high) {
        Record pivot = arr.get(high); // Last element as pivot
        int i = (low - 1); // Index of smaller element

        for (int j = low; j < high; j++) { // Iterate from low up to (but not including) high
            if (arr.get(j).number <= pivot.number) {
                i++;
                Collections.swap(arr, i, j);
            }
        }
        Collections.swap(arr, i + 1, high); // Place pivot at its correct sorted position
        return (i + 1); // Return the pivot's final index
    }

    // Insertion Sort for small sub-arrays (part of hybrid Quick Sort)
    private static <T extends quick_sort_step.Record> void insertionSort(List<T> list, int low, int high) {
        for (int p = low + 1; p <= high; p++) {
            T tmp = list.get(p);
            int j;
            for (j = p; j > low && list.get(j - 1).number > tmp.number; j--) {
                list.set(j, list.get(j - 1));
            }
            list.set(j, tmp);
        }
    }
}
