import java.io.*;
import java.util.*;

public class merge_sort_step {
    static List<Element> steps = new ArrayList<>();

    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input  target dataset size:");
        int inputFile = scanner.nextInt();
        String datasetFile = "dataset_" + inputFile + ".csv";

        System.out.println("Enter the start row:");
        int startRow = scanner.nextInt();

        System.out.println("Enter the end row:");
        int endRow = scanner.nextInt();
        scanner.close();

        try {
            List<Element> data = readCSV(datasetFile, startRow, endRow);
            List<Element> toSort = new ArrayList<>(data);

            List<String> stepLogs = new ArrayList<>();
            mergeSort(toSort, stepLogs);

            String outputFile = String.format("merge_sort_step_%d_%d.txt", startRow, endRow);
            writeSteps(outputFile, stepLogs);
            System.out.println("Sorting steps written to: " + outputFile);
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

    static List<Element> readCSV(String filename, int start, int end) throws IOException {
        List<Element> list = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        int currentRow = 0;

        while ((line = br.readLine()) != null) {
            currentRow++;
            if (currentRow < start) continue;
            if (currentRow > end) break;

            String[] parts = line.split(",", 2);
            if (parts.length == 2) {
                int number = Integer.parseInt(parts[0].trim());
                String text = parts[1].trim();
                list.add(new Element(number, text));
            }
        }
        br.close();
        return list;
    }

    static void mergeSort(List<Element> list, List<String> stepLog) {
        if (list.size() <= 1) return;

        int mid = list.size() / 2;
        List<Element> left = new ArrayList<>(list.subList(0, mid));
        List<Element> right = new ArrayList<>(list.subList(mid, list.size()));

        mergeSort(left, stepLog);
        mergeSort(right, stepLog);

        list.clear();
        int i = 0, j = 0;
        while (i < left.size() && j < right.size()) {
            if (left.get(i).number <= right.get(j).number) {
                list.add(left.get(i++));
            } else {
                list.add(right.get(j++));
            }
        }
        while (i < left.size()) list.add(left.get(i++));
        while (j < right.size()) list.add(right.get(j++));

        stepLog.add(formatList(list));
    }

    static String formatList(List<Element> list) {
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < list.size(); i++) {
            sb.append(list.get(i));
            if (i != list.size() - 1) sb.append(", ");
        }
        sb.append("]");
        return sb.toString();
    }

    static void writeSteps(String filename, List<String> steps) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
        for (String step : steps) {
            bw.write(step);
            bw.newLine();
        }
        bw.close();
    }
}
