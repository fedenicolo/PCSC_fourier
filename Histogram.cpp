//
// Created by nicol on 17/11/2024.
//
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

// Compute the histogram from a generic data input
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

// Visualize the histogram using Gnuplot
void Histogram::visualize() const {
    Gnuplot gp;

    std::vector<std::pair<double, int>> plotData;
    double binWidth = (maxRange - minRange) / numBins;
    for (size_t i = 0; i < bins.size(); ++i) {
        double binCenter = minRange + binWidth * (i + 0.5); // Center of the bin
        plotData.emplace_back(binCenter, bins[i]);
    }

    // settings
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



