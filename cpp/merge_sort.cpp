#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>   // For measuring execution time
#include <sstream>  // For string stream operations

// Using namespace std for brevity, common in competitive programming/smaller scripts
using namespace std;

// Function to write the contents of a vector of strings to an output file.
// Each string is written on a new line.
void writeVector(vector<string>& arr, ofstream& outfile) {
    for (int i = 0; i < arr.size(); i++) {
        outfile << arr[i] << endl;
    }
}

// Function to extract the integer ID from a string in "ID,Value" format.
// It finds the comma and converts the substring before it to an integer.
int extractInt(const string& s) {
    // stoi converts string to integer.
    // s.substr(0, s.find(',')) gets the part of the string before the first comma.
    return stoi(s.substr(0, s.find(',')));
}

// Merges two sorted sub-arrays back into a single sorted array.
// This is a core component of the Merge Sort algorithm.
void merge(vector<string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1; // Size of the left sub-array
    int n2 = right - mid;    // Size of the right sub-array

    // Temporary vectors to hold the split parts.
    // L for the left part, R for the right part.
    vector<string> L(n1), R(n2);

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // Initial indices of first, second, and merged sub-arrays
    int i = 0; // Initial index of first sub-array (L)
    int j = 0; // Initial index of second sub-array (R)
    int k = left; // Initial index of merged array (arr)
    
    // Merge the temporary arrays back into arr[left..right]
    while (i < n1 && j < n2) {
        // Compare elements based on their integer IDs
        if (extractInt(L[i]) <= extractInt(R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++; // Move to the next position in the merged array
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// The main recursive function that implements the Merge Sort algorithm.
// It divides the array into two halves, recursively sorts them, and then merges them.
// The 'outfile' parameter was removed as it's not needed for the sorting logic itself.
void mergeSort(vector<string>& arr, int left, int right) {
    // Base case: if the sub-array has 0 or 1 element, it's already sorted.
    if (left >= right) {
        return;
    }

    // Calculate the middle point to divide the array into two halves
    int mid = left + (right - left) / 2;

    // Recursively sort the first and second halves
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // Merge the sorted halves
    merge(arr, left, mid, right);
}

// Main function where the program execution begins.
int main() {
    string line;        // To read each line from the input file
    string filename;    // Stores the input filename provided by the user
    vector<string> str; // Stores the data read from the file
    int n;              // Stores the number of elements in the data vector

    cout << "Enter the file name (e.g., dataset_1000.csv): ";
    cin >> filename;

    // --- Start of robust output filename generation logic ---
    // This logic extracts the numeric part from the input filename
    // (e.g., "1000" from "dataset_1000.csv") and constructs the output filename
    // with "merge_sort_" prefix.
    std::string output_filename;
    size_t last_dot_pos = filename.find_last_of('.');
    size_t last_underscore_pos = filename.find_last_of('_');

    // Check if both a dot (for extension) and an underscore (for number prefix) exist,
    // and the underscore comes before the dot.
    if (last_dot_pos != string::npos && last_underscore_pos != string::npos &&
        last_underscore_pos < last_dot_pos) {
        // Extract the number part from the input filename.
        // For "dataset_1000.csv", this extracts "1000".
        std::string number_str = filename.substr(last_underscore_pos + 1, last_dot_pos - (last_underscore_pos + 1));
        // Construct the output filename using "merge_sort_" prefix.
        output_filename = "merge_sort_" + number_str + ".csv";
    } else {
        // Fallback for cases where the filename format is not "prefix_NUMBER.extension".
        // It appends "_merge_sorted.csv" or similar based on whether an extension exists.
        if (last_dot_pos != string::npos) {
            output_filename = filename.substr(0, last_dot_pos) + "_merge_sorted" + filename.substr(last_dot_pos);
        } else {
            output_filename = filename + "_merge_sorted.csv";
        }
    }
    // --- End of robust output filename generation logic ---

    // Open the input file for reading.
    ifstream infile(filename);
    // Open the output file for writing, using the dynamically generated filename.
    ofstream outfile(output_filename);

    // Check if the input file was opened successfully.
    if (!infile.is_open()) {
        cerr << "Error: Could not open input file '" << filename << "'" << endl;
        return 1; // Indicate an error
    }
    // Check if the output file was opened successfully.
    if (!outfile.is_open()) {
        cerr << "Error: Could not open output file '" << output_filename << "'" << endl;
        infile.close(); // Close input file before exiting
        return 1; // Indicate an error
    }

    // Read data line by line from the input file.
    while (getline(infile, line)) {
        stringstream ss(line);
        string entry;
        // Read the entire line as a string (assuming it's in "ID,Value" format)
        if (getline(ss, entry)) { 
            str.push_back(entry); // Add the line to the vector
        }
    }
    n = str.size(); // Get the total number of elements to sort

    // Start measuring the execution time of the merge sort algorithm.
    auto start = chrono::high_resolution_clock::now();
    
    // Call the mergeSort function to sort the vector.
    // The 'outfile' parameter is no longer passed to mergeSort.
    mergeSort(str, 0, n - 1);
    
    // Stop measuring the execution time.
    auto end = chrono::high_resolution_clock::now();
    
    // Calculate the duration of the sorting process in milliseconds.
    chrono::duration<double, std::milli> duration = end - start;
    
    // Write the sorted data to the output file.
    writeVector(str, outfile);
    
    // Print the running time and the output filename to the console.
    cout << "Running time: " << duration.count() << " ms" << endl;
    cout << "Sorted data saved to: " << output_filename << std::endl;

    // Close the input and output files.
    infile.close();
    outfile.close();
    
    return 0; // Indicate successful program execution
}
