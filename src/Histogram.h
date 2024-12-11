#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <Eigen/Dense>
#include "Input.h" // Base class for Image and Sound

/**
 * @class Histogram
 * @brief A class for computing and visualizing histograms from data matrices or input objects.
 */
class Histogram {
public:
    /**
     * @brief Constructs a histogram from an Input object.
     * @param inputObject The input object (e.g., Image or Sound).
     * @param numBins The number of bins for the histogram (default: 10).
     * @throws std::invalid_argument If numBins is not positive.
     */
    Histogram(Input& inputObject, int numBins = 10);

    /**
     * @brief Constructs a histogram from an Eigen matrix.
     * @param dataMatrix The input Eigen matrix.
     * @param numBins The number of bins for the histogram (default: 10).
     * @throws std::invalid_argument If numBins is not positive.
     */
    Histogram(const Eigen::MatrixXd& dataMatrix, int numBins = 10);

    /**
     * @brief Computes the histogram by populating bins with frequency counts.
     */
    void compute();

    /**
     * @brief Gets the normalized histogram as a probability density.
     * @return A vector representing the normalized histogram.
     */
    Eigen::VectorXd getNormalizedHistogram() const;

    /**
     * @brief Gets the frequency counts for each bin.
     * @return A vector of integers representing the frequency counts.
     */
    Eigen::VectorXi getBins() const;

    /**
     * @brief Gets the histogram data for visualization.
     * @return A matrix with columns [bin center, frequency, bin width].
     */
    Eigen::MatrixXd getHistogramData() const;

    /**
     * @brief Visualizes the histogram using Gnuplot.
     */
    void visualize() const;

private:
    Eigen::MatrixXd data;       ///< Flattened data matrix.
    std::vector<int> bins;      ///< Frequency counts for each bin.
    int numBins;                ///< Number of bins in the histogram.
    double minRange;            ///< Minimum value of the data range.
    double maxRange;            ///< Maximum value of the data range.

    /**
     * @brief Flattens a multi-row matrix into a single row.
     */
    void flattenMatrix();

    /**
     * @brief Initializes the histogram data and bin ranges.
     * @param dataMatrix The input Eigen matrix.
     * @param numBins The number of bins for the histogram.
     */
    void initialize(const Eigen::MatrixXd& dataMatrix, int numBins);
};

#endif // HISTOGRAM_H
