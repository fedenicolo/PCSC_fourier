//
// Created by nicol on 17/11/2024.
//

/*
#include "Histogram.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <numeric>
#include "gnuplot-iostream.h"

Histogram::Histogram(int numBins, double minRange, double maxRange)
    : bins(numBins, 0), numBins(numBins), minRange(minRange), maxRange(maxRange) {
    if (numBins <= 0 || minRange >= maxRange) {
        throw std::invalid_argument("Invalid number of bins or range.");
    }
}

// Compute the histogram from a vector input
void Histogram::compute(const std::vector<double> &data) {
    std::fill(bins.begin(), bins.end(), 0);

    for (double value : data) {
        if (value >= minRange && value <= maxRange) {
            int bin = static_cast<int>(((value - minRange) / (maxRange - minRange)) * numBins);
            if (bin >= numBins) bin = numBins - 1; // Handle edge case for maxRange
            bins[bin]++;
        }
    }
}

// Get normalized histogram as a probability density
std::vector<double> Histogram::getNormalizedHistogram() const {
    int totalCount = std::accumulate(bins.begin(), bins.end(), 0);
    std::vector<double> normalized(bins.size(), 0.0);

    if (totalCount > 0) {
        for (size_t i = 0; i < bins.size(); ++i) {
            normalized[i] = static_cast<double>(bins[i]) / totalCount;
        }
    }
    return normalized;
}

//get frequency counts
const std::vector<int>& Histogram::getBins() const {
    return bins;
}

std::vector<std::tuple<double, int, double>> Histogram::getHistogramData() const {
    std::vector<std::tuple<double, int, double>> histogramData;
    double binWidth = (maxRange - minRange) / numBins;

    for (size_t i = 0; i < bins.size(); ++i) {
        double binCenter = minRange + binWidth * (i + 0.5); // Center of the bin
        histogramData.emplace_back(binCenter, bins[i], binWidth);
    }

    return histogramData;
}

// Visualize Gnuplot
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
    gp << "plot '-' with boxes title 'Histogram'\n";
    gp.send1d(plotData);
}
*/

//
// Created by nicol on 17/11/2024.
//
#include "Histogram.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <numeric>
#include "gnuplot-iostream.h"

// Constructor
Histogram::Histogram(int numBins, double minRange, double maxRange)
    : bins(numBins, 0), numBins(numBins), minRange(minRange), maxRange(maxRange) {
    if (numBins <= 0 || minRange >= maxRange) {
        throw std::invalid_argument("Invalid number of bins or range.");
    }
}

// Compute the histogram from an Eigen matrix input
void Histogram::compute(const Eigen::MatrixXd &data) {
    if (data.rows() != 1) {
        throw std::invalid_argument("Input matrix must have exactly one row.");
    }

    std::fill(bins.begin(), bins.end(), 0);

    for (int i = 0; i < data.cols(); ++i) {
        double value = data(0, i);
        if (value >= minRange && value <= maxRange) {
            int bin = static_cast<int>(((value - minRange) / (maxRange - minRange)) * numBins);
            if (bin >= numBins) bin = numBins - 1; // Handle edge case for maxRange
            bins[bin]++;
        }
    }
}

// Get normalized histogram as a probability density (Eigen matrix)
Eigen::MatrixXd Histogram::getNormalizedHistogram() const {
    int totalCount = std::accumulate(bins.begin(), bins.end(), 0);
    Eigen::MatrixXd normalized(1, bins.size());
    normalized.setZero();

    if (totalCount > 0) {
        for (size_t i = 0; i < bins.size(); ++i) {
            normalized(0, i) = static_cast<double>(bins[i]) / totalCount;
        }
    }
    return normalized;
}

// Get frequency counts (Eigen matrix)
Eigen::MatrixXi Histogram::getBins() const {
    Eigen::MatrixXi binCounts(1, bins.size());
    for (size_t i = 0; i < bins.size(); ++i) {
        binCounts(0, i) = bins[i];
    }
    return binCounts;
}

// Get histogram data for visualization (Eigen matrix)
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

// Visualize Gnuplot
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
    gp << "plot '-' with boxes title 'Histogram'\n";
    gp.send1d(plotData);
}



