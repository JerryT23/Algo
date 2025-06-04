import java.util.Scanner;

public class main_app {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter dataset size: ");
        int size = scanner.nextInt();
        String datasetFile = "dataset_" + size + ".csv";
        System.out.println("Generating dataset...");
        dataset_generator.main(new String[]{String.valueOf(size)});

        System.out.println("Sorting dataset using Merge Sort...");
        merge_sort.main(new String[]{datasetFile});

        System.out.print("Enter start row for merge_sort_step: ");
        int startRow = scanner.nextInt();
        System.out.print("Enter end row for merge_sort_step: ");
        int endRow = scanner.nextInt();
        System.out.println("Generating merge sort step output...");
        merge_sort_step.main(new String[]{datasetFile, String.valueOf(startRow), String.valueOf(endRow)});

        String sortedFile = "merge_sort_" + size + ".csv";
        System.out.println("Running binary search performance tests...");
        binary_search.main(new String[]{sortedFile});

        System.out.print("Enter target number for binary_search_step: ");
        int target = scanner.nextInt();
        System.out.println("Performing binary search step-by-step...");
        binary_search_step.main(new String[]{sortedFile, String.valueOf(target)});

        System.out.println("All steps completed successfully.");

        scanner.close();
    }
}