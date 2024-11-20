//
// Created by nicol on 20/11/2024.
//
#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <vector>
#include "gnuplot-iostream.h"

class Visualizer {
public:
    // Constructor
    Visualizer();

    //histogram
    void plotHistogram(const std::vector<double>& data, int bins);

    //line plot
    void plotLine(const std::vector<double>& data);

    //scatter plot
    void plotScatter(const std::vector<double>& x, const std::vector<double>& y);

    // Print summary statistics
    void printSummary(const std::vector<double>& data);

private:
    void configureLinePlot(const std::vector<double>& data, Gnuplot& localGp);
    void configureScatterPlot(const std::vector<double>& x, const std::vector<double>& y, Gnuplot& localGp);

    Gnuplot gp;
};

#endif

