#include "tools.h"

void readInputFile(const std::string &filename, std::vector<std::vector<double>> &utilityMatrix, double &minRating, double &maxRating) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open the file!" << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(file, line);
    minRating = std::stod(line);

    std::getline(file, line);
    maxRating = std::stod(line);

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;
        while (ss >> value) {
            if (value == "-") {
                row.push_back(-1);
            } else {
                row.push_back(std::stod(value));
            }
        }
        utilityMatrix.push_back(row);
    }
    file.close();
}

double pearsonCorrelation(const std::vector<double> &a, const std::vector<double> &b) {
    std::vector<double> commonA, commonB;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != -1 && b[i] != -1) {
            commonA.push_back(a[i]);
            commonB.push_back(b[i]);
        }
    }
    if (commonA.empty()) return 0.0;

    double meanA = std::accumulate(commonA.begin(), commonA.end(), 0.0) / commonA.size();
    double meanB = std::accumulate(commonB.begin(), commonB.end(), 0.0) / commonB.size();
    double numerator = 0.0, denomA = 0.0, denomB = 0.0;
    for (size_t i = 0; i < commonA.size(); ++i) {
        numerator += (commonA[i] - meanA) * (commonB[i] - meanB);
        denomA += std::pow(commonA[i] - meanA, 2);
        denomB += std::pow(commonB[i] - meanB, 2);
    }
    return numerator / std::sqrt(denomA * denomB);
}

double cosineSimilarity(const std::vector<double> &a, const std::vector<double> &b) {
    double dotProduct = 0.0, normA = 0.0, normB = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != -1 && b[i] != -1) {
            dotProduct += a[i] * b[i];
            normA += std::pow(a[i], 2);
            normB += std::pow(b[i], 2);
        }
    }
    return dotProduct / (std::sqrt(normA) * std::sqrt(normB));
}

double euclideanDistance(const std::vector<double> &a, const std::vector<double> &b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != -1 && b[i] != -1) {
            sum += std::pow(a[i] - b[i], 2);
        }
    }
    return std::sqrt(sum);
}

std::vector<std::pair<int, double>> findNeighbors(const std::vector<std::vector<double>> &utilityMatrix, int userIndex, int itemIndex, int k, const std::string &metric) {
    std::vector<std::pair<int, double>> similarities;
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
                std::cerr << "Invalid metric!" << std::endl;
                exit(1);
            }
            similarities.push_back(std::make_pair(i, similarity));
        }
    }
    std::sort(similarities.begin(), similarities.end(), [](const std::pair<int, double> &a, const std::pair<int, double> &b) {
        return a.second > b.second;
    });
    if (similarities.size() > k) {
        similarities.resize(k);
    }
    return similarities;
}

double predictSimple(const std::vector<std::vector<double>> &utilityMatrix, int userIndex, int itemIndex, const std::vector<std::pair<int, double>> &neighbors) {
    double numerator = 0.0, denominator = 0.0;
    for (const auto &neighbor : neighbors) {
        int neighborIndex = neighbor.first;
        double similarity = neighbor.second;
        if (utilityMatrix[neighborIndex][itemIndex] != -1) {
            numerator += similarity * utilityMatrix[neighborIndex][itemIndex];
            denominator += std::abs(similarity);
        }
    }
    return numerator / denominator;
}

double predictDiffWithMean(const std::vector<std::vector<double>> &utilityMatrix, int userIndex, int itemIndex, const std::vector<std::pair<int, double>> &neighbors) {
    double meanUser = std::accumulate(utilityMatrix[userIndex].begin(), utilityMatrix[userIndex].end(), 0.0,
                                      [](double sum, double rating) { return rating != -1 ? sum + rating : sum; }) /
                      std::count_if(utilityMatrix[userIndex].begin(), utilityMatrix[userIndex].end(),
                                    [](double rating) { return rating != -1; });
    double numerator = 0.0, denominator = 0.0;
    for (const auto &neighbor : neighbors) {
        int neighborIndex = neighbor.first;
        double similarity = neighbor.second;
        double meanNeighbor = std::accumulate(utilityMatrix[neighborIndex].begin(), utilityMatrix[neighborIndex].end(), 0.0,
                                              [](double sum, double rating) { return rating != -1 ? sum + rating : sum; }) /
                             std::count_if(utilityMatrix[neighborIndex].begin(), utilityMatrix[neighborIndex].end(),
                                           [](double rating) { return rating != -1; });
        if (utilityMatrix[neighborIndex][itemIndex] != -1) {
            numerator += similarity * (utilityMatrix[neighborIndex][itemIndex] - meanNeighbor);
            denominator += std::abs(similarity);
        }
    }
    return meanUser + (numerator / denominator);
}

void printMatrix(const std::vector<std::vector<double>> &matrix) {
    for (const auto &row : matrix) {
        for (const auto &elem : row) {
            if (elem == -1) {
                std::cout << "- ";
            } else {
                std::cout << elem << " ";
            }
        }
        std::cout << std::endl;
    }
}

void Usage(int argc, char *argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <filename> <metric> <k> <predictionType>" << std::endl;
        std::cout << "<metric> - pearson/cosine/euclidean" << std::endl;
        std::cout << "<k> - number of neighbors" << std::endl;
        std::cout << "<predictionType> - simple/mean" << std::endl;
        exit(1);
    }
}