import java.io.*;
import java.util.*;

public class binary_search {
    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input  target dataset size:");
        int inputFile = scanner.nextInt();
        String datasetFile = "merge_sort_" + inputFile + ".csv";
        scanner.close();

        String outputFile = datasetFile.replace("merge_sort_", "binary_search_").replace(".csv", ".txt");

        try {
            List<Integer> data = loadData(datasetFile);
            int n = data.size();

            int bestTarget = data.get(n / 2);
            long bestTime = timeSearch(data, bestTarget);

            long totalAvgTime = 0;
            Random rand = new Random();
            for (int i = 0; i < 100; i++) {
                int target = data.get(rand.nextInt(n));
                totalAvgTime += timeSearch(data, target);
            }
            long avgTime = totalAvgTime / 100;

            int worstTarget = 1_500_000_000;
            long worstTime = timeSearch(data, worstTarget);

            BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile));
            bw.write("Best case (target=" + bestTarget + "): " + bestTime + " ms\n");
            bw.write("Average case (100 random targets): " + avgTime + " ms\n");
            bw.write("Worst case (target=" + worstTarget + "): " + worstTime + " ms\n");
            bw.close();

            System.out.println("Search timing written to: " + outputFile);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static List<Integer> loadData(String filename) throws IOException {
        List<Integer> list = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;

        while ((line = br.readLine()) != null) {
            String[] parts = line.split(",", 2);
            if (parts.length == 2) {
                list.add(Integer.parseInt(parts[0].trim()));
            }
        }
        br.close();
        return list;
    }

    static long timeSearch(List<Integer> list, int target) {
        long start = System.nanoTime();
        binarySearch(list, target);
        long end = System.nanoTime();
        return (end - start) / 1_000_000; // ms
    }

    static int binarySearch(List<Integer> list, int target) {
        int low = 0, high = list.size() - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            int midVal = list.get(mid);
            if (midVal == target)
                return mid;
            else if (midVal < target)
                low = mid + 1;
            else
                high = mid - 1;
        }
        return -1;
    }
}

