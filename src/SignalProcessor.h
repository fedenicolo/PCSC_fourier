//
// Created by nicol on 10/12/2024.
//

#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <iostream>
#include <Eigen/Dense>
#include <complex>
#include <cmath>

class SignalProcessor {
private:
    Eigen::Matrix<std::complex<double>, -1, -1> data; // Stores the input data (image or sound)
    Eigen::MatrixXd frequencyGrid; // Precomputed frequency grid

    // Helper method to compute the frequency grid

    void computeFrequencyGrid();

public:
    // Constructor
    template <typename T>
    SignalProcessor(const Eigen::Matrix<std::complex<T>, -1, -1>& input);

    // std::pair<Eigen::MatrixXd, Eigen::MatrixXd> computeGradients();
    Eigen::MatrixXd computeMagnitude(const Eigen::MatrixXd& gradientX, const Eigen::MatrixXd& gradientY);
    // Eigen::MatrixXd extractContours(double threshold);

    
    Eigen::MatrixXcd applyHighPassFilter(double cutoff);
    Eigen::MatrixXcd applyLowPassFilter(double cutoff);
    
    template <typename T>
    void load_signal(const Eigen::MatrixBase<T>& input);

    template <typename T>
    Eigen::Matrix<std::complex<T>, -1, -1> getData();
        
};

template <typename T>
SignalProcessor::SignalProcessor(const Eigen::Matrix<std::complex<T>, -1, -1>& input){
    SignalProcessor::load_signal(input);
}

template <typename T>
void SignalProcessor::load_signal(const Eigen::MatrixBase<T>& input){
    SignalProcessor::data.resize(input.rows(), input.cols());
    SignalProcessor::data = input.template cast<std::complex<double>>();
    SignalProcessor::computeFrequencyGrid();
}


void SignalProcessor::computeFrequencyGrid() {
        int rows = data.rows();
        int cols = data.cols();

        Eigen::MatrixXd rowFreq = Eigen::MatrixXd::Zero(rows, 1);
        Eigen::MatrixXd colFreq = Eigen::MatrixXd::Zero(1, cols);

        // Compute row frequencies
        for (int i = 0; i < rows; ++i) {
            rowFreq(i, 0) = (2 * M_PI * i) / rows;
        }

        // Compute column frequencies
        for (int j = 0; j < cols; ++j) {
            colFreq(0, j) = (2 * M_PI * j) / cols;
        }

        // Compute the combined frequency grid
        frequencyGrid = Eigen::MatrixXd::Zero(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                frequencyGrid(i, j) = std::sqrt(rowFreq(i, 0) * rowFreq(i, 0) + colFreq(0, j) * colFreq(0, j));
            }
        }
    }


    // Method to compute gradients
    // std::pair<Eigen::MatrixXd, Eigen::MatrixXd> SignalProcessor::computeGradients() {
    //     int rows = data.rows();
    //     int cols = data.cols();

    //     Eigen::MatrixXd gradientX = Eigen::MatrixXd::Zero(rows, cols);
    //     Eigen::MatrixXd gradientY = Eigen::MatrixXd::Zero(rows, cols);

    //     if (rows == 1) {
    //         // Sound case: compute gradient along the single row
    //         for (int j = 0; j < cols - 1; ++j) {
    //             gradientX(0, j) = data(0, j + 1) - data(0, j);
    //         }
    //     } else {
    //         // Image case: compute gradients in both directions
    //         for (int i = 0; i < rows - 1; ++i) {
    //             for (int j = 0; j < cols - 1; ++j) {
    //                 gradientX(i, j) = data(i + 1, j) - data(i, j); // Horizontal gradient
    //                 gradientY(i, j) = data(i, j + 1) - data(i, j); // Vertical gradient
    //             }
    //         }
    //     }

    //     return {gradientX, gradientY};
    // }

    // Method to compute gradient magnitude
    Eigen::MatrixXd SignalProcessor::computeMagnitude(const Eigen::MatrixXd& gradientX, const Eigen::MatrixXd& gradientY){
        return (gradientX.array().square() + gradientY.array().square()).sqrt();
    }

    // Method to apply threshold
    Eigen::MatrixXd applyThreshold(const Eigen::MatrixXd& magnitude, double threshold){
        Eigen::MatrixXd contours = Eigen::MatrixXd::Zero(magnitude.rows(), magnitude.cols());

        for (int i = 0; i < magnitude.rows(); ++i) {
            for (int j = 0; j < magnitude.cols(); ++j) {
                if (magnitude(i, j) > threshold) {
                    contours(i, j) = 1.0; // Mark as contour
                }
            }
        }

        return contours;
    }

    // // Public method to extract contours
    // Eigen::MatrixXd SignalProcessor::extractContours(double threshold){
    //     // Step 1: Compute gradients
    //     auto [gradientX, gradientY] = computeGradients();

    //     // Step 2: Compute gradient magnitude
    //     Eigen::MatrixXd magnitude = computeMagnitude(gradientX, gradientY);

    //     // Step 3: Apply threshold to extract contours
    //     return applyThreshold(magnitude, threshold);
    // }

    // High Band Filter

    Eigen::MatrixXcd SignalProcessor::applyHighPassFilter(double cutoff) {
        Eigen::MatrixXcd filteredData = data; // Start with a copy of the data

        int rows = data.rows();
        int cols = data.cols();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (frequencyGrid(i, j) <= cutoff) {
                    filteredData(i, j) = std::complex<double>(0.0, 0.0); // Suppress low frequencies
                }
            }
        }

        return filteredData;
    }

    // Low Band Filter
    Eigen::MatrixXcd SignalProcessor::applyLowPassFilter(double cutoff){
        Eigen::MatrixXcd filteredData = data; // Start with a copy of the data

        int rows = data.rows();
        int cols = data.cols();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (frequencyGrid(i, j) > cutoff) {
                    filteredData(i, j) = std::complex<double>(0.0, 0.0); // Suppress high frequencies
                }
            }
        }

        return filteredData;
    }

    // Getter to retrieve the data
    template <typename T>
    const Eigen::Matrix<std::complex<T>, -1, -1> SignalProcessor::getData(){
        return SignalProcessor::data.template cast<std::complex<T>>();
    }

#endif //SIGNALPROCESSOR_H
