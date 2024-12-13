#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <iostream>
#include <Eigen/Dense>
#include <complex>
#include <cmath>
#include <fstream>

/**
 * @class SignalProcessor
 * @brief A class for processing signals (images or sound) with operations like filtering.
 */
class SignalProcessor {
    private:
        Eigen::MatrixXcd data; // Stores the input data (image or sound)
        Eigen::MatrixXd frequencyGrid; // Precomputed frequency grid
        bool isSound; // Indicates whether the input is a sound (true) or an image (false)

        // Helper methods
        void computeFrequencyGrid();
        void computeFrequencyGridForSound();

    public:
        // Constructor
        template <typename T>
        SignalProcessor(const Eigen::Matrix<std::complex<T>, -1, -1>& input, bool isSound);

        Eigen::MatrixXcd applyHighPassFilter(double cutoff);
        Eigen::MatrixXcd applyLowPassFilter(double cutoff);

        template <typename T>
        void load_signal(const Eigen::MatrixBase<T>& input, bool isSound);

        template <typename T>
        Eigen::Matrix<std::complex<T>, -1, -1> getData();
        Eigen::MatrixXd getFrequencyGrid() const {
            return frequencyGrid;
        }
};

/**
 * @brief Constructs a SignalProcessor object with the given input data.
 * @param input The input data matrix. Must be an Eigen Complex Matrix.
 * @param isSound Indicates whether the input is a sound (true) or an image (false).
 * @tparam T Defines the complex type of the matrix, e.g., T=double means the matrix is std::complex<double>.
 */
template <typename T>
SignalProcessor::SignalProcessor(const Eigen::Matrix<std::complex<T>, -1, -1>& input, bool isSound) : isSound(isSound) {
    SignalProcessor::load_signal(input, isSound);
}

/**
 * @brief Loads the signal into the processor and computes the frequency grid.
 * @tparam T The type of the input data matrix.
 * @param input The input data matrix.
 * @param isSound Indicates whether the input is a sound (true) or an image (false).
 */
template <typename T>
void SignalProcessor::load_signal(const Eigen::MatrixBase<T>& input, bool isSound) {
    SignalProcessor::data.resize(input.rows(), input.cols());
    SignalProcessor::data = input.template cast<std::complex<double>>();
    SignalProcessor::isSound = isSound;

    if (isSound) {
        computeFrequencyGridForSound();
    } else {
        computeFrequencyGrid();
    }
}

/**
 * @brief Computes the frequency grid for images.
 */
void SignalProcessor::computeFrequencyGrid() {
    int rows = data.rows();
    int cols = data.cols();

    Eigen::MatrixXd rowFreq = Eigen::MatrixXd::Zero(rows, 1);
    Eigen::MatrixXd colFreq = Eigen::MatrixXd::Zero(1, cols);

    // Compute row frequencies
    for (int i = 0; i < rows; ++i) {
        rowFreq(i, 0) = (i < rows / 2) ? i : i - rows;
    }

    // Compute column frequencies
    for (int j = 0; j < cols; ++j) {
        colFreq(0, j) = (j < cols / 2) ? j : j - cols;
    }

    // Compute the combined frequency grid
    frequencyGrid = Eigen::MatrixXd::Zero(rows, cols);
    double maxFrequency = std::sqrt((rows / 2.0) * (rows / 2.0) + (cols / 2.0) * (cols / 2.0)); // Max possible frequency

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            frequencyGrid(i, j) = std::sqrt(rowFreq(i, 0) * rowFreq(i, 0) + colFreq(0, j) * colFreq(0, j)) / maxFrequency;
        }
    }
}

/**
 * @brief Computes the frequency grid for sound signals.
 * Assumes the input is a single row vector and spans the normalized frequency domain [0, 1].
 */
void SignalProcessor::computeFrequencyGridForSound() {
    int cols = data.cols();

    // Compute frequencies for the single row
    frequencyGrid = Eigen::MatrixXd::Zero(1, cols);
    for (int j = 0; j < cols; ++j) {
        frequencyGrid(0, j) = (j < cols / 2) ? j : j - cols;
    }
    frequencyGrid /= cols; // Normalize to the range [0, 1]
}

/**
 * @brief Applies a high-pass filter to the input data.
 * @param cutoff The cutoff frequency for the high-pass filter.
 * @return A filtered matrix with high frequencies preserved.
 */
Eigen::MatrixXcd SignalProcessor::applyHighPassFilter(double cutoff) {
    Eigen::MatrixXcd filteredData = data;

    for (int i = 0; i < frequencyGrid.rows(); ++i) {
        for (int j = 0; j < frequencyGrid.cols(); ++j) {
            if (frequencyGrid(i, j) < cutoff) {
                filteredData(i, j) = std::complex<double>(0.0, 0.0);
            }
        }
    }

    return filteredData;
}

/**
 * @brief Applies a low-pass filter to the input data.
 * @param cutoff The cutoff frequency for the low-pass filter.
 * @return A filtered matrix with low frequencies preserved.
 */
Eigen::MatrixXcd SignalProcessor::applyLowPassFilter(double cutoff) {
    Eigen::MatrixXcd filteredData = data;

    for (int i = 0; i < frequencyGrid.rows(); ++i) {
        for (int j = 0; j < frequencyGrid.cols(); ++j) {
            if (frequencyGrid(i, j) > cutoff) {
                filteredData(i, j) = std::complex<double>(0.0, 0.0);
            }
        }
    }

    return filteredData;
}

/**
 * @brief Retrieves the current input data of the SignalProcessor object.
 * @tparam T Denotes the complex type of the returned data.
 * @return A copy of the data matrix.
 */
template <typename T>
Eigen::Matrix<std::complex<T>, -1, -1> SignalProcessor::getData() {
    return SignalProcessor::data.template cast<std::complex<T>>();
}

#endif // SIGNALPROCESSOR_H