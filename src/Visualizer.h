#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <Eigen/Dense>
#include "Histogram.h"
#include <string>
#include <vector>
#include "gnuplot-iostream.h"

class Visualizer {
public:
    // Constructor
    Visualizer();

    // Generic plot function
    void plot(const Eigen::MatrixXd& data, const std::string& plotType, int numBins = 10, const std::string& saveAs = "");
    void printSummary(const Eigen::MatrixXd& data) const;

private:
    // Helper functions for different plot types
    void plotHistogram(const Eigen::MatrixXd& data, int numBins, const std::string& saveAs);
    void plotLine(const Eigen::MatrixXd& data, const std::string& saveAs);
    void plotScatter(const Eigen::MatrixXd& data, const std::string& saveAs);

    // Output configuration (for saving images)
    void configureOutput(const std::string& saveAs);
};

#endif // VISUALIZER_H
