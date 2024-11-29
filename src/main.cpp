#include "Visualizer.h"
#include "Histogram.h"
#include "Fourier.h"
#include <vector>
/*
int main() {
    Visualizer visualizer;

    // Example histogram data
    std::vector<double> histogramData = {1.0, 2.0, 3.0, 3.5, 4.0, 5.5, 6.0, 7.0};
    //visualizer.plotHistogram(histogramData, 8, "results/histogram.png");

    // Example line data
    std::vector<double> lineData = {1.0, 1.5, 2.0, 2.5, 3.0, 3.5};
    visualizer.plotLine(lineData, "results/line_plot.png");

    // Example scatter data
    std::vector<double> scatterX = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> scatterY = {4.0, 3.0, 2.0, 1.0};
    visualizer.plotScatter(scatterX, scatterY, "results/scatter_plot.png");


    // Summary statistics
    visualizer.printSummary(histogramData);
    
    return 0;
} */

#include <Eigen/Dense>
#include <filesystem>
#include "PNGOutput.h"
int main() {
    // Ensure the results directory exists
    std::string resultsDir = "results";
    if (!std::filesystem::exists(resultsDir)) {
        std::filesystem::create_directory(resultsDir);
        std::cout << "Created 'results' directory." << std::endl;
    }

    // Example histogram data
    std::vector<double> histogramDataVec = {1.0, 2.0, 3.0, 3.5, 4.0, 5.5, 6.0, 7.0};

    // Convert to Eigen::MatrixXd
    Eigen::MatrixXd histogramData(1, histogramDataVec.size());
    for (size_t i = 0; i < histogramDataVec.size(); ++i) {
        histogramData(0, i) = histogramDataVec[i];
    }

    // Create Histogram and Visualizer instances
    Histogram histogram(3, histogramData.minCoeff(), histogramData.maxCoeff());
    histogram.compute(histogramData);

    Visualizer visualizer;
    visualizer.plotHistogram(histogram, "results/histogram.png", "freq");

    // Test PNG Output
    Eigen::MatrixXd randomImage = Eigen::MatrixXd::Random(256, 256).array() * 127.5 + 127.5; // Normalize to [0, 255]
    PNGOutput pngOutput("results/random_image.png");
    pngOutput.save(randomImage);

    return 0;
}


