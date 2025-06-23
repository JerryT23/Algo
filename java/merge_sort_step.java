import java.io.*;
import java.util.*;

public class merge_sort_step {
    public static void main(String[] abc) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Enter the input target dataset size:");
        int inputFile = scanner.nextInt();
        String datasetFile = "dataset_" + inputFile + ".csv";

        System.out.println("Enter the start row:");
        int startRow = scanner.nextInt();

        System.out.println("Enter the end row:");
        int endRow = scanner.nextInt();
        scanner.close();

        String outputFile = String.format("merge_sort_step_%d_%d.txt", startRow, endRow);

        try {
            List<String> data = readCSV(datasetFile, startRow, endRow);

            try (BufferedWriter bw = new BufferedWriter(new FileWriter(outputFile))) {
                formatList(data, bw);
                mergeSort(data, bw, 0, data.size() - 1);
            }

            System.out.println("Sorting steps written to: " + outputFile);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    static List<String> readCSV(String filename, int start, int end) throws IOException {
        List<String> lines = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        int currentRow = 0;

        while ((line = br.readLine()) != null) {
            currentRow++;
            if (currentRow < start) continue;
            if (currentRow > end) break;
            lines.add(line.trim());
        }

        br.close();
        return lines;
    }

    static int extractInt(String s) {
        return Integer.parseInt(s.substring(0, s.indexOf(',')));
    }

    static void formatList(List<String> arr, BufferedWriter bw) throws IOException {
        bw.write("[");
        for (int i = 0; i < arr.size(); i++) {
            bw.write(arr.get(i));
            if (i != arr.size() - 1) {
                bw.write(", ");
            }
        }
        bw.write("]");
        bw.newLine();
    }

    static void mergeSort(List<String> arr, BufferedWriter bw, int left, int right) throws IOException {
        if (left >= right) return;

        int mid = left + (right - left) / 2;
        mergeSort(arr, bw, left, mid);
        mergeSort(arr, bw, mid + 1, right);
        merge(arr, left, mid, right);
        formatList(arr, bw);
    }

    static void merge(List<String> arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        List<String> L = new ArrayList<>(arr.subList(left, left + n1));
        List<String> R = new ArrayList<>(arr.subList(mid + 1, mid + 1 + n2));

        int i = 0, j = 0;
        int k = left;

        while (i < n1 && j < n2) {
            if (extractInt(L.get(i)) <= extractInt(R.get(j))) {
                arr.set(k++, L.get(i++));
            } else {
                arr.set(k++, R.get(j++));
            }
        }

        while (i < n1) arr.set(k++, L.get(i++));
        while (j < n2) arr.set(k++, R.get(j++));
    }
}
