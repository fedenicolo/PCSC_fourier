//
// Created by nicol on 29/11/2024.
//
#include "CSVOutput.h"
#include "ImageExceptions.h"
#include <Eigen/Dense>
#include <iostream>

CSVOutput::CSVOutput(const std::string& path) : Output(path) {}

void CSVOutput::save(const Eigen::MatrixXd& data) {
    std::ofstream file(outputFilePath);
    if (!file.is_open()) {
        throw INVALID_CSV_OPEN("Cannot open file: " + outputFilePath);
    }

    //Ask the user if he wants pixel values to be normalized or not in his CSV file
    bool image = false;
    bool normalized = false;

    std::string input;


    while (true) {
        std::cout << "Is the file an image? (yes/no): ";
        std::getline(std::cin, input);

        if (input == "yes") {
            image = true;
            break;  // Exit the loop when valid input is given
        } else if (input == "no") {
            image = false;
            break;  // Exit the loop when valid input is given
        } else {
            std::cout << "Invalid response. Please type 'yes' or 'no'." << std::endl;
        }
    }

    //If the file is an image, ask about scaling
    if (image) {
        while (true) {
            std::cout << "Do you want pixel values between 0 and 1? (yes/no): ";
            std::getline(std::cin, input);

            if (input == "yes") {
                normalized = true;
                break;  // Exit the loop when valid input is given
            } else if (input == "no") {
                normalized = false;
                break;  // Exit the loop when valid input is given
            } else {
                std::cout << "Invalid response. Please type 'yes' or 'no'." << std::endl;
            }
        }
    }

    // Output the results
    std::cout << "Summary:" << std::endl;
    std::cout << "Is the file an image? " << (image ? "Yes" : "No") << std::endl;
    if (image) {
        std::cout << "Pixel values scale: " << (normalized ? "0 to 1" : "0 to 255") << std::endl;
    }

    for (int i = 0; i < data.rows(); ++i) {
        for (int j = 0; j < data.cols(); ++j) {
            //Scale the pixel values back to [0,255] if requested by the user
            if (image & !normalized){
                file << 255*data(i, j);
            } else {
                file << data(i, j);
            }
            if (j < data.cols() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }
    file.close();
}
