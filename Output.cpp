#include "Output.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

// Constructor
Output::Output() {}

// Display the plot in a Gnuplot window
void Output::display(const std::string& script) {
    std::string tempFilename = "temp_script.gnuplot";
    createScriptFile(script, tempFilename);

    // Execute the script in Gnuplot
    int result = std::system(("gnuplot -persist " + tempFilename).c_str());
    if (result != 0) {
        throw std::runtime_error("Failed to execute Gnuplot script.");
    }
}

// Save the plot as a PNG
void Output::saveAsPNG(const std::string& script, const std::string& filename) {
    std::string tempFilename = "temp_script.gnuplot";
    std::ofstream scriptFile(tempFilename);
    if (!scriptFile) {
        throw std::runtime_error("Failed to create temporary script file.");
    }

    // Add Gnuplot commands to set PNG output
    scriptFile << "set terminal png size 800,600\n";
    scriptFile << "set output '" << filename << "'\n";
    scriptFile << script;
    scriptFile.close();

    // Execute the script in Gnuplot
    int result = std::system(("gnuplot " + tempFilename).c_str());
    if (result != 0) {
        throw std::runtime_error("Failed to execute Gnuplot script.");
    }
}

// Utility to create a temporary script file
void Output::createScriptFile(const std::string& script, const std::string& filename) {
    std::ofstream scriptFile(filename);
    if (!scriptFile) {
        throw std::runtime_error("Failed to create temporary script file.");
    }
    scriptFile << script;
    scriptFile.close();
}
