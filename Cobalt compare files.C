#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm> // For sorting

using namespace std;

// Struct to hold the data of each row
struct Row {
    long long col1;  // First column as long long
    double col2;     // Second column
    double col3;     // Third column
};

// Function to read a file into a vector of Rows
vector<Row> read_file(const string& filename) {
    vector<Row> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        exit(1);
    }

    Row row;
    while (file >> row.col1 >> row.col2 >> row.col3) {
        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to compare the two files with a given tolerance and show progress
void compare_files(const vector<Row>& file1_data, const vector<Row>& file2_data, long long tolerance, ofstream& output_file) {
    size_t i = 0, j = 0;
    size_t total_rows = file1_data.size() + file2_data.size();  // Total rows in both files
    size_t processed_rows = 0;

    while (i < file1_data.size() && j < file2_data.size()) {
        long long val1 = file1_data[i].col1;
        long long val2 = file2_data[j].col1;

        // Check if the difference is within tolerance
        if (abs(val1 - val2) <= tolerance) {
            // Now, check if the second column values are equal
            if (file1_data[i].col2 != file2_data[j].col2) {
                // Print error if second column values do not match
                output_file << "Error: Mismatch in second column values for "
                            << "File1_1: " << file1_data[i].col1 << ", File2_1: " << file2_data[j].col1 << endl;
            }

            // Output the matching data (first column within tolerance), with tabs instead of pipes
            output_file << file1_data[i].col1 << "\t"
                        << file1_data[i].col2 << "\t"
                        << file1_data[i].col3 << "\t"
                        << file2_data[j].col3 << endl;

            i++;
            j++;
        }
            // Move the pointer of the file with the smaller value
        else if (val1 < val2) {
            i++;
        } else {
            j++;
        }

        // Calculate and print progress every 100 rows processed
        processed_rows++;
        if (processed_rows % 100 == 0 || processed_rows == total_rows) {
            int progress = static_cast<int>((static_cast<double>(processed_rows) / total_rows) * 100);
            cout << "Progress: " << progress << "%" << endl;
        }
    }
}

int main() {
    // Updated file paths based on your request
    string file1_path = "ch0_filtered.txt";
    string file2_path = "ch1_filtered.txt";
    long long tolerance = 500000;  // Updated tolerance to 500000

    // Open output file to write results
    ofstream output_file("output.txt");

    if (!output_file.is_open()) {
        cerr << "Failed to open output file: output.txt" << endl;
        return 1;
    }

    // Read data from both files
    vector<Row> file1_data = read_file(file1_path);
    vector<Row> file2_data = read_file(file2_path);

    // Output header to the file with tabs separating columns
    output_file << "File1_1 (long long)\tFile1_2\tFile1_3\tFile2_3" << endl;

    // Compare the two files and print matching rows to the output file
    compare_files(file1_data, file2_data, tolerance, output_file);

    // Close output file
    output_file.close();

    cout << "Results have been written to output.txt" << endl;

    return 0;
}
