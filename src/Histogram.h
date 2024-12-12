#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <Eigen/Dense>
#include "Input.h" // Base class for Image and Sound

/**
 * @class Histogram
 * @brief A class for computing and visualizing histograms from data matrices or input objects, including Fourier transform results.
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
     * @brief Constructs a histogram from a real-valued Eigen matrix.
     * @param dataMatrix The input Eigen matrix.
     * @param numBins The number of bins for the histogram (default: 10).
     * @throws std::invalid_argument If numBins is not positive.
     */
    Histogram(const Eigen::MatrixXd& dataMatrix, int numBins = 10);

    /**
     * @brief Constructs a histogram from a complex-valued Eigen matrix, such as a Fourier transform result.
     * @param complexMatrix The complex-valued Eigen matrix (e.g., Fourier transform result).
     * @param numBins The number of bins for the histogram.
     * @param sampleRate The sample rate of the input data (default: 1.0).
     * @throws std::invalid_argument If numBins is not positive.
     */
    Histogram(const Eigen::MatrixXcd& complexMatrix, int numBins, double sampleRate = 1.0);

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


private:
    Eigen::MatrixXd data;          ///< Flattened data matrix.
    std::vector<int> bins;         ///< Frequency counts for each bin.
    int numBins;                   ///< Number of bins in the histogram.
    double minRange;               ///< Minimum value of the data range.
    double maxRange;               ///< Maximum value of the data range.

    /**
     * @brief Flattens a multi-row matrix into a single row.
     */
    void flattenMatrix();

    /**
     * @brief Initializes the histogram data and bin ranges for a real-valued matrix.
     * @param dataMatrix The input Eigen matrix.
     * @param numBins The number of bins for the histogram.
     */
    void initialize(const Eigen::MatrixXd& dataMatrix, int numBins);

    /**
     * @brief Initializes the histogram data and bin ranges for a complex-valued matrix.
     * @param complexMatrix The complex-valued Eigen matrix (e.g., Fourier transform result).
     * @param numBins The number of bins for the histogram.
     * @param sampleRate The sample rate of the input data.
     */
    void initialize(const Eigen::MatrixXcd& complexMatrix, int numBins, double sampleRate);
};

#endif // HISTOGRAM_H
