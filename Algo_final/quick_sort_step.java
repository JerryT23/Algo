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
            return number + ":" + text;
        }
    }

  
    private static class Visualizer {
        private final BufferedWriter writer;

        Visualizer(BufferedWriter writer) {
            this.writer = writer;
        }

        public void logInitialState(List<Record> records) throws IOException {
            writer.write("Initial Array: " + formatRecords(records));
            writer.newLine();
            writer.newLine();
        }

        public void logPartition(int pivotIndex, List<Record> records) throws IOException {
            writer.write("Pivot chosen (at index " + pivotIndex + "). Array after partition: ");
            writer.write(formatRecords(records));
            writer.newLine();
        }
        
        public void logFinalState(List<Record> records) throws IOException {
            writer.newLine();
            writer.write("Final Sorted Array: " + formatRecords(records));
            writer.newLine();
        }

        private String formatRecords(List<Record> records) {
            // A modern, stream-based way to format the list
            return records.stream()
                          .map(Record::toString)
                          .collect(Collectors.joining(", ", "[", "]"));
        }
    }

    public static void main(String[] args) {
        String inputFile = "dataset_1000.csv"; 

        try (Scanner sc = new Scanner(System.in)) {
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

                visualizer.logInitialState(subset);
                quickSort(subset, 0, subset.size() - 1, visualizer);
                visualizer.logFinalState(subset);
                
                System.out.println("Successfully wrote visualization to: " + outputFile);
            }

        } catch (InputMismatchException e) {
            System.err.println("Error: Please enter valid integer numbers for rows.");
        } catch (FileNotFoundException e) {
            System.err.println("Error: Input file not found: " + inputFile);
        } catch (IOException e) {
            System.err.println("An unexpected I/O error occurred: " + e.getMessage());
        }
    }

    public static List<Record> readCsvSubset(String filename, int startRow, int endRow) throws IOException {
        List<Record> result = new ArrayList<>();
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String line;
            for (int i = 1; (line = br.readLine()) != null && i <= endRow; i++) {
                if (i >= startRow) {
                    String[] parts = line.split(",", 2);
                    if (parts.length < 2) continue; // Skip malformed lines gracefully
                    result.add(new Record(Integer.parseInt(parts[0].trim()), parts[1].trim()));
                }
            }
        }
        return result;
    }

    public static void quickSort(List<Record> arr, int low, int high, Visualizer visualizer) throws IOException {
        if (low < high) {
            int pi = partition(arr, low, high);
            visualizer.logPartition(pi, arr); 
            
            quickSort(arr, low, pi - 1, visualizer);
            quickSort(arr, pi + 1, high, visualizer);
        }
    }
    
    public static int partition(List<Record> arr, int low, int high) {
        int middle = (low + high) / 2;
        Record pivot;

        if (arr.get(middle).number < arr.get(low).number) Collections.swap(arr, low, middle);
        if (arr.get(high).number < arr.get(low).number) Collections.swap(arr, low, high);
        if (arr.get(high).number < arr.get(middle).number) Collections.swap(arr, middle, high);
        
        Collections.swap(arr, middle, high);
        pivot = arr.get(high);

        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr.get(j).number <= pivot.number) {
                i++;
                Collections.swap(arr, i, j);
            }
        }
        Collections.swap(arr, i + 1, high);
        return i + 1;
    }
}