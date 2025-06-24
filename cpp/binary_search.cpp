#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
using namespace std;

int extractInt(const string& s) {
    return stoi(s.substr(0, s.find(',')));
}

void binarySearch(vector<string> vec,ofstream& outfile, int low, int high, int x)
{
    while (low <= high) {
        int mid = low + (high - low) / 2;

        // Check if x is == mid
        if (extractInt(vec[mid]) == x)
            return ;

        // If x greater, go right half
        if (extractInt(vec[mid]) < x)
            low = mid + 1;
        // If x is smaller, go left half
        else
            high = mid - 1;
    }
}

// Driver code
int main(void)
{
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration;
    int n;
    string line;
    string filename;
    vector<string> vec;
    cout << "Enter the file name (e.g., merge_sort_1000000.csv): ";
    cin >> filename;
    int inputSize = stoi(filename.substr(11, 7)); // Extracting the input size from the filename
    ifstream infile(filename);
    ofstream outfile("binary_search_"+filename.substr(11, 7)+".txt");
    int worst_case = -1; 
    int average_case = 988166268;
    int best_case;

    while (getline(infile, line)) {
        vec.push_back(line);
    }
    n = vec.size();
    best_case = extractInt(vec[n / 2]); // Assuming the best case is the middle element in a sorted array


    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < inputSize; i++) 
        binarySearch(vec,outfile, 0, n - 1, worst_case);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    outfile << "Worst Case: " << worst_case << " took " << duration.count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < inputSize; i++) 
        binarySearch(vec,outfile, 0, n - 1, average_case);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    outfile << "Average Case: " << average_case << " took " << duration.count() << " ms\n";

    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < inputSize; i++) 
        binarySearch(vec,outfile, 0, n - 1, best_case);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    outfile << "Best Case: " << best_case << " took " << duration.count() << " ms\n";
    return 0;
}