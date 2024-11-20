//
// Created by nicol on 20/11/2024.
//
#include "Visualizer.h"
#include "Histogram.h" // For using the Histogram class
#include <iostream>
#include <iomanip>
#include <numeric>
#include <stdexcept>
#include<thread>
#include<chrono>

// Constructor with persistent Gnuplot settings
Visualizer::Visualizer() {
    gp << "set grid\n";
}

// Histogram
void Visualizer::plotHistogram(const std::vector<double>& data, int bins) {
    Gnuplot localGp;
    localGp << "set terminal wxt persist\n";
    Histogram histogram(bins, *std::min_element(data.begin(), data.end()), *std::max_element(data.begin(), data.end()));
    histogram.compute(data);
    histogram.visualize();
}

// Line Graph
void Visualizer::plotLine(const std::vector<double>& data) {
    Gnuplot localGp;
    localGp << "set terminal wxt 1 size 800,600 persist enhanced title 'Line Plot'\n";
    configureLinePlot(data, localGp); //private configuration method

    std::vector<std::pair<int, double>> plotData;

    for (size_t i = 0; i < data.size(); ++i) {
        plotData.emplace_back(static_cast<int>(i), data[i]);
    }

    gp << "plot '-' with lines ls 1 title 'Data'\n";
    gp.send1d(plotData);

    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// Plot Scatter
void Visualizer::plotScatter(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }
    Gnuplot localGp2;
    localGp2 << "set terminal wxt 2 size 800,600 persist enhanced title 'Scatter Plot'\n";
    configureScatterPlot(x, y, localGp2);

    std::vector<std::pair<double, double>> plotData;

    for (size_t i = 0; i < x.size(); ++i) {
        plotData.emplace_back(x[i], y[i]);
    }

    gp << "plot '-' with points ls 2 title 'Data'\n";
    gp.send1d(plotData);
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// Print Summary Statistics
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


// Private: Gnuplot for line plots
void Visualizer::configureLinePlot(const std::vector<double>& data, Gnuplot& localGp) {
    localGp << "set style line 1 lc rgb 'blue' lt 1 lw 2\n";
    localGp << "set xlabel 'Index' font ',12'\n";
    localGp << "set ylabel 'Value' font ',12'\n";
    localGp << "set title 'Line Plot' font ',14'\n";
}

// Private: Gnuplot for scatter plots
void Visualizer::configureScatterPlot(const std::vector<double>& x, const std::vector<double>& y, Gnuplot& localGp) {
    localGp << "set style line 2 lc rgb 'red' pt 7 ps 1.5\n"; // Red points
    localGp << "set xlabel 'X' font ',12'\n";
    localGp << "set ylabel 'Y' font ',12'\n";
    localGp << "set title 'Scatter Plot' font ',14'\n";
}
