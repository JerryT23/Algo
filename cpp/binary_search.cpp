#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <random>
using namespace std;

int extractInt(const string& s) {
    return stoi(s.substr(0, s.find(',')));
}

void binarySearch(vector<int>& vec, int x)
{
    int low = 0;
    int high = vec.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (vec[mid] == x)
            return ;
        if (vec[mid] < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
}

int main(void)
{
    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration;

    int n;
    string line;
    string filename;
    vector<int> vec;

    cout << "Enter the file name (e.g., merge_sort_1000000.csv): ";
    cin >> filename;

    int inputSize = stoi(filename.substr(11, filename.find('.') - 11 ));
    ifstream infile(filename);

    ofstream outfile("binary_search_"+filename.substr(11, filename.find('.') - 11 )+".txt");
    int average_case;
    int best_case;

    while (getline(infile, line)) {
        vec.push_back(extractInt(line));
    }

    n = vec.size();
    int worst_case = -1; 
    average_case = vec[rand() % n];
    best_case = vec[(n / 2) - 1];
    start = chrono::high_resolution_clock::now();

    for (int i = 0; i < inputSize; i++) 
        binarySearch(vec, worst_case);

    end = chrono::high_resolution_clock::now();
    duration = end - start;
    outfile << "Worst Case: " << worst_case << " took " << duration.count() << " ms\n";
    start = chrono::high_resolution_clock::now();

    for (int i = 0; i < inputSize; i++) 
        binarySearch(vec, average_case);

    end = chrono::high_resolution_clock::now();
    duration = end - start;
    outfile << "Average Case: " << average_case << " took " << duration.count() << " ms\n";
    start = chrono::high_resolution_clock::now();

    for (int i = 0; i < inputSize; i++) 
        binarySearch(vec, best_case);
        
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    outfile << "Best Case: " << best_case << " took " << duration.count() << " ms\n";
    return 0;
}