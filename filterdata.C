#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct DataRow {
    long long timestamp;
    int energy;
};

vector<DataRow> readFile(string filename) {
    vector<DataRow> data;
    ifstream file(filename);

    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return data;
    }

    long long timestamp;
    int energy;
    while (file >> timestamp >> energy) {
        data.push_back({timestamp, energy});
    }
    return data;
}

// Function to process the target vector (no threading)
void processTarget(const vector<long long>& reference, const vector<long long>& target, const vector<int>& energy, const vector<int>& energy3, int tolerance, const string& fileName) {
    // Buffer for accumulating results
    vector<string> resultBuffer;

    for (int j = 0; j < target.size(); j++) { // Iterate through the target
        long long mindiff = tolerance;
        bool trig = false;

        // Use binary search for efficient searching
        auto lower = lower_bound(reference.begin(), reference.end(), target[j] - tolerance);
        auto upper = upper_bound(reference.begin(), reference.end(), target[j] + tolerance);

        // Check the reference range around the target[j]
        for (auto it = lower; it != upper; ++it) {
            int i = it - reference.begin();
            long long diff = abs(reference[i] - target[j]);

            if (diff <= mindiff) {
                trig = true;
                mindiff = diff;
                // Write result when the best match is found
                resultBuffer.push_back(to_string(target[j]) + "\t" + to_string(energy3[j]) + "\t" + to_string(energy[i]) + "\n");
            }
        }
    }

    // Write the results at once
    if (!resultBuffer.empty()) {
        ofstream ofile(fileName, ios::app);
        if (!ofile) {
            cerr << "Error opening file for writing: " << fileName << endl;
            return;
        }

        for (const auto& line : resultBuffer) {
            ofile << line;
        }
    }
}

void findCloseValues(vector<long long> reference, vector<long long> target, vector<int> energy, vector<int> energy3, int tolerance) {
    string fileName = "filtered.txt";

    // Process target vector without multithreading
    processTarget(reference, target, energy, energy3, tolerance, fileName);
}

int main() {
    // File di input
    string file1 = "ch0.txt";
    string file3 = "ch3.txt";

    // Lettura dei file
    vector<DataRow> ch1 = readFile(file1);
    vector<DataRow> ch3 = readFile(file3);

    vector<long long> ts_ch1;
    vector<long long> ts_ch3;
    vector<int> energy_ch1;
    vector<int> energy_ch3;

    for (int i = 0; i < ch1.size(); i++) {
        ts_ch1.push_back(ch1[i].timestamp);
        energy_ch1.push_back(ch1[i].energy);
    }

    for (int i = 0; i < ch3.size(); i++) {
        ts_ch3.push_back(ch3[i].timestamp);
        energy_ch3.push_back(ch3[i].energy);
    }

    long long tolerance = 500000;

    cout << "Confronto con canale 1:" << endl;
    auto start = chrono::high_resolution_clock::now();
    findCloseValues(ts_ch1, ts_ch3, energy_ch1, energy_ch3, tolerance);
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::seconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " seconds." << endl;

    return 0;
}

