#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>    // For measuring execution time
#include <algorithm> // For std::swap
#include <limits>    // For std::numeric_limits

// Custom struct to hold integer and string data
struct DataEntry {
    int id;
    std::string value;

    // Overload the < operator for sorting based on id
    bool operator<(const DataEntry& other) const {
        return id < other.id; // Sort by integer ID
    }
};

// The core partitioning step of the Quick Sort algorithm.
// It rearranges elements such that elements smaller than the pivot are to its left,
// and larger elements are to its right.
int partition(std::vector<DataEntry> &vec, int low, int high) {
    DataEntry pivot = vec[high]; // Choose the last element as the pivot
    int i = (low - 1);           // Index of the smaller element, indicating the right position of the pivot found so far

    // Traverse through all elements, comparing each with the pivot.
    for (int j = low; j <= high - 1; j++) {
        // If the current element's ID is smaller than or equal to the pivot's ID,
        // increment 'i' and swap the elements.
        if (vec[j].id <= pivot.id) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    // Place the pivot element in its correct sorted position.
    std::swap(vec[i + 1], vec[high]);
    return (i + 1); // Return the partitioning index
}

// The Quick Sort recursive function.
// It sorts the sub-array specified by 'low' and 'high' indices.
void quick_sort(std::vector<DataEntry> &vec, int low, int high) {
    if (low < high) {
        // Perform the partitioning and get the pivot index.
        int pi = partition(vec, low, high);

        // Recursively sort the sub-arrays on either side of the pivot.
        quick_sort(vec, low, pi - 1);
        quick_sort(vec, pi + 1, high);
    }
}

// Function to read data from a CSV file.
// By default, it reads all rows.
// start_row and end_row parameters are for future flexibility but default to read all.
std::vector<DataEntry> readCsvData(const std::string& filename, int start_row = 1, int end_row = std::numeric_limits<int>::max()) {
    std::vector<DataEntry> data;
    std::ifstream file(filename); // Open the CSV file

    // Check if the file was opened successfully.
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return data; // Return empty vector to indicate failure
    }

    std::string line;
    int current_row = 0;
    // Read the file line by line.
    while (std::getline(file, line)) {
        current_row++;
        // Process lines only within the specified row range (1-indexed).
        // With default arguments, this will effectively read all rows.
        if (current_row >= start_row && current_row <= end_row) {
            std::stringstream ss(line); // Use stringstream to parse the line
            std::string id_str, value_str;

            // Attempt to extract ID and value separated by a comma.
            if (std::getline(ss, id_str, ',') && std::getline(ss, value_str)) {
                DataEntry entry;
                try {
                    // Convert ID string to integer.
                    entry.id = std::stoi(id_str);
                    entry.value = value_str;
                    data.push_back(entry); // Add the entry to the vector.
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Warning: Invalid integer format in row " << current_row << ": '" << id_str << "' - " << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Warning: Integer out of range in row " << current_row << ": '" << id_str << "' - " << e.what() << std::endl;
                }
            }
        }
        // If we've passed the end_row, stop reading to optimize performance for large files.
        // This condition is still useful if end_row is explicitly set to a finite value.
        if (current_row > end_row) {
            break;
        }
    }
    file.close(); // Close the file.
    return data;
}

// Function to write sorted data to a new CSV file.
void writeCsvData(const std::string& filename, const std::vector<DataEntry>& data) {
    std::ofstream file(filename); // Open the output file

    // Check if the file was opened successfully for writing.
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create output file '" << filename << "'" << std::endl;
        return;
    }

    // Write each data entry to the file in CSV format.
    for (const auto& entry : data) {
        file << entry.id << "," << entry.value << "\n";
    }
    file.close(); // Close the output file.
}

int main() {
    std::string input_filename;

    // Prompt the user for the input dataset filename.
    std::cout << "Enter the input dataset filename (e.g., dataset_1000.csv): ";
    std::cin >> input_filename;

    // Generate the output filename.
    // This logic attempts to append "_sorted.csv" or a number from the original name.
    std::string output_filename;
    size_t last_dot_pos = input_filename.find_last_of('.');
    size_t last_underscore_pos = input_filename.find_last_of('_');

    if (last_dot_pos != std::string::npos && last_underscore_pos != std::string::npos &&
        last_underscore_pos < last_dot_pos) {
        // Extract the number part between the last underscore and the dot
        std::string number_str = input_filename.substr(last_underscore_pos + 1, last_dot_pos - (last_underscore_pos + 1));
        output_filename = "quick_sort_" + number_str + ".csv";
    } else if (last_dot_pos != std::string::npos) {
        // If no underscore pattern, just append "_sorted" before the extension
        // Corrected to put "_sorted" before the extension
        output_filename = input_filename.substr(0, last_dot_pos) + "_sorted" + input_filename.substr(last_dot_pos);
    }
    else {
        // Fallback if no extension, just append "_sorted.csv"
        output_filename = input_filename + "_sorted.csv";
    }

    // Read data from the specified CSV file.
    // By calling readCsvData without start_row/end_row, it reads the entire file.
    // I/O time for reading is not included in the sorting algorithm's running time measurement.
    std::vector<DataEntry> data_to_sort = readCsvData(input_filename);
    if (data_to_sort.empty()) {
        std::cerr << "No data was read or the file could not be opened. Exiting." << std::endl;
        return 1; // Exit if no data was read.
    }

    // Start timer for measuring the sorting algorithm's execution time.
    auto start_time = std::chrono::high_resolution_clock::now();

    // Perform quick sort on the read data.
    quick_sort(data_to_sort, 0, data_to_sort.size() - 1);

    // End timer for the sorting algorithm.
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration of the sorting process in milliseconds.
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Write the sorted data to the output CSV file.
    // I/O time for writing is not included in the sorting algorithm's running time measurement.
    writeCsvData(output_filename, data_to_sort);

    // Print the running time in milliseconds and the output filename.
    std::cout << "Quick sort running time: " << duration_ms.count() << " ms" << std::endl;
    std::cout << "Sorted data saved to: " << output_filename << std::endl;

    return 0; // Indicate successful program execution.
}
