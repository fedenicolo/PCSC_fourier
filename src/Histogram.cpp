#include "Histogram.h"
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <algorithm> // For std::clamp
#include <complex>
#include "gnuplot-iostream.h"

// Constructors
Histogram::Histogram(const Eigen::MatrixXd& dataMatrix, int numBins)
    : numBins(numBins) {
    if (numBins <= 0) {
        throw std::invalid_argument("Number of bins must be positive.");
    }
    initialize(dataMatrix, numBins);
}

Histogram::Histogram(const Eigen::MatrixXcd& complexMatrix, int numBins, double sampleRate)
    : numBins(numBins) {
    if (numBins <= 0) {
        throw std::invalid_argument("Number of bins must be positive.");
    }
    initialize(complexMatrix, numBins, sampleRate);
}

// Initialization for real-valued matrix
void Histogram::initialize(const Eigen::MatrixXd& dataMatrix, int numBins) {
    data = dataMatrix;
    flattenMatrix();
    minRange = data.minCoeff();
    maxRange = data.maxCoeff();
    bins.resize(numBins, 0);
}

// Initialization for complex-valued matrix
void Histogram::initialize(const Eigen::MatrixXcd& complexMatrix, int numBins, double sampleRate) {
    // Compute the power spectrum
    Eigen::MatrixXd powerSpectrum = complexMatrix.array().abs2().matrix();

    // Flatten the power spectrum for a 1D histogram
    data = Eigen::Map<Eigen::VectorXd>(powerSpectrum.data(), powerSpectrum.size());

    // Compute frequency bins (assume equally spaced frequencies)
    int numRows = complexMatrix.rows();
    int numCols = complexMatrix.cols();
    int totalSamples = numRows * numCols;

    minRange = data.minCoeff();
    maxRange = data.maxCoeff();
    bins.resize(numBins, 0);
}

// Flatten a multi-row matrix into a single row
void Histogram::flattenMatrix() {
    if (data.rows() > 1) {
        data.resize(1, data.size()); // Flatten into a single row
    }
}

// Compute the histogram
void Histogram::compute() {
    std::fill(bins.begin(), bins.end(), 0);
    double range = maxRange - minRange;

    for (int i = 0; i < data.size(); ++i) {
        double value = data(i);
        if (value >= minRange && value <= maxRange) {
            int bin = static_cast<int>(((value - minRange) / range) * numBins);
            bin = std::clamp(bin, 0, numBins - 1);
            bins[bin]++;
        }
    }
}

// Get normalized histogram as a probability density
Eigen::VectorXd Histogram::getNormalizedHistogram() const {
    int totalCount = std::accumulate(bins.begin(), bins.end(), 0);
    Eigen::VectorXd normalized(bins.size());
    normalized.setZero();

    if (totalCount > 0) {
        for (size_t i = 0; i < bins.size(); ++i) {
            normalized(i) = static_cast<double>(bins[i]) / totalCount;
        }
    }
    return normalized;
}

// Get frequency counts
Eigen::VectorXi Histogram::getBins() const {
    Eigen::VectorXi binCounts(bins.size());
    for (size_t i = 0; i < bins.size(); ++i) {
        binCounts(i) = bins[i];
    }
    return binCounts;
}

// Get histogram data for visualization
Eigen::MatrixXd Histogram::getHistogramData() const {
    Eigen::MatrixXd histogramData(bins.size(), 3); // Columns: [binCenter, frequency, binWidth]
    double binWidth = (maxRange - minRange) / numBins;

    for (size_t i = 0; i < bins.size(); ++i) {
        double binCenter = minRange + binWidth * (i + 0.5); // Center of the bin
        histogramData(i, 0) = binCenter;
        histogramData(i, 1) = bins[i];
        histogramData(i, 2) = binWidth;
    }

    return histogramData;
}
