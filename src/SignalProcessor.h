//
// Created by nicol on 10/12/2024.
//

#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <iostream>
#include <Eigen/Dense>
#include <complex>
#include <cmath>
#include <fstream>

/**
 * @class SignalProcessor
 * @brief A class for processing signals (images or sound) with operations like contour extraction and filtering.
 */

class SignalProcessor {
    private:
        Eigen::MatrixXcd data; // Stores the input data (image or sound)
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
        Eigen::MatrixXd applyThreshold(const Eigen::MatrixXd& magnitude, double threshold);
        
        Eigen::MatrixXcd applyHighPassFilter(double cutoff);
        Eigen::MatrixXcd applyLowPassFilter(double cutoff);
        
        template <typename T>
        void load_signal(const Eigen::MatrixBase<T>& input);

        template <typename T>
        Eigen::Matrix<std::complex<T>, -1, -1> getData();
        
};

/**
 * @brief Constructs a SignalProcessor object with the given input data.
 * @param input The input data matrix. Must be an Eigen Complex Matrix
 * @tparam T defines the complex type of the matrix, eg T=double would mean the matrix is a std::complex<double>
*/
template <typename T>
SignalProcessor::SignalProcessor(const Eigen::Matrix<std::complex<T>, -1, -1>& input){
    SignalProcessor::load_signal(input);
}


/**
 * @brief A function to load in the signal
 * 
 * @tparam T the type of the input data matrix.
 * @param input The input data matrix
 */
template <typename T>
void SignalProcessor::load_signal(const Eigen::MatrixBase<T>& input){
    SignalProcessor::data.resize(input.rows(), input.cols());
    SignalProcessor::data = input.template cast<std::complex<double>>();
    SignalProcessor::computeFrequencyGrid();
}

/**
 * @brief Computes the freqency grid. This is used in the fitlering process.
 * 
 */

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

    int center = rows / 2;


    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float u_centered = i - center;
            float v_centered = j - center;

            frequencyGrid(i, j) = std::sqrt(u_centered*u_centered + v_centered*v_centered);
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
/**
 * @brief Computes the magnitude of the gradient from its horizontal and vertical components.
 * @param gradientX The horizontal gradient matrix.
 * @param gradientY The vertical gradient matrix.
 * @return The gradient magnitude matrix.
 */
Eigen::MatrixXd SignalProcessor::computeMagnitude(const Eigen::MatrixXd& gradientX, const Eigen::MatrixXd& gradientY){
    return (gradientX.array().square() + gradientY.array().square()).sqrt();
}

// Method to apply threshold


/**
  * @brief Applies a threshold to the gradient magnitude to extract contours.
  *  @param magnitude The gradient magnitude matrix.
  * @param threshold The threshold value to apply.
  * @return A matrix representing the extracted contours.
  */

Eigen::MatrixXd SignalProcessor::applyThreshold(const Eigen::MatrixXd& magnitude, double threshold){
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


/**
  * @brief Applies a high-band filter to the input data.
  * @param cutoff The cutoff frequency for the high-band filter.
  * @return A filtered matrix with high frequencies preserved.
*/
Eigen::MatrixXcd SignalProcessor::applyHighPassFilter(double cutoff) {
    std::ofstream file("test.txt");
    Eigen::MatrixXcd filteredData = data;
    Eigen::MatrixXd filter(data.rows(), data.cols());
    file << "Here is the matrix before filtering:\n" << filteredData << '\n';

    for(int x = 0; x < filteredData.rows(); x++){
        for(int y = 0; y < filteredData.cols(); y++){
            filteredData(x, y) = (frequencyGrid(x, y) <= cutoff) ? filteredData(x, y) : std::complex<double>(0.0, 0.0);
        }
    }


    if (file.is_open()){
        file << "Here is the filter:\n" << filter << '\n';
        file << "Here is the filtered data:\n" << filteredData << '\n';
    }

    file.close();

    return filteredData;
}


/**
  * @brief Applies a low-band filter to the input data.
 * @param cutoff The cutoff frequency for the low-band filter.
 * @return A filtered matrix with low frequencies preserved.
 */
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

/**
 * @brief Retrieves the current input data of the SignalProcessor object.
 * @tparam T denotes the complex type of the returned data. Eg if you want to get a Eigen::Matrix<std::complex<int>, -1,
 * -1> you would write signalProcessor.getData<int>()
 * @return A copy of the data matrix. This is to avoid the user modifying the original data.
 * 
 */

template <typename T>
Eigen::Matrix<std::complex<T>, -1, -1> SignalProcessor::getData(){
    return SignalProcessor::data.block(0, 0, SignalProcessor::data.rows(), SignalProcessor::data.cols()).template cast<std::complex<T>>();
}
#endif //SIGNALPROCESSOR_H
