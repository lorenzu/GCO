#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>

#include "tools.h"

int main(int argc, char *argv[]) {
    Usage(argc, argv);

    std::string filename = argv[1];
    std::vector<std::vector<double>> utilityMatrix;
    double minRating, maxRating;
    readInputFile(filename, utilityMatrix, minRating, maxRating);


    std::string metric = argv[2];
    //std::cout << "Enter the similarity metric (pearson/cosine/euclidean): ";
    //std::cin >> metric;

    int k = std::stoi(argv[3]);
    //std::cout << "Enter the number of neighbors: ";
    //std::cin >> k;


    std::string predictionType = argv[4];
    //std::cout << "Enter the type of prediction (simple/mean): ";
    //std::cin >> predictionType;

    std::vector<std::vector<double>> predictedMatrix = utilityMatrix;

    for (size_t userIndex = 0; userIndex < utilityMatrix.size(); ++userIndex) {
        for (size_t itemIndex = 0; itemIndex < utilityMatrix[userIndex].size(); ++itemIndex) {
            if (utilityMatrix[userIndex][itemIndex] == -1) {
                std::vector<std::pair<int, double>> neighbors = findNeighbors(utilityMatrix, userIndex, itemIndex, k, metric);
                double prediction;
                if (predictionType == "simple") {
                    prediction = predictSimple(utilityMatrix, userIndex, itemIndex, neighbors);
                } else if (predictionType == "mean") {
                    prediction = predictDiffWithMean(utilityMatrix, userIndex, itemIndex, neighbors);
                } else {
                    std::cerr << "Invalid prediction type!" << std::endl;
                    return 1;
                }
                predictedMatrix[userIndex][itemIndex] = prediction;
            }
        }
    }

    std::cout << "Predicted Utility Matrix:" << std::endl;
    printMatrix(predictedMatrix);
    printMatrixinFile(predictedMatrix, "predictedMatrix.txt");
    return 0;
}

