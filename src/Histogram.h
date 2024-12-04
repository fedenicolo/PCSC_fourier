#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <Eigen/Dense>
#include "Input.h" // Base class for Image and Sound

class Histogram {
public:
    // Constructors
    Histogram(Input& inputObject, int numBins = 10); // From Input
    Histogram(const Eigen::MatrixXd& dataMatrix, int numBins = 10); // From Eigen matrix

    // Compute the histogram
    void compute();

    // Get normalized histogram as a probability density
    Eigen::VectorXd getNormalizedHistogram() const;

    // Get frequency counts
    Eigen::VectorXi getBins() const;

    // Get histogram data for visualization
    Eigen::MatrixXd getHistogramData() const;

    // Visualize histogram using Gnuplot
    void visualize() const;

private:
    Eigen::MatrixXd data;       // Flattened data
    std::vector<int> bins;      // Frequency counts
    int numBins;                // Number of bins
    double minRange;            // Minimum range value
    double maxRange;            // Maximum range value

    // Helper methods
    void flattenMatrix();
    void initialize(const Eigen::MatrixXd& dataMatrix, int numBins);
};

#endif // HISTOGRAM_H
