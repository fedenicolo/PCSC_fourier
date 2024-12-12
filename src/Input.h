//
// Created by nicol on 24/11/2024.
//
#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <Eigen/Dense>

/**
 * @class Input
 * @brief A base class for reading data in different formats.
 *
 * The `Input` class is as an abstract base class for reading data in various formats. It provides a common
 * interface for reading data.
 *
 * The `Input` class is designed to be inherited by other classes (e.g.,`MP3Input`) that handle specific
 * file formats.
 *
 */

class Input {
protected:
    std::string filepath; ///< File path of the file that will be read.

public:
    /**
     * @brief Constructs an Input object with the specified file path.
     *
     * This constructor initializes the `Input` object and sets the file path to the file that will be read.
     *
     * @param path The file path to the file that will be read (std::string).
     */
    explicit Input(const std::string& path);

    /**
     * @brief Virtual destructor for Input class.
     *
     * The destructor is marked virtual to ensure that the destructor of a derived class is properly called
     * when an object is deleted.
     */
    virtual ~Input() = default;

    /**
     * @brief Reads data from a file.
     *
     * This is a pure virtual method that must be implemented by derived classes. It allows reading from a file
     * specified when constructing the object. The data is stored locally as an Eigen Matrix of doubles.
     */
    virtual void readData() = 0;

    /**
     * @brief Gets the input file path.
     *
     * This method returns file path of the file that will be read.
     *
     * @return std::string The path of the input file.
     */
    std::string getFilepath() const;

    /**
     * @brief Checks if the filepath to the file is valid.
     *
     * This function is used to verify whether the file path attribute of the Input object is valid or not.
     *
     * @return True if the file path is valid, false otherwise.
     */
    virtual bool isValidFile() const;

    /**
     * @brief Gets the input data stored locally in the Input object as an Eigen matrix.
     *
     * This is a pure virtual method that must be implemented by derived classes. It allows retrieving the input data
     * stored locally in the Input object as an Eigen matrix.
     *
     * @return Eigen::MatrixXd The matrix containing the data loaded from the input file.
     */
    virtual Eigen::MatrixXd getData() = 0;
};

#endif // INPUT_H
