#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int extractInt(const string& s) {
    return stoi(s.substr(0, s.find(',')));
}

void binarySearch(vector<string> vec,ofstream& outfile, int low, int high, int x)
{
    while (low <= high) {
        int mid = low + (high - low) / 2;

        outfile << to_string(mid+1) << ": " << vec[mid] << endl; //row number = index number + 1
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

    outfile << to_string(-1);
}

// Driver code
int main(void)
{
    int x;
    int n;
    string line;
    string filename;
    vector<string> vec;
    cout << "Enter the element to search: ";
    cin >> x;
    cout << "Enter the file name(eg:dataset_1000.csv): ";
    cin >> filename;
    ifstream infile(filename);

    while (getline(infile, line)) {
        vec.push_back(line);
    }
    n = vec.size();

    ofstream outfile("binary_search_step_" + to_string(x) +".txt");

    binarySearch(vec,outfile, 0, n - 1, x);
    return 0;
}