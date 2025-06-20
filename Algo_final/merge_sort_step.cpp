#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

void writeVector(vector<string>& arr, ofstream& outfile)
{
    outfile << '[';
    for (int i = 0; i < arr.size(); i++)
        outfile << arr[i] << ", ";
    outfile <<']'<< endl;
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
    writeVector(arr,outfile);
}

int main()
{
    string line;
    vector<string> str;
    int n;
    ifstream infile("dataset_1000.csv");

    int startRow;
    int endRow;
    cout << "Select Start Row and End Row for Merge Sort" << endl;
    cout << "Enter Start Row: ";
    cin >> startRow;
    if (startRow < 1) {
        cout << "Start Row must be greater than or equal to 1." << endl;
        return -1;
    }
    cout << "Enter End Row: ";
    cin >> endRow;
    if (endRow < startRow) {
        cout << "End Row must be greater than or equal to Start Row." << endl;
        return -1;
    }
    std::ofstream outfile("merge_sort_step_"+ to_string(startRow)+ '_' + to_string(endRow) + ".txt");

    for (int i = 1; i < startRow; i++) {
        getline(infile, line); // Skip lines until startRow
    }

    for (int i = startRow; i <= endRow && getline(infile, line); i++) {
        stringstream ss(line);
        string numStr;
        if (getline(ss, numStr)) { 
            str.push_back(numStr);
        }
    }
    n = str.size();

    writeVector(str, outfile);

    mergeSort(str,outfile, 0, n - 1);
    outfile.close();
    return 0;
}