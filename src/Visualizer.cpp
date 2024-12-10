#include "Visualizer.h"
#include <iostream>
#include <stdexcept>

// Constructor
Visualizer::Visualizer() {}

// Generic plot function
void Visualizer::plot(const Eigen::MatrixXd& data, const std::string& plotType, int numBins, const std::string& saveAs) {
    if (plotType == "histogram") {
        plotHistogram(data, numBins, saveAs);
    } else if (plotType == "line") {
        plotLine(data, saveAs);
    } else {
        throw std::invalid_argument("Unknown plot type: " + plotType);
    }
}

// Plot histogram
void Visualizer::plotHistogram(const Eigen::MatrixXd& data, int numBins, const std::string& saveAs) {
    // Create a histogram from the input matrix
    Histogram histogram(data, numBins);  // Use constructor with Eigen matrix
    histogram.compute();

    // Get the histogram data for visualization (bin center, frequency, and bin width)
    Eigen::MatrixXd histogramData = histogram.getHistogramData();
    Eigen::VectorXd normalizedHistogram = histogram.getNormalizedHistogram();

    // Prepare data for Gnuplot
    std::vector<std::pair<double, double>> plotData;
    for (int i = 0; i < histogramData.rows(); ++i) {
        plotData.push_back({histogramData(i, 0), normalizedHistogram(i)});  // bin center, frequency
    }

    // Plot the histogram using Gnuplot
    Gnuplot gp;
    configureOutput(gp, saveAs);
    gp << "set title 'Histogram'\n";
    gp << "set xlabel 'Value'\n";
    gp << "set ylabel 'Frequency'\n";
    gp << "set grid\n";
    gp << "set boxwidth 0.8 relative\n";
    gp << "set style fill solid 1.0 border -1\n";
    gp << "set xtics rotate by -45\n";
    gp << "plot '-' using 1:2 with boxes title 'Histogram'\n";
    gp.send1d(plotData);

    if (!saveAs.empty()) {
        gp << "unset output\n"; // Finalize the PNG file
    }
}

// Plot line
void Visualizer::plotLine(const Eigen::MatrixXd& data, const std::string& saveAs) {
    std::vector<std::pair<double, double>> plotData;
    Eigen::VectorXd flatData = Eigen::Map<const Eigen::VectorXd>(data.data(), data.size());

    for (int i = 0; i < flatData.size(); ++i) {
        plotData.push_back({static_cast<double>(i), flatData(i)});  // x are the indices, y are the values
    }

    // Plot the data using Gnuplot
    Gnuplot gp;
    configureOutput(gp, saveAs);
    gp << "set title 'Line Plot'\n";
    gp << "set xlabel 'X'\n";
    gp << "set ylabel 'Y'\n";
    gp << "set grid\n";
    gp << "plot '-' with lines title 'Data'\n";
    gp.send1d(plotData);

    if (!saveAs.empty()) {
        gp << "unset output\n"; // Finalize the PNG file
    }
}

// Configure output for saving plots
void Visualizer::configureOutput(Gnuplot& gp, const std::string& saveAs) {
    if (!saveAs.empty()) {
        gp << "set terminal pngcairo enhanced\n"; // Use PNG Cairo terminal for better output
        gp << "set output '" << saveAs << "'\n";
    }
}

// Print summary statistics
void Visualizer::printSummary(Eigen::MatrixXd& data) const {
    // Flatten the matrix into a vector
    Eigen::Map<Eigen::VectorXd> flattened(data.data(), data.size());

    // Calculate basic statistics
    double mean = flattened.mean();
    double stddev = std::sqrt((flattened.array() - mean).square().mean());
    double minVal = flattened.minCoeff();
    double maxVal = flattened.maxCoeff();
    double range = maxVal - minVal;

    // Print the statistics
    std::cout << "Summary Statistics:\n";
    std::cout << "Mean: " << mean << "\n";
    std::cout << "Standard Deviation: " << stddev << "\n";
    std::cout << "Min: " << minVal << "\n";
    std::cout << "Max: " << maxVal << "\n";
    std::cout << "Range: " << range << "\n";
}
