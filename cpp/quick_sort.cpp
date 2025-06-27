#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>  
#include <algorithm> 
#include <limits>  

struct DataEntry {
    int id;
    std::string value;

    bool operator<(const DataEntry& other) const {
        return id < other.id;
    }
};

int partition(std::vector<DataEntry> &vec, int low, int high) {
    DataEntry pivot = vec[high]; 
    int i = (low - 1);           

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

std::vector<DataEntry> readCsvData(const std::string& filename, int start_row = 1, int end_row = std::numeric_limits<int>::max()) {
    std::vector<DataEntry> data;
    std::ifstream file(filename);

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
                    std::cerr << "Warning: Invalid integer format in row " << current_row << ": '" << id_str << "' - " << e.what() << std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Warning: Integer out of range in row " << current_row << ": '" << id_str << "' - " << e.what() << std::endl;
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

int main() {
    std::string input_filename;

    std::cout << "Enter the input dataset filename (e.g., dataset_1000.csv): ";
    std::cin >> input_filename;

    std::string output_filename;
    size_t last_dot_pos = input_filename.find_last_of('.');
    size_t last_underscore_pos = input_filename.find_last_of('_');

    if (last_dot_pos != std::string::npos && last_underscore_pos != std::string::npos &&
        last_underscore_pos < last_dot_pos) {
        std::string number_str = input_filename.substr(last_underscore_pos + 1, last_dot_pos - (last_underscore_pos + 1));
        output_filename = "quick_sort_" + number_str + ".csv";
    } else if (last_dot_pos != std::string::npos) {
        output_filename = input_filename.substr(0, last_dot_pos) + "_sorted" + input_filename.substr(last_dot_pos);
    }
    else {
        output_filename = input_filename + "_sorted.csv";
    }

    std::vector<DataEntry> data_to_sort = readCsvData(input_filename);
    if (data_to_sort.empty()) {
        std::cerr << "No data was read or the file could not be opened. Exiting." << std::endl;
        return 1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    quick_sort(data_to_sort, 0, data_to_sort.size() - 1);

    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    writeCsvData(output_filename, data_to_sort);

    std::cout << "Quick sort running time: " << duration_ms.count() << " ms" << std::endl;
    std::cout << "Sorted data saved to: " << output_filename << std::endl;

    return 0;
}
