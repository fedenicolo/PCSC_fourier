#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <Eigen/Dense>
#include "Histogram.h"
#include <string>
#include <vector>
#include "gnuplot-iostream.h"

/**
 * @class Visualizer
 * @brief A class for visualizing data using various plot types with Gnuplot integration.
 */
class Visualizer {
public:
    /**
     * @brief Default constructor for the Visualizer class.
     */
    Visualizer();

    /**
     * @brief Plots data using the specified plot type.
     * @param data The input data matrix to be plotted.
     * @param plotType The type of plot (e.g., "histogram", "line").
     * @param numBins The number of bins for histogram plots (default: 10).
     * @param saveAs The file path to save the plot as an image (default: empty string).
     * @throws std::invalid_argument If an unknown plot type is provided.
     */
    void plot(const Eigen::MatrixXd& data, const std::string& plotType, int numBins = 10, const std::string& saveAs = "");

    /**
     * @brief Prints summary statistics (mean, standard deviation, min, max, range) of the input data.
     * @param data The input data matrix to compute statistics for.
     */
    void printSummary(Eigen::MatrixXd& data) const;

private:
    /**
     * @brief Plots a histogram of the input data.
     * @param data The input data matrix to create the histogram from.
     * @param numBins The number of bins for the histogram.
     * @param saveAs The file path to save the histogram as an image.
     */
    void plotHistogram(const Eigen::MatrixXd& data, int numBins, const std::string& saveAs);

    /**
     * @brief Plots a line graph of the input data.
     * @param data The input data matrix to plot.
     * @param saveAs The file path to save the line plot as an image.
     */
    void plotLine(const Eigen::MatrixXd& data, const std::string& saveAs);

    /**
     * @brief Configures the output file for saving plots.
     * @param gp The Gnuplot object used for plotting.
     * @param saveAs The file path to save the plot as an image.
     */
    void configureOutput(Gnuplot& gp, const std::string& saveAs);
};

#endif // VISUALIZER_H
