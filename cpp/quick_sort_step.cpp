#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm> 

struct DataEntry {
    int id;
    std::string value;

    bool operator<(const DataEntry& other) const {
        return id < other.id;
    }
};

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

int partition_step(std::vector<DataEntry> &vec, int low, int high, std::ofstream& outFile, int original_low, int original_high) {
    DataEntry pivot = vec[high]; 
    int i = (low - 1);          

    for (int j = low; j <= high - 1; j++) {
        if (vec[j].id <= pivot.id) {
            i++;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[high]);

    outFile << "pi=" << (i + 1) - original_low << " "; 
    printVectorToFile(outFile, vec, original_low, original_high);

    return (i + 1); 
}

void quick_sort_step(std::vector<DataEntry> &vec, int low, int high, std::ofstream& outFile, int original_low, int original_high) {
    if (low < high) {
        int pi = partition_step(vec, low, high, outFile, original_low, original_high);

        quick_sort_step(vec, low, pi - 1, outFile, original_low, original_high);
        quick_sort_step(vec, pi + 1, high, outFile, original_low, original_high);
    }
}

std::vector<DataEntry> readCsvData(const std::string& filename, int start_row, int end_row) {
    std::vector<DataEntry> data;
    std::ifstream file(filename); // Open the CSV file

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return data; 
    }

    std::string line;
    int current_row = 0;
    while (std::getline(file, line)) {
        current_row++;
        if (current_row >= start_row && current_row <= end_row) {
            std::stringstream ss(line); 
            std::string id_str, value_str;

            if (std::getline(ss, id_str, ',') && std::getline(ss, value_str)) {
                DataEntry entry;
                try {
                    entry.id = std::stoi(id_str);
                    entry.value = value_str;
                    data.push_back(entry); 
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Warning: Invalid integer format in row " << current_row << ": '" << id_str << "'" << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Warning: Integer out of range in row " << current_row << ": '" << id_str << "'" << std::endl;
                }
            }
        }
        if (current_row > end_row) {
            break;
        }
    }
    file.close();
    return data;
}

int main() {
    std::string dataset_filename;
    int start_row;
    int end_row;

    std::cout << "Enter the dataset filename (e.g., dataset_1000.csv): ";
    std::cin >> dataset_filename;

    std::cout << "Enter the starting row number: ";
    std::cin >> start_row;

    std::cout << "Enter the ending row number: ";
    std::cin >> end_row;

    if (start_row <= 0 || end_row < start_row) {
        std::cerr << "Error: Invalid row numbers. Start row must be positive and less than or equal to end row." << std::endl;
        return 1;
    }

    std::string output_filename = "quick_sort_step_" + std::to_string(start_row) + "_" + std::to_string(end_row) + ".txt";

    std::vector<DataEntry> data_to_sort = readCsvData(dataset_filename, start_row, end_row);
    if (data_to_sort.empty()) {
        std::cerr << "No data was read or file could not be opened. Exiting." << std::endl;
        return 1;
    }

    std::ofstream outFile(output_filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Cannot create output file '" << output_filename << "'" << std::endl;
        return 1;
    }

    printVectorToFile(outFile, data_to_sort, 0, data_to_sort.size() - 1);

    quick_sort_step(data_to_sort, 0, data_to_sort.size() - 1, outFile, 0, data_to_sort.size() - 1);

    outFile.close();
    std::cout << "Quick sort steps logged to " << output_filename << std::endl;

    return 0;
}
