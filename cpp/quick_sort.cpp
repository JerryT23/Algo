#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono> 
#include <algorithm> 

// Custom struct to hold integer and string data
struct DataEntry {
    int id;
    std::string value;

    // Overload the < operator for sorting based on id
    bool operator<(const DataEntry& other) const {
        return id < other.id; // Sort by integer
    }
};

int partition(std::vector<DataEntry> &vec, int low, int high) {
    DataEntry pivot = vec[high]; // Last element as pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        if (vec[j].id <= pivot.id) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);
    return (i + 1);
}

void quick_sort(std::vector<DataEntry> &vec, int low, int high) {
    if (low < high) {
        int pi = partition(vec, low, high);
        quick_sort(vec, low, pi - 1);
        quick_sort(vec, pi + 1, high);
    }
}

// Function to read data from CSV
std::vector<DataEntry> readCsvData(const std::string& filename) {
    std::vector<DataEntry> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return data; // Return empty vector to indicate failure
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id_str, value_str;
        // Each row has 2 fields separated by a comma: integer and string.
        if (std::getline(ss, id_str, ',') && std::getline(ss, value_str)) {
            try {
                DataEntry entry;
                entry.id = std::stoi(id_str);
                entry.value = value_str;
                data.push_back(entry);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Invalid integer format in row: '" << line << "' - " << e.what() << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Warning: Integer out of range in row: '" << line << "' - " << e.what() << std::endl;
            }
        }
    }
    file.close();
    return data;
}

// Function to write sorted data to CSV
void writeCsvData(const std::string& filename, const std::vector<DataEntry>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create output file '" << filename << "'" << std::endl;
        return;
    }

    for (const auto& entry : data) {
        file << entry.id << "," << entry.value << "\n";
    }
    file.close();
}

int main(int argc, char* argv[]) {
    // Expected arguments: program name, input filename
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_dataset_filename>" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1]; // e.g., dataset_1000000.csv

    // Generate output filename by appending "(sorted)" before the extension
    std::string output_filename = input_filename;
    size_t dot_pos = output_filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        output_filename.insert(dot_pos, "(sorted)");
    } else {
        output_filename += "(sorted)"; // If no extension, just append
    }

    // Read data (I/O time should not be included in running time capture)
    std::vector<DataEntry> data_to_sort = readCsvData(input_filename);
    if (data_to_sort.empty()) {
        return 1; // Error message already printed by readCsvData
    }

    // Start timer for sorting algorithm
    auto start_time = std::chrono::high_resolution_clock::now();

    // Perform quick sort
    quick_sort(data_to_sort, 0, data_to_sort.size() - 1);

    // End timer for sorting algorithm
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end_time - start_time;

    // Write sorted data (I/O time should not be included in running time capture)
    writeCsvData(output_filename, data_to_sort);

    // Print the running time.
    std::cout << "Quick sort running time: " << duration.count() << " seconds" << std::endl;
    std::cout << "Sorted data saved to: " << output_filename << std::endl;

    return 0;
}