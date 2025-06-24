#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::swap

// Structure to hold data entries, each with an integer ID and a string value.
struct DataEntry {
    int id;
    std::string value;

    // Overload the less-than operator to enable sorting DataEntry objects based on their 'id'.
    bool operator<(const DataEntry& other) const {
        return id < other.id;
    }
};

// Function to print a portion of the vector to an output file.
// It formats the output as [id/value, id/value, ...].
void printVectorToFile(std::ofstream& outFile, const std::vector<DataEntry>& vec, int low_idx, int high_idx) {
    outFile << "[";
    for (int i = low_idx; i <= high_idx; ++i) {
        outFile << vec[i].id << "/" << vec[i].value;
        if (i < high_idx) {
            outFile << ", ";
        }
    }
    outFile << "]\n";
}

// The core partitioning step of the Quick Sort algorithm.
// It rearranges elements such that elements smaller than the pivot are to its left,
// and larger elements are to its right.
// It also logs the state of the array after the pivot is placed.
int partition_step(std::vector<DataEntry> &vec, int low, int high, std::ofstream& outFile, int original_low, int original_high) {
    DataEntry pivot = vec[high]; // Choose the last element as the pivot
    int i = (low - 1);           // Index of the smaller element, indicating the right position of the pivot found so far

    // Traverse through all elements, comparing each with the pivot.
    for (int j = low; j <= high - 1; j++) {
        // If the current element is smaller than or equal to the pivot's ID,
        // increment 'i' and swap the elements.
        if (vec[j].id <= pivot.id) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    // Place the pivot element in its correct sorted position.
    std::swap(vec[i + 1], vec[high]);

    // Log the vector state AFTER the pivot is placed in its final position
    // and before recursive calls. The 'pi' represents the pivot index relative
    // to the *original full sub-array* that quick_sort_step was called with.
    outFile << "pi=" << (i + 1) - original_low << " "; 
    printVectorToFile(outFile, vec, original_low, original_high);

    return (i + 1); // Return the partitioning index
}

// The Quick Sort recursive function with logging steps.
// It sorts the sub-array specified by 'low' and 'high' indices.
// 'original_low' and 'original_high' are used to ensure correct logging
// of the entire sub-array being sorted in each step.
void quick_sort_step(std::vector<DataEntry> &vec, int low, int high, std::ofstream& outFile, int original_low, int original_high) {
    if (low < high) {
        // Perform the partitioning and get the pivot index.
        int pi = partition_step(vec, low, high, outFile, original_low, original_high);

        // Recursively sort the sub-arrays on either side of the pivot.
        quick_sort_step(vec, low, pi - 1, outFile, original_low, original_high);
        quick_sort_step(vec, pi + 1, high, outFile, original_low, original_high);
    }
}

// Function to read data from a CSV file.
// It reads data entries within a specified row range.
std::vector<DataEntry> readCsvData(const std::string& filename, int start_row, int end_row) {
    std::vector<DataEntry> data;
    std::ifstream file(filename); // Open the CSV file

    // Check if the file was opened successfully.
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return data; 
    }

    std::string line;
    int current_row = 0;
    // Read the file line by line.
    while (std::getline(file, line)) {
        current_row++;
        // Process lines only within the specified row range.
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
                    std::cerr << "Warning: Invalid integer format in row " << current_row << ": '" << id_str << "'" << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Warning: Integer out of range in row " << current_row << ": '" << id_str << "'" << std::endl;
                }
            }
        }
        // Stop reading once the end_row is exceeded.
        if (current_row > end_row) {
            break;
        }
    }
    file.close(); // Close the file.
    return data;
}

int main() {
    std::string dataset_filename;
    int start_row;
    int end_row;

    // Prompt the user for input.
    std::cout << "Enter the dataset filename (e.g., dataset_1000.csv): ";
    std::cin >> dataset_filename;

    std::cout << "Enter the starting row number: ";
    std::cin >> start_row;

    std::cout << "Enter the ending row number: ";
    std::cin >> end_row;

    // Validate the input rows.
    if (start_row <= 0 || end_row < start_row) {
        std::cerr << "Error: Invalid row numbers. Start row must be positive and less than or equal to end row." << std::endl;
        return 1;
    }

    // Construct the output filename based on the input rows.
    std::string output_filename = "quick_sort_step_" + std::to_string(start_row) + "_" + std::to_string(end_row) + ".txt";

    // Read data from the specified CSV file and row range.
    std::vector<DataEntry> data_to_sort = readCsvData(dataset_filename, start_row, end_row);
    if (data_to_sort.empty()) {
        std::cerr << "No data was read or file could not be opened. Exiting." << std::endl;
        return 1;
    }

    // Open the output file for logging quick sort steps.
    std::ofstream outFile(output_filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot create output file '" << output_filename << "'" << std::endl;
        return 1;
    }

    // Print the initial state of the vector to the output file.
    printVectorToFile(outFile, data_to_sort, 0, data_to_sort.size() - 1);

    // Perform quick sort and log each step.
    quick_sort_step(data_to_sort, 0, data_to_sort.size() - 1, outFile, 0, data_to_sort.size() - 1);

    outFile.close(); // Close the output file.
    std::cout << "Quick sort steps logged to " << output_filename << std::endl;

    return 0; // Indicate successful execution.
}
