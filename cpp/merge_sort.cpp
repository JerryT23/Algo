#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

using namespace std;

void writeVector(vector<string>& arr, ofstream& outfile) {
    for (int i = 0; i < arr.size(); i++) {
        outfile << arr[i] << endl;
    }
}

int extractInt(const string& s) {
    return stoi(s.substr(0, s.find(',')));
}

void merge(vector<string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<string> L(n1), R(n2);
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (extractInt(L[i]) <= extractInt(R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<string>& arr, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() {
    string line;
    string filename;
    vector<string> str;
    int n;
    cout << "Enter the file name (e.g., dataset_1000.csv): ";
    cin >> filename;
    std::string output_filename;
    size_t last_dot_pos = filename.find_last_of('.');
    size_t last_underscore_pos = filename.find_last_of('_');
    if (last_dot_pos != string::npos && last_underscore_pos != string::npos &&
        last_underscore_pos < last_dot_pos) {
        std::string number_str = filename.substr(last_underscore_pos + 1, last_dot_pos - (last_underscore_pos + 1));
        output_filename = "merge_sort_" + number_str + ".csv";
    } else {
        if (last_dot_pos != string::npos) {
            output_filename = filename.substr(0, last_dot_pos) + "_merge_sorted" + filename.substr(last_dot_pos);
        } else {
            output_filename = filename + "_merge_sorted.csv";
        }
    }
    ifstream infile(filename);
    ofstream outfile(output_filename);
    if (!infile.is_open()) {
        cerr << "Error: Could not open input file '" << filename << "'" << endl;
        return 1;
    }
    if (!outfile.is_open()) {
        cerr << "Error: Could not open output file '" << output_filename << "'" << endl;
        infile.close();
        return 1;
    }
    while (getline(infile, line)) {
        stringstream ss(line);
        string entry;
        if (getline(ss, entry)) {
            str.push_back(entry);
        }
    }
    n = str.size();
    auto start = chrono::high_resolution_clock::now();
    mergeSort(str, 0, n - 1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end - start;
    writeVector(str, outfile);
    cout << "Running time: " << duration.count() << " ms" << endl;
    cout << "Sorted data saved to: " << output_filename << std::endl;
    infile.close();
    outfile.close();
    return 0;
}

