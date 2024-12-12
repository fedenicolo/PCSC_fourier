//
// Created by nicol on 29/11/2024.
//
#ifndef CSVOUTPUT_H
#define CSVOUTPUT_H

#include "Output.h"
#include <fstream>

/**
 * @class CSVOutput
 * @brief A class for saving data to a ".csv" file.
 *
 * This class saves data stored in an Eigen Matrix to a ".csv" file.
 * It inherits from the `Output` class.
 *
 * The data matrix provided is interpreted as normalized data either in [-1,1] for audio data or [0,1] for pixel data.
 *
 * @see Output
 */

class CSVOutput : public Output {
public:
    /**
     * @brief Constructs the `CSVOutput` object.
     *
     * This constructor initializes a CSVOutput object with the specified file path.
     *
     * @param path The file path of the output file where the data will be saved (std::string).
     */
    explicit CSVOutput(const std::string& path);

    /**
     * @brief Saves the given data to a CSV file.
     *
     * This function saves the provided `Eigen::MatrixXd` data to a CSV file.
     * The user is asked if the data should be treated as audio data or pixel values.
     * If the data should be treated as pixel values, the user is asked if he wants the pixel values to be stored
     * normalized or not.
     *
     * @param data The input data to be saved, represented as an `Eigen::MatrixXd` object.
     *
     * @throws INVALID_CSV_OPEN If the file cannot be opened for writing.
     *
     * @see ImageExceptions.h for the details.
     *
     */
    void save(const Eigen::MatrixXd& data) override;
};

#endif // CSVOUTPUT_H
