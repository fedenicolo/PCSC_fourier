

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <vector>
#include <string>
#include "Histogram.h"

class Visualizer {
public:
    // Constructor
    Visualizer();

    // Methods for plotting
    void plotHistogram(const Histogram& histogram, const std::string& saveAs = "", const std::string& type = "");
    void plotLine(const std::vector<double>& data, const std::string& saveAs = "");
    void plotScatter(const std::vector<double>& x, const std::vector<double>& y, const std::string& saveAs = "");
    void plotStem(const Eigen::MatrixXd& data, const std::string& saveAs);
    // Summary statistics
    void printSummary(const std::vector<double>& data);

private:
    // Private helper methods for plot configurations
    void configureLinePlot(const std::vector<double>& data);
    void configureScatterPlot(const std::vector<double>& x, const std::vector<double>& y);

    // Output configuration
    void configureOutput(const std::string& saveAs);
};

#endif

