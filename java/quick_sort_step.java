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
            return number + "/" + text;
        }
    }

    private static class Visualizer {
        private final BufferedWriter writer;

        Visualizer(BufferedWriter writer) {
            this.writer = writer;
        }

        public void logInitialState(List<Record> records) throws IOException {
            writer.write(formatRecords(records));
            writer.newLine();
        }

        public void logPartition(int pivotIndex, List<Record> records) throws IOException {
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
        try (Scanner sc = new Scanner(System.in)) {
            System.out.print("Enter dataset filename (e.g. dataset_1000.csv): ");
            String inputFile = sc.nextLine().trim();

            System.out.print("Enter start row: ");
            int startRow = sc.nextInt();
            System.out.print("Enter end row: ");
            int endRow = sc.nextInt();

            if (startRow <= 0 || endRow < startRow) {
                System.err.println("Error: Invalid row range. Please provide positive, sequential rows.");
                return;
            }

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
            for (int i = 1; (line = br.readLine()) != null; i++) {
                if (i >= startRow && i <= endRow) {
                    String[] parts = line.split(",", 2);
                    if (parts.length < 2) {
                        System.err.println("Warning: Skipping malformed line " + i + ": " + line);
                        continue; 
                    }
                    try {
                        result.add(new Record(Integer.parseInt(parts[0].trim()), parts[1].trim()));
                    } catch (NumberFormatException e) {
                        throw new NumberFormatException("Invalid number format in row " + i + ": " + parts[0].trim());
                    }
                }
                if (i > endRow) {
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
                insertionSort(arr, low, high);
            } else {
                int pi = partition(arr, low, high);
                visualizer.logPartition(pi, arr); 

                // Recursive calls for sub-arrays
                quickSort(arr, low, pi - 1, visualizer);
                quickSort(arr, pi + 1, high, visualizer);
            }
        }
    }

    public static int partition(List<Record> arr, int low, int high) {
        Record pivot = arr.get(high); 
        int i = (low - 1); 

        for (int j = low; j < high; j++) {
            if (arr.get(j).number <= pivot.number) {
                i++;
                Collections.swap(arr, i, j);
            }
        }
        Collections.swap(arr, i + 1, high); 
        return (i + 1); 
    }

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
