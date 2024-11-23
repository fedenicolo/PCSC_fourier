//
// Created by nicol on 21/11/2024.
//
#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>

class Output {
public:
    // Constructor
    Output();

    // Display the plot in a Gnuplot window
    void display(const std::string& script);

    // Save the plot as a PNG
    void saveAsPNG(const std::string& script, const std::string& filename);

private:
    // Utility to create a temporary script file
    void createScriptFile(const std::string& script, const std::string& filename);
};

#endif

