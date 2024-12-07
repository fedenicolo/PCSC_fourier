//
// Created by nicol on 29/11/2024.
//
#include "CSVOutput.h"
#include "ImageExceptions.h"
#include <Eigen/Dense>

CSVOutput::CSVOutput(const std::string& path) : Output(path) {}

void CSVOutput::save(const Eigen::MatrixXd& data) {
    std::ofstream file(outputFilePath);
    if (!file.is_open()) {
        throw INVALID_CSV_OPEN("Cannot open file: " + outputFilePath);
    }

    for (int i = 0; i < data.rows(); ++i) {
        for (int j = 0; j < data.cols(); ++j) {
            file << data(i, j);
            if (j < data.cols() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
    file.close();
}
