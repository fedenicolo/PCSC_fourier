#include "Histogram.h"
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <algorithm> // For std::clamp
#include "gnuplot-iostream.h"

// Constructor from Input
Histogram::Histogram(Input& inputObject, int numBins)
    : numBins(numBins) {
    if (numBins <= 0) {
        throw std::invalid_argument("Number of bins must be positive.");
    }
    initialize(inputObject.getData(), numBins);
}

// Constructor from Eigen matrix
Histogram::Histogram(const Eigen::MatrixXd& dataMatrix, int numBins)
    : numBins(numBins) {
    if (numBins <= 0) {
        throw std::invalid_argument("Number of bins must be positive.");
    }
    initialize(dataMatrix, numBins);
}

// Shared initialization logic
void Histogram::initialize(const Eigen::MatrixXd& dataMatrix, int numBins) {
    data = dataMatrix;
    flattenMatrix();

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

    for (int i = 0; i < data.cols(); ++i) {
        double value = data(0, i);
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

// Visualize histogram using Gnuplot
void Histogram::visualize() const {
    Gnuplot gp;

    std::vector<std::pair<double, int>> plotData;
    double binWidth = (maxRange - minRange) / numBins;

    for (size_t i = 0; i < bins.size(); ++i) {
        double binCenter = minRange + binWidth * (i + 0.5); // Center of the bin
        plotData.emplace_back(binCenter, bins[i]);
    }

    gp << "set title 'Histogram'\n";
    gp << "set xlabel 'Value'\n";
    gp << "set ylabel 'Frequency'\n";
    gp << "set grid\n";
    gp << "set boxwidth 0.8 relative\n";
    gp << "set style fill solid 1.0 border -1\n";
    gp << "set xtics rotate by -45\n";
    gp << "set format x '%.1f'\n";
    gp << "set format y '%.0f'\n";
    gp << "plot '-' using 1:2 with boxes title 'Histogram'\n";
    gp.send1d(plotData);
}
