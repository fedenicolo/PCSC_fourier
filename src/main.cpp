#include "Visualizer.h"
#include "Histogram.h"
#include "Fourier.h"
#include "PNGOutput.h"
#include <Eigen/Dense>
#include <filesystem>
#include <iostream>

int main() {
    // Ensure the results directory exists
    std::string resultsDir = "results";
    if (!std::filesystem::exists(resultsDir)) {
        std::filesystem::create_directory(resultsDir);
        std::cout << "Created 'results' directory." << std::endl;
    }

    // Example histogram data
    std::vector<double> histogramDataVec = {1.0, 2.0, 3.0, 3.5, 4.0, 5.5, 6.0, 7.0, 9.0};

    // Convert to Eigen::MatrixXd
    Eigen::MatrixXd histogramData(1, histogramDataVec.size());
    for (size_t i = 0; i < histogramDataVec.size(); ++i) {
        histogramData(0, i) = histogramDataVec[i];
    }

    // Create Histogram and Visualizer instances
    Histogram histogram(3, histogramData.minCoeff(), histogramData.maxCoeff());
    histogram.compute(histogramData);

    Visualizer visualizer;

    // Plot histogram (frequency)
    visualizer.plotHistogram(histogram, "results/histogram_freq.png", "normal");

    // Plot histogram (normalized)
    visualizer.plotHistogram(histogram, "results/histogram_normalized.png", "normalized");

    // Generate random data for a stem plot
    Eigen::MatrixXd randomStemData = Eigen::VectorXd::LinSpaced(20, 0, 19).array().sin().matrix();
    visualizer.plotStem(randomStemData, "results/stem_plot.png");

    // Test PNG Output
    Eigen::MatrixXd randomImage = Eigen::MatrixXd::Random(256, 256).array() * 127.5 + 127.5; // Normalize to [0, 255]
    PNGOutput pngOutput("results/random_image.png");
    pngOutput.save(randomImage);

    std::cout << "Testing Fourier Transform..." << std::endl;
    Fourier fourier(randomImage, true);
    fourier.transform();
    Eigen::MatrixXcd fourierTransform = fourier.get_fft_result<double>();
    std::cout << "Fourier Transform: " << fourierTransform << std::endl;
    std::cout << "Fourier Transform Shape: (" << fourierTransform.rows() << "x" << fourierTransform.cols() << ")" << std::endl;


    std::cout << "All tasks completed successfully!" << std::endl;
    return 0;
}
