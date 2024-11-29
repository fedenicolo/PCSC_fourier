//
// Created by nicol on 17/11/2024.
//

/*
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

//TODO: Change to Eigen Matrix
#include <vector>

class Histogram {
public:
    Histogram(int numBins, double minRange, double maxRange);

    void compute(const std::vector<double> &data);

    const std::vector<int>& getBins() const;

    std::vector<double> getNormalizedHistogram() const;

    std::vector<std::tuple<double, int, double>> getHistogramData() const;

    void visualize() const;

private:
    std::vector<int> bins;
    int numBins;
    double minRange;
    double maxRange;
};

#endif // HISTOGRAM_H

*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <Eigen/Dense> // For Eigen matrices

class Histogram {
public:
    // Constructor
    Histogram(int numBins, double minRange, double maxRange);

    // Compute the histogram from an Eigen matrix (single row)
    void compute(const Eigen::MatrixXd& data);

    // Get normalized histogram as a probability density (Eigen matrix)
    Eigen::MatrixXd getNormalizedHistogram() const;

    // Get frequency counts (Eigen matrix)
    Eigen::MatrixXi getBins() const;

    // Get histogram data for visualization (Eigen matrix)
    Eigen::MatrixXd getHistogramData() const;

    // Visualize histogram using Gnuplot
    void visualize() const;

private:
    std::vector<int> bins; // Frequency counts
    int numBins;           // Number of bins
    double minRange;       // Minimum range value
    double maxRange;       // Maximum range value
};

#endif // HISTOGRAM_H
