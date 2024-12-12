#ifndef OUTPUT_H
#define OUTPUT_H

#include <string>
#include <Eigen/Dense>

/**
 * @class Output
 * @brief A base class for saving data in different formats.
 *
 * The `Output` class is as an abstract base class for saving data in various formats. It provides a common
 * interface for saving data.
 *
 * The `Output` class is designed to be inherited by other classes (e.g.,`MP3Output`) that handle specific
 * file formats.
 *
 */

class Output {
protected:
    std::string outputFilePath; ///< File path where the data will be saved.
public:
    /**
     * @brief Constructs an Output object with the specified output file path.
     *
     * This constructor initializes the `Output` object and sets the file path where
     * the output data will be saved.
     *
     * @param path The file path where the output will be saved (std::string).
     */
    explicit Output(const std::string& path);

    /**
     * @brief Virtual destructor for Output class.
     *
     * The destructor is marked virtual to ensure that the destructor of a derived class is properly called
     * when an object is deleted.
     */
    virtual ~Output() = default;

    /**
     * @brief Saves the data in the desired format.
     *
     * This is a pure virtual method that must be implemented by derived classes. It allows saving the data
     * passed in the form of an Eigen matrix to the output file.
     *
     * @param data The Eigen Matrix containing the data that will be saved in the output file (Eigen::MatrixXd).
     */
    virtual void save(const Eigen::MatrixXd& data) = 0;

    /**
     * @brief Gets the file path where the data will be saved.
     *
     * This method returns the output file path.
     *
     * @return std::string The path of the output file where the data will be saved.
     */
    std::string getOutputFilePath() const;

    /**
     * @brief Sets the path of the output file where the data will be saved.
     *
     * This method allows the user to set or update the file path where the output data
     * will be saved.
     *
     * @param path The path where the data will be saved (std::string).
     */
    void setOutputFilePath(const std::string& path);
};

#endif // OUTPUT_H
