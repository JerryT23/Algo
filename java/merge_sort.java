import java.io.*;
import java.util.*;

public class merge_sort {
    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input target dataset file name:(e.g. dataset_1000.csv)");
        String datasetFile = scanner.nextLine().trim();
        scanner.close();

        try {
            List<Element> data = readCSV(datasetFile);

            long startTime = System.nanoTime();
            mergeSort(data);
            long endTime = System.nanoTime();

            String outputFile = getSortedFilename(datasetFile);
            writeCSV(outputFile, data);

            long elapsedMillis = (endTime - startTime) / 1_000_000;
            System.out.println("Merge sort completed. Time taken: " + elapsedMillis + " ms");
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
            return number + "," + text;
        }
    }

    static List<Element> readCSV(String filename) throws IOException {
        List<Element> list = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;

        while ((line = br.readLine()) != null) {
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

    static void writeCSV(String filename, List<Element> data) throws IOException {
        BufferedWriter bw = new BufferedWriter(new FileWriter(filename));
        for (Element e : data) {
            bw.write(e.toString());
            bw.newLine();
        }
        bw.close();
    }

    static void mergeSort(List<Element> list) {
        if (list.size() <= 1) return;

        int mid = list.size() / 2;
        List<Element> left = new ArrayList<>(list.subList(0, mid));
        List<Element> right = new ArrayList<>(list.subList(mid, list.size()));

        mergeSort(left);
        mergeSort(right);

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
    }

    static String getSortedFilename(String datasetFile) {
        String base = datasetFile.contains(".") ? datasetFile.substring(0, datasetFile.lastIndexOf('.')) : datasetFile;
        return base.replace("dataset", "merge_sort") + ".csv";
    }
}
