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

        outfile << to_string(mid+1) << ": " << vec[mid] << endl;
        if (extractInt(vec[mid]) == x)
            return ;
        if (extractInt(vec[mid]) < x)
            low = mid + 1;
        else
            high = mid - 1;
    }

    outfile << to_string(-1);
}

int main(void)
{
    int x;
    int n;
    string line;
    string filename;
    vector<string> vec;
    cout << "Enter the file name(e.g., quick_sort_1000.csv): ";
    cin >> filename;
    cout << "Enter the element to search: ";
    cin >> x;
    ifstream infile(filename);

    while (getline(infile, line)) {
        vec.push_back(line);
    }
    n = vec.size();

    ofstream outfile("binary_search_step_" + to_string(x) +".txt");

    binarySearch(vec,outfile, 0, n - 1, x);
    return 0;
}