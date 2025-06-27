import java.io.*;
import java.util.*;

public class binary_search {
    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input dataset filename (e.g., quick_sort_1000.csv):");
        String datasetFile = scanner.nextLine().trim();
        scanner.close();

        String outputFileName;
        String baseFileName = datasetFile;
        int lastDotIndex = datasetFile.lastIndexOf(".");
        if (lastDotIndex != -1) {
            baseFileName = datasetFile.substring(0, lastDotIndex);
        }

        String numberString = "";
        int lastUnderscoreIndex = baseFileName.lastIndexOf("_");
        if (lastUnderscoreIndex != -1 && lastUnderscoreIndex < baseFileName.length() - 1) {
            numberString = baseFileName.substring(lastUnderscoreIndex + 1);
            if (!numberString.matches("\\d+")) {
                numberString = "";
            }
        }

        if (!numberString.isEmpty()) {
            outputFileName = "binary_search_" + numberString + ".txt";
        } else {
            outputFileName = "binary_search_output.txt";
            System.err.println("Warning: Could not extract number from filename for output. Using default: " + outputFileName);
        }

        try {
            List<Integer> data = loadData(datasetFile);
            int n = data.size();

            // Handle empty data case
            if (n == 0) {
                System.out.println("No data loaded from " + datasetFile + ". Exiting.");
                return;
            }

            // Best Case: Target is the middle element
            int bestTarget = data.get((n / 2)-1);
            double bestTime = timeSearchN(data, bestTarget, n);

            // Average Case: 100 random targets
            double totalAvgTime = 0;
            Random rand = new Random();
            for (int i = 0; i < 100; i++) {
                int target = data.get(rand.nextInt(n)); // Pick a random existing element
                totalAvgTime += timeSearchN(data, target, n);
            }
            double avgTime = totalAvgTime / 100;

            int worstTarget = 1_500_000_000;
            if (n > 0) {
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
     * @param list The list to search in.
     * @param target The integer target to search for.
     * @param n The number of times to repeat the search for timing.
     * @return The total time in milliseconds for 'n' searches.
     */
    static double timeSearchN(List<Integer> list, int target, int n) {
        long start = System.nanoTime();
        for (int i = 0; i < n; i++) { 
            binarySearch(list, target);
        }
        long end = System.nanoTime();
        // Convert nanoseconds to milliseconds
        return (end - start) / 1_000_000.0;
    }

    /**
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
