#include "Visualizer.h"
#include "Histogram.h"
#include "Fourier.h"
#include <vector>

int main() {
    Visualizer visualizer;

    // Example histogram data
    std::vector<double> histogramData = {1.0, 2.0, 3.0, 3.5, 4.0, 5.5, 6.0, 7.0};
    visualizer.plotHistogram(histogramData, 8, "results/histogram.png");

    // Example line data
    std::vector<double> lineData = {1.0, 1.5, 2.0, 2.5, 3.0, 3.5};
    visualizer.plotLine(lineData, "results/line_plot.png");

    // Example scatter data
    std::vector<double> scatterX = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> scatterY = {4.0, 3.0, 2.0, 1.0};
    visualizer.plotScatter(scatterX, scatterY, "results/scatter_plot.png");

<<<<<<< HEAD:main.cpp
    // Summary statistics
    visualizer.printSummary(histogramData);

=======
        Fourier<double> fourier(soundData);
        fourier.print_signal();
        fourier.transform(0);
        fourier.inverse_transform();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
>>>>>>> main:src/main.cpp
    return 0;
}
