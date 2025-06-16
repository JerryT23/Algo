import java.io.*;
import java.util.*;

public class binary_search_step {
    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input  target dataset size:");
        int inputFile = scanner.nextInt();
        String datasetFile = "merge_sort_" + inputFile + ".csv";

        System.out.print("Enter target number for binary_search_step: ");
        int target = scanner.nextInt();
        scanner.close();
        
        String outputFile = String.format("binary_search_step_%d.txt", target);

        try {
            List<Element> data = loadData(datasetFile);
            List<String> logSteps = new ArrayList<>();

            int resultIndex = binarySearchWithSteps(data, target, logSteps);

            if (resultIndex == -1) {
                logSteps.add("-1");
            }

            BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile));
            for (String line : logSteps) {
                bw.write(line);
                bw.newLine();
            }
            bw.close();

            System.out.println("Search steps written to: " + outputFile);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static class Element {
        int number;
        String text;

        Element(int number, String text) {
            this.number = number;
            this.text = text;
        }

        @Override
        public String toString() {
            return number + "/" + text;
        }
    }

    static List<Element> loadData(String filename) throws IOException {
        List<Element> list = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = br.readLine()) != null) {
            String[] parts = line.split(",", 2);
            if (parts.length == 2) {
                int num = Integer.parseInt(parts[0].trim());
                String txt = parts[1].trim();
                list.add(new Element(num, txt));
            }
        }
        br.close();
        return list;
    }

    static int binarySearchWithSteps(List<Element> list, int target, List<String> log) {
        int low = 0;
        int high = list.size() - 1;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            Element midElement = list.get(mid);
            log.add(mid + ": " + midElement);

            if (midElement.number == target)
                return mid;
            else if (midElement.number < target)
                low = mid + 1;
            else
                high = mid - 1;
        }

        return -1;
    }
}
