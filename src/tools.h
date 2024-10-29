#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>

// Function prototypes
void readInputFile(const std::string &filename, std::vector<std::vector<double>> &utilityMatrix, double &minRating, double &maxRating);
double pearsonCorrelation(const std::vector<double> &a, const std::vector<double> &b);
double cosineSimilarity(const std::vector<double> &a, const std::vector<double> &b);
double euclideanDistance(const std::vector<double> &a, const std::vector<double> &b);
std::vector<std::pair<int, double>> findNeighbors(const std::vector<std::vector<double>> &utilityMatrix, int userIndex, int itemIndex, int k, const std::string &metric);
double predictSimple(const std::vector<std::vector<double>> &utilityMatrix, int userIndex, int itemIndex, const std::vector<std::pair<int, double>> &neighbors);
double predictDiffWithMean(const std::vector<std::vector<double>> &utilityMatrix, int userIndex, int itemIndex, const std::vector<std::pair<int, double>> &neighbors);
void printMatrix(const std::vector<std::vector<double>> &matrix);
void Usage(int argc, char *argv[]);