#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
using namespace std;

void writeVector(vector<string>& arr, ofstream& outfile)
{
    for (int i = 0; i < arr.size(); i++)
        outfile << arr[i]<< endl;
}

int extractInt(const string& s) {
    return stoi(s.substr(0, s.find(',')));
}

void merge(vector<string>& arr, int left, 
                     int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // temp vectors to hold the split parts
    vector<string> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;
    
    while (i < n1 && j < n2) {
        if (extractInt(L[i]) <= extractInt(R[j])) {
            arr[k] = L[i];
            i++;
        }
        else {
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

void mergeSort(vector<string>& arr, ofstream& outfile, int left, int right)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, outfile, left, mid);
    mergeSort(arr, outfile, mid + 1, right);
    merge(arr, left, mid, right);
}

int main()
{
    string line;
    vector<string> str;
    int n;
    ifstream infile("dataset_1000.csv");
    std::ofstream outfile("merge_sort_1000.csv ");

    while (getline(infile, line)) {
        stringstream ss(line);
        string numStr;
        if (getline(ss, numStr)) { 
            str.push_back(numStr);
        }
    }
    n = str.size();

    auto start = chrono::high_resolution_clock::now();
    mergeSort(str,outfile, 0, n - 1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end - start;
    writeVector(str, outfile);
    outfile << "Running time: " << duration.count() << " ms";
    outfile.close();
    return 0;
}