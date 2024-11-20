#include "Histogram.h"
#include <vector>
#include <iostream>


#include "Visualizer.h"
#include <vector>
#include <iostream>

int main() {
    try {
        // Example sound data (amplitudes between -1.0 and 1.0)
        std::vector<double> soundData = {-0.5, 0.1, 0.5, 0.3, -0.2, 0.8, 0.9, -0.7};

        // Example image data (pixel intensities between 0 and 255)
        std::vector<double> imageData = {50, 150, 200, 100, 255, 0, 100, 150};

        // Example frequency data (magnitudes for a simulated Fourier Transform)
        std::vector<double> frequencyData = {0.2, 0.5, 0.8, 0.9, 0.7, 0.4, 0.1};

        // Example scatter plot data (x, y)
        std::vector<double> scatterX = {1, 2, 3, 4, 5};
        std::vector<double> scatterY = {5, 4, 3, 2, 1};

        // Instantiate the Visualizer
        Visualizer visualizer;

        // Test Histogram for sound data
        std::cout << "Visualizing Histogram for Sound Data...\n";
        // visualizer.plotHistogram(soundData, 10);

        // Test Histogram for image data
        std::cout << "Visualizing Histogram for Image Data...\n";
        // visualizer.plotHistogram(imageData, 8);

        // Test Line Plot for frequency data
        std::cout << "Visualizing Line Plot for Frequency Data...\n";
        visualizer.plotLine(frequencyData);

        // Test Scatter Plot for (x, y) data
        std::cout << "Visualizing Scatter Plot...\n";
        visualizer.plotScatter(scatterX, scatterY);

        // Print Summary Statistics for sound data
        std::cout << "Printing Summary Statistics for Sound Data...\n";
        visualizer.printSummary(soundData);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


/*
int main() {
    try {
        // Example sound data (amplitudes between -1.0 and 1.0)
        std::vector<double> soundData = {-0.5, 0.1, 0.5, 0.3, -0.2, 0.8, 0.9, -0.7};

        // Example image data (pixel intensities between 0 and 255)
        std::vector<double> imageData = {50, 150, 200, 100, 255, 0, 100, 150};

        // Test Histogram for sound (10 bins, range -1.0 to 1.0)
        std::cout << "Testing Histogram for Sound Data...\n";
        Histogram soundHistogram(10, -1.0, 1.0);
        soundHistogram.compute(soundData);
        soundHistogram.visualize(); // Visualize the histogram with Gnuplot

        // Test Histogram for image (256 bins, range 0 to 255)
        std::cout << "Testing Histogram for Image Data...\n";
        Histogram imageHistogram(256, 0, 255);
        imageHistogram.compute(imageData);
        imageHistogram.visualize(); // Visualize the histogram with Gnuplot

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
*/
