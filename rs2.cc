#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;

// Function prototypes
void readInputFile(const string &filename, vector<vector<double>> &utilityMatrix, double &minRating, double &maxRating);
double pearsonCorrelation(const vector<double> &a, const vector<double> &b);
double cosineSimilarity(const vector<double> &a, const vector<double> &b);
double euclideanDistance(const vector<double> &a, const vector<double> &b);
vector<pair<int, double>> findNeighbors(const vector<vector<double>> &utilityMatrix, int userIndex, int itemIndex, int k, const string &metric);
double predictSimple(const vector<vector<double>> &utilityMatrix, int userIndex, int itemIndex, const vector<pair<int, double>> &neighbors);
double predictDiffWithMean(const vector<vector<double>> &utilityMatrix, int userIndex, int itemIndex, const vector<pair<int, double>> &neighbors);
void printMatrix(const vector<vector<double>> &matrix);

int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    vector<vector<double>> utilityMatrix;
    double minRating, maxRating;
    readInputFile(filename, utilityMatrix, minRating, maxRating);

    string metric;
    cout << "Enter the similarity metric (pearson/cosine/euclidean): ";
    cin >> metric;

    int k;
    cout << "Enter the number of neighbors: ";
    cin >> k;

    string predictionType;
    cout << "Enter the type of prediction (simple/mean): ";
    cin >> predictionType;

    vector<vector<double>> predictedMatrix = utilityMatrix;

    for (size_t userIndex = 0; userIndex < utilityMatrix.size(); ++userIndex) {
        for (size_t itemIndex = 0; itemIndex < utilityMatrix[userIndex].size(); ++itemIndex) {
            if (utilityMatrix[userIndex][itemIndex] == -1) {
                vector<pair<int, double>> neighbors = findNeighbors(utilityMatrix, userIndex, itemIndex, k, metric);
                double prediction;
                if (predictionType == "simple") {
                    prediction = predictSimple(utilityMatrix, userIndex, itemIndex, neighbors);
                } else if (predictionType == "mean") {
                    prediction = predictDiffWithMean(utilityMatrix, userIndex, itemIndex, neighbors);
                } else {
                    cerr << "Invalid prediction type!" << endl;
                    return 1;
                }
                predictedMatrix[userIndex][itemIndex] = prediction;
            }
        }
    }

    cout << "Predicted Utility Matrix:" << endl;
    printMatrix(predictedMatrix);

    return 0;
}

void readInputFile(const string &filename, vector<vector<double>> &utilityMatrix, double &minRating, double &maxRating) {
    ifstream file(filename);
    if (!file) {
        cerr << "Could not open the file!" << endl;
        exit(1);
    }

    string line;
    getline(file, line);
    minRating = stod(line);

    getline(file, line);
    maxRating = stod(line);

    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        string value;
        while (ss >> value) {
            if (value == "-") {
                row.push_back(-1);
            } else {
                row.push_back(stod(value));
            }
        }
        utilityMatrix.push_back(row);
    }
    file.close();
}

double pearsonCorrelation(const vector<double> &a, const vector<double> &b) {
    vector<double> commonA, commonB;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != -1 && b[i] != -1) {
            commonA.push_back(a[i]);
            commonB.push_back(b[i]);
        }
    }
    if (commonA.empty()) return 0.0;

    double meanA = accumulate(commonA.begin(), commonA.end(), 0.0) / commonA.size();
    double meanB = accumulate(commonB.begin(), commonB.end(), 0.0) / commonB.size();
    double numerator = 0.0, denomA = 0.0, denomB = 0.0;
    for (size_t i = 0; i < commonA.size(); ++i) {
        numerator += (commonA[i] - meanA) * (commonB[i] - meanB);
        denomA += pow(commonA[i] - meanA, 2);
        denomB += pow(commonB[i] - meanB, 2);
    }
    return numerator / sqrt(denomA * denomB);
}

double cosineSimilarity(const vector<double> &a, const vector<double> &b) {
    double dotProduct = 0.0, normA = 0.0, normB = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != -1 && b[i] != -1) {
            dotProduct += a[i] * b[i];
            normA += pow(a[i], 2);
            normB += pow(b[i], 2);
        }
    }
    return dotProduct / (sqrt(normA) * sqrt(normB));
}

double euclideanDistance(const vector<double> &a, const vector<double> &b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != -1 && b[i] != -1) {
            sum += pow(a[i] - b[i], 2);
        }
    }
    return sqrt(sum);
}

vector<pair<int, double>> findNeighbors(const vector<vector<double>> &utilityMatrix, int userIndex, int itemIndex, int k, const string &metric) {
    vector<pair<int, double>> similarities;
    for (size_t i = 0; i < utilityMatrix.size(); ++i) {
        if (i != userIndex && utilityMatrix[i][itemIndex] != -1) {
            double similarity = 0.0;
            if (metric == "pearson") {
                similarity = pearsonCorrelation(utilityMatrix[userIndex], utilityMatrix[i]);
            } else if (metric == "cosine") {
                similarity = cosineSimilarity(utilityMatrix[userIndex], utilityMatrix[i]);
            } else if (metric == "euclidean") {
                similarity = -euclideanDistance(utilityMatrix[userIndex], utilityMatrix[i]);
            } else {
                cerr << "Invalid metric!" << endl;
                exit(1);
            }
            similarities.push_back(make_pair(i, similarity));
        }
    }
    sort(similarities.begin(), similarities.end(), [](const pair<int, double> &a, const pair<int, double> &b) {
        return a.second > b.second;
    });
    if (similarities.size() > k) {
        similarities.resize(k);
    }
    return similarities;
}

double predictSimple(const vector<vector<double>> &utilityMatrix, int userIndex, int itemIndex, const vector<pair<int, double>> &neighbors) {
    double numerator = 0.0, denominator = 0.0;
    for (const auto &neighbor : neighbors) {
        int neighborIndex = neighbor.first;
        double similarity = neighbor.second;
        if (utilityMatrix[neighborIndex][itemIndex] != -1) {
            numerator += similarity * utilityMatrix[neighborIndex][itemIndex];
            denominator += abs(similarity);
        }
    }
    return numerator / denominator;
}

double predictDiffWithMean(const vector<vector<double>> &utilityMatrix, int userIndex, int itemIndex, const vector<pair<int, double>> &neighbors) {
    double meanUser = accumulate(utilityMatrix[userIndex].begin(), utilityMatrix[userIndex].end(), 0.0,
                                 [](double sum, double rating) { return rating != -1 ? sum + rating : sum; }) /
                      count_if(utilityMatrix[userIndex].begin(), utilityMatrix[userIndex].end(),
                               [](double rating) { return rating != -1; });
    double numerator = 0.0, denominator = 0.0;
    for (const auto &neighbor : neighbors) {
        int neighborIndex = neighbor.first;
        double similarity = neighbor.second;
        double meanNeighbor = accumulate(utilityMatrix[neighborIndex].begin(), utilityMatrix[neighborIndex].end(), 0.0,
                                         [](double sum, double rating) { return rating != -1 ? sum + rating : sum; }) /
                              count_if(utilityMatrix[neighborIndex].begin(), utilityMatrix[neighborIndex].end(),
                                       [](double rating) { return rating != -1; });
        if (utilityMatrix[neighborIndex][itemIndex] != -1) {
            numerator += similarity * (utilityMatrix[neighborIndex][itemIndex] - meanNeighbor);
            denominator += abs(similarity);
        }
    }
    return meanUser + (numerator / denominator);
}

void printMatrix(const vector<vector<double>> &matrix) {
    for (const auto &row : matrix) {
        for (const auto &elem : row) {
            if (elem == -1) {
                cout << "- ";
            } else {
                cout << elem << " ";
            }
        }
        cout << endl;
    }
}
