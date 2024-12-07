#include <iostream>
#include <memory>
#include <string>
#include <filesystem>
#include "BMPInput.h"
#include "WAVInput.h"
#include "Visualizer.h"
#include "CSVOutput.h"

// Helper function to extract file extension
std::string getFileExtension(const std::string& filepath) {
    size_t dotPos = filepath.find_last_of(".");
    if (dotPos == std::string::npos) return "";
    return filepath.substr(dotPos + 1);
}

void performHistogram(const Eigen::MatrixXd& data, Visualizer& visualizer) {
    // Number of bins for histogram (can be adjusted or asked interactively)
    int numBins = 10;
    std::string savePath = "histogram_before_fourier.png";

    // Use the Visualizer class to plot and save the histogram
    visualizer.plot(data, "histogram", numBins, savePath);
    std::cout << "Histogram visualized and saved as " << savePath << ".\n";
}

int main() {
    while (true) {
        // Step 1: Load file
        std::unique_ptr<Input> input = nullptr;
        Eigen::MatrixXd data;
        Eigen::MatrixXd transformedData;
        Visualizer visualizer;

        std::string filePath;
        std::cout << "\nEnter the input file path: ";
        std::cin >> filePath;

        // Recognize file type
        std::string extension = getFileExtension(filePath);
        if (extension == "bmp") {
            input = std::make_unique<BMPInput>(filePath);
        } else if (extension == "wav") {
            input = std::make_unique<WAVInput>(filePath);
        } else {
            std::cout << "Unsupported file type. Supported types are BMP and WAV. Try again.\n";
            continue;
        }

        input->readData();
        data = input->getData();
        std::cout << "File loaded successfully.\n";

        // Step 2: Ask for histogram
        char choice;
        std::cout << "Do you want to compute and visualize a histogram before the Fourier Transform? (y/n): ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            performHistogram(data, visualizer);
        }

        // Step 3: Perform Fourier Transform
        // #### Fourier Transform placeholder
        std::cout << "Fourier Transform completed.\n";

        // Step 4: Menu for additional operations
        while (true) {
            std::cout << "\nWhat would you like to do next?\n";
            std::cout << "1. Compute histogram after fourier\n";
            std::cout << "2. Apply filters\n";
            std::cout << "3. Save data\n";
            std::cout << "4. Finish with this file\n";
            std::cout << "Enter your choice: ";
            int option;
            std::cin >> option;

            switch (option) {
                case 1:
                    performHistogram(data, visualizer);
                    break;
                case 2:
                    // #### Filter placeholder
                    std::cout << "Filter applied.\n";
                    break;
                case 3: {
                    std::string outputPath;
                    std::cout << "Enter output file path (e.g., output.csv): ";
                    std::cin >> outputPath;

                    CSVOutput output(outputPath);
                    /// ###apply antitransform
                    output.save(data);
                    std::cout << "Data saved to " << outputPath << ".\n";
                    break;
                }
                case 4:
                    goto RestartOrExit;
                default:
                    std::cout << "Invalid choice. Try again.\n";
            }
        }

    RestartOrExit:
        // Step 5: Restart or exit
        char restartChoice;
        std::cout << "\nDo you want to process another file? (y/n): ";
        std::cin >> restartChoice;
        if (restartChoice != 'y' && restartChoice != 'Y') {
            std::cout << "Exiting program. Goodbye!\n";
            return 0;
        }
    }
}
