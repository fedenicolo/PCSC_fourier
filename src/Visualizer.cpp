#include "Visualizer.h"
#include "Histogram.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <numeric>
#include "gnuplot-iostream.h"
#include <filesystem>

// Constructor
Visualizer::Visualizer() {
    std::string outputDirectory = "results";
    if (!std::filesystem::exists(outputDirectory)) {
        std::filesystem::create_directory(outputDirectory);
    }
}

// Configure output for saving plots
void Visualizer::configureOutput(const std::string& saveAs) {
    if (!saveAs.empty()) {
        Gnuplot gp;
        gp << "set terminal png size 800,600\n";
        gp << "set output '" << saveAs << "'\n";
    }
}

// Plot Histogram
void Visualizer::plotHistogram(const Histogram& histogram, const std::string& saveAs, const std::string& type) {
    Eigen::MatrixXd histogramData = histogram.getHistogramData();
    Eigen::MatrixXd normalizedData = histogram.getNormalizedHistogram();

    Gnuplot gp;
    gp << "set title 'Histogram'\n";
    gp << "set xlabel 'Value'\n";
    gp << "set ylabel 'Frequency'\n";
    gp << "set boxwidth 0.8 relative\n";
    gp << "set style fill solid 1.0 border -1\n";
    gp << "plot '-' using 1:2 with boxes title 'Histogram'\n";

    std::vector<std::pair<double, double>> plotData;

    // Prepare data for plotting
    if(type == "normal") {
        std::vector<std::pair<double, int>> plotData;
        for (int i = 0; i < histogramData.rows(); ++i) {
            double binCenter = histogramData(i, 0); // First column: bin center
            int frequency = static_cast<int>(histogramData(i, 1)); // Second column: frequency
            plotData.emplace_back(binCenter, frequency);
        }
    }

    else {
        std::vector<std::pair<double, double>> plotData;
        for (int i = 0; i < normalizedData.cols(); ++i) {
            double binIndex = static_cast<double>(i); // Bin index
            double normalizedFrequency = normalizedData(0, i); // Assuming a single-row matrix
            plotData.emplace_back(binIndex, normalizedFrequency);
        }
    }
    gp.send1d(plotData);

    // Save plot if saveAs is provided
    if (!saveAs.empty()) {
        configureOutput(saveAs);
        gp << "replot\n";
        gp << "unset output\n";
    }
}

// Plot Line
void Visualizer::plotLine(const std::vector<double>& data, const std::string& saveAs) {
    Gnuplot gp;
    gp << "set title 'Line Plot'\n";
    gp << "set xlabel 'Index'\n";
    gp << "set ylabel 'Value'\n";
    gp << "plot '-' with lines title 'Data'\n";

    // Prepare data for plotting
    std::vector<std::pair<int, double>> plotData;
    for (size_t i = 0; i < data.size(); ++i) {
        plotData.emplace_back(static_cast<int>(i), data[i]);
    }

    gp.send1d(plotData);

    // Save plot if saveAs is provided
    if (!saveAs.empty()) {
        configureOutput(saveAs);
        gp << "replot\n";
        gp << "unset output\n";
    }
}

// Plot Scatter
void Visualizer::plotScatter(const std::vector<double>& x, const std::vector<double>& y, const std::string& saveAs) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }

    Gnuplot gp;
    gp << "set title 'Scatter Plot'\n";
    gp << "set xlabel 'X'\n";
    gp << "set ylabel 'Y'\n";
    gp << "set style line 1 lc rgb 'red' pt 7 ps 1.5\n";
    gp << "plot '-' using 1:2 with points ls 1 title 'Data'\n";

    // Prepare data for plotting
    std::vector<std::pair<double, double>> plotData;
    for (size_t i = 0; i < x.size(); ++i) {
        plotData.emplace_back(x[i], y[i]);
    }

    gp.send1d(plotData);

    // Save plot if saveAs is provided
    if (!saveAs.empty()) {
        configureOutput(saveAs);
        gp << "replot\n";
        gp << "unset output\n";
    }
}

// Summary Statistics
void Visualizer::printSummary(const std::vector<double>& data) {
    if (data.empty()) {
        std::cout << "Data is empty. Cannot compute statistics.\n";
        return;
    }

    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    double mean = sum / data.size();

    double variance = 0.0;
    for (double value : data) {
        variance += (value - mean) * (value - mean);
    }
    variance /= data.size();

    double minValue = *std::min_element(data.begin(), data.end());
    double maxValue = *std::max_element(data.begin(), data.end());

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Summary Statistics:\n";
    std::cout << "Mean: " << mean << "\n";
    std::cout << "Variance: " << variance << "\n";
    std::cout << "Min: " << minValue << "\n";
    std::cout << "Max: " << maxValue << "\n";
}
