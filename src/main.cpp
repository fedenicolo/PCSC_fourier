#include "Histogram.h"
#include "Fourier.h"
#include <vector>
#include <iostream>

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

        Fourier<double> fourier(soundData);
        fourier.print_signal();
        fourier.transform(0);
        fourier.inverse_transform();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

