import java.io.*;
import java.util.*;

public class binary_search {
    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input dataset filename (e.g., quick_sort_1000.csv):");
        String datasetFile = scanner.nextLine().trim();
        scanner.close();

        // --- Start: Modified logic for output filename generation ---
        String outputFileName;
        String baseFileName = datasetFile;
        int lastDotIndex = datasetFile.lastIndexOf(".");
        if (lastDotIndex != -1) {
            baseFileName = datasetFile.substring(0, lastDotIndex); // Get "quick_sort_1000" from "quick_sort_1000.csv"
        }

        String numberString = "";
        int lastUnderscoreIndex = baseFileName.lastIndexOf("_");
        if (lastUnderscoreIndex != -1 && lastUnderscoreIndex < baseFileName.length() - 1) {
            // Extract the part after the last underscore
            numberString = baseFileName.substring(lastUnderscoreIndex + 1);
            // Basic check to see if it's likely a number (can be improved with regex if needed)
            if (!numberString.matches("\\d+")) {
                numberString = ""; // If it's not purely digits, clear it
            }
        }

        if (!numberString.isEmpty()) {
            outputFileName = "binary_search_" + numberString + ".txt";
        } else {
            // Fallback for unexpected filename formats
            outputFileName = "binary_search_output.txt";
            System.err.println("Warning: Could not extract number from filename for output. Using default: " + outputFileName);
        }
        // --- End: Modified logic for output filename generation ---

        try {
            List<Integer> data = loadData(datasetFile);
            // It's crucial for Binary Search that the data is sorted.
            // Assuming the input CSV from previous quick sort example is already sorted.
            // If not, you might need to sort it here: Collections.sort(data);
            int n = data.size();

            // Handle empty data case
            if (n == 0) {
                System.out.println("No data loaded from " + datasetFile + ". Exiting.");
                return;
            }

            // Best Case: Target is the middle element
            int bestTarget = data.get(n / 2);
            double bestTime = timeSearchN(data, bestTarget, n);

            // Average Case: 100 random targets
            double totalAvgTime = 0;
            Random rand = new Random();
            for (int i = 0; i < 100; i++) {
                int target = data.get(rand.nextInt(n)); // Pick a random existing element
                totalAvgTime += timeSearchN(data, target, n);
            }
            double avgTime = totalAvgTime / 100;

            // Worst Case: Target not in the list (value outside the range, or at an edge)
            // Choose a value clearly outside the range of typical integer IDs to guarantee not found.
            // Assuming IDs are positive, a very large negative or positive number might work.
            // For robustness, consider data.get(0) - 1 or data.get(n-1) + 1 if actual range is unknown.
            int worstTarget = 1_500_000_000; // A large number unlikely to be in typical datasets
            if (n > 0) {
                 // A value guaranteed not to be in the list if the list elements are unique and ordered.
                worstTarget = data.get(n-1) + 1;
            }
            double worstTime = timeSearchN(data, worstTarget, n);

            // Write results to the output file
            BufferedWriter bw = new BufferedWriter(new FileWriter(outputFileName));
            bw.write(String.format("Best case (target=%d): %.4f ms\n", bestTarget, bestTime));
            bw.write(String.format("Average case (100 random targets): %.4f ms\n", avgTime));
            bw.write(String.format("Worst case (target=%d): %.4f ms\n", worstTarget, worstTime));
            bw.close();

            System.out.println("Search timing written to: " + outputFileName);
        } catch (IOException e) {
            System.err.println("Error reading or writing file: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("Error parsing number from file: " + e.getMessage());
        } catch (Exception e) {
            System.err.println("An unexpected error occurred: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Loads integer IDs from the first column of a CSV file.
     * Assumes each line contains an integer followed by a comma and then a string.
     * @param filename The path to the CSV file.
     * @return A list of integers (IDs).
     * @throws IOException If there's an error reading the file.
     * @throws NumberFormatException If an ID cannot be parsed as an integer.
     */
    static List<Integer> loadData(String filename) throws IOException {
        List<Integer> list = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;

        while ((line = br.readLine()) != null) {
            String[] parts = line.split(",", 2); // Split only on the first comma
            if (parts.length == 2) {
                list.add(Integer.parseInt(parts[0].trim())); // Parse the ID from the first part
            } else {
                System.err.println("Skipping malformed line in CSV: " + line);
            }
        }
        br.close();
        return list;
    }

    /**
     * Times how long it takes to perform binary search 'n' times for a given target.
     * The timing is for 'n' searches, not a single search.
     * @param list The list to search in.
     * @param target The integer target to search for.
     * @param n The number of times to repeat the search for timing.
     * @return The total time in milliseconds for 'n' searches.
     */
    static double timeSearchN(List<Integer> list, int target, int n) {
        long start = System.nanoTime();
        for (int i = 0; i < n; i++) { // Repeating 'n' times for measurable duration
            binarySearch(list, target);
        }
        long end = System.nanoTime();
        // Convert nanoseconds to milliseconds
        return (end - start) / 1_000_000.0;
    }

    /**
     * Performs a standard iterative binary search on a sorted list of integers.
     * @param list The sorted list to search.
     * @param target The integer value to find.
     * @return The index of the target if found, otherwise -1.
     */
    static int binarySearch(List<Integer> list, int target) {
        int low = 0;
        int high = list.size() - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2; // Calculate mid to prevent overflow
            int midVal = list.get(mid);

            if (midVal == target) {
                return mid; // Target found
            } else if (midVal < target) {
                low = mid + 1; // Target is in the right half
            } else {
                high = mid - 1; // Target is in the left half
            }
        }
        return -1; // Target not found
    }
}
