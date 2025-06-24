#include <iostream>   // For input/output operations (std::cout, std::cin, std::cerr)
#include <vector>     // Not strictly used for the dataset itself, but good practice
#include <string>     // For string manipulation (std::string)
#include <fstream>    // For file input/output (std::ofstream)
#include <sstream>    // For string stream operations (not directly used here, but common)
#include <random>     // For random number generation (std::random_device, std::mt19937, std::uniform_int_distribution)
#include <set>        // For storing unique integers (std::set)
#include <algorithm>  // For algorithms like std::min/max (not strictly used here, but good practice)

// Function to generate a random string of a specified length
std::string generateRandomString(int length, std::mt19937& rng) {
    // Define the distribution for characters (lowercase 'a' to 'z')
    std::uniform_int_distribution<char> dist('a', 'z');
    std::string randomStr;
    randomStr.reserve(length); // Pre-allocate memory for efficiency

    for (int i = 0; i < length; ++i) {
        randomStr += dist(rng); // Append a random character
    }
    return randomStr;
}

// Function to generate a dataset with unique integer IDs and random strings
void generateDataset(int size, const std::string& filename) {
    // Maximum value for the integer IDs (1 billion as per original Java code)
    const int maxValue = 1'000'000'000;

    // Validate the requested size against the maximum value
    if (size > maxValue) {
        // Throw an exception if the size is too large
        throw std::invalid_argument("Size too large. Must be <= 1 billion.");
    }

    // Initialize random number generator
    // std::random_device provides a non-deterministic seed (if available)
    std::random_device rd;
    // std::mt19937 is a Mersenne Twister engine, a good general-purpose PRNG
    std::mt19937 rng(rd());

    // Use std::set to ensure all generated integers are unique
    // std::set automatically keeps elements sorted, which is fine for uniqueness here.
    std::set<int> uniqueIntegers;
    // Distribution for generating random integers from 1 to maxValue
    std::uniform_int_distribution<int> int_dist(1, maxValue);

    // Generate unique integers until the desired 'size' is reached
    while (uniqueIntegers.size() < size) {
        uniqueIntegers.insert(int_dist(rng)); // Insert random number into the set (duplicates are ignored)
    }

    // Open the output file for writing
    std::ofstream outputFile(filename);
    // Check if the file was opened successfully
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: Could not open file '" + filename + "' for writing.");
    }

    // Iterate through the unique integers and write them to the file with random strings
    // The order of elements from std::set will be sorted, which is fine for a dataset.
    for (int number : uniqueIntegers) {
        // Generate a random string with length between 5 and 6 (5 + 0 or 5 + 1)
        std::uniform_int_distribution<int> length_dist(5, 6);
        std::string randomStr = generateRandomString(length_dist(rng), rng);
        
        // Write the number and the random string to the file, separated by a comma
        outputFile << number << "," << randomStr << "\n";
    }

    outputFile.close(); // Close the output file
}

int main() {
    int size;
    std::cout << "Enter dataset size: ";
    std::cin >> size;

    // Construct the output filename, e.g., "dataset_1000.csv"
    std::string outputFile = "dataset_" + std::to_string(size) + ".csv";

    try {
        generateDataset(size, outputFile); // Call the dataset generation function
        std::cout << "Dataset generated: " << outputFile << std::endl;
    } catch (const std::invalid_argument& e) {
        // Catch specific exceptions for invalid arguments (e.g., size too large)
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Indicate an error
    } catch (const std::runtime_error& e) {
        // Catch specific exceptions for runtime errors (e.g., file open failure)
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Indicate an error
    } catch (const std::exception& e) {
        // Catch any other standard exceptions
        std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
        return 1; // Indicate an error
    }

    return 0; // Indicate successful execution
}
