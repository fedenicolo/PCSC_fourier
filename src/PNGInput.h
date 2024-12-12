//
// Created by nicol on 24/11/2024.
//
#ifndef PNGINPUT_H
#define PNGINPUT_H

#include "Image.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

/**
 * @class PNGInput
 * @brief A class to read and process ".png" image files.
 *
 * This class allows to easily extract pixel values from a ".png" file.
 * It provides methods to access the pixels values, as well as basic information about the image such as its height,
 * its width and its number of channels.
 *
 * This class inherits from the Image class and overrides its methods to provide specific functionalities for
 * ".png" files.
 *
 * @see Image
 */

class PNGInput : public Image {
public:
    /**
     * @brief Constructs a PNGInput object for the given file.
     *
     * @param filepath The path to the PNG file to be read (std::string).
     */
    explicit PNGInput(const std::string& filepath);

    /**
     * @brief Reads the image data from the PNG file.
     *
     * This method reads the pixel values from the ".png" file, converts the pixel values to grayscale if the image has
     * multiple channels and stores the normalized grayscale pixel values locally in the `ImageData` matrix.
     * The pixel data is accessed using the getData method.
     *
     * @throws INVALID_PNG_READ if the file could not be loaded.
     *
     * @see ImageExceptions.h for more details
     */
    void readData() override;

    /**
     * @brief Gets the width of the image.
     *
     * This method returns the width of the image in pixels.
     *
     * @return int The width of the image in pixels.
     */
    int getWidth() const {return width;}

    /**
     * @brief Gets the height of the image.
     *
     * This method returns the height of the image in pixels.
     *
     * @return int The height of the image in pixels.
     */
    int getHeight() const {return height;}

    /**
     * @brief Gets the number of channels of the image.
     *
     * This method returns the number of channels in the image.
     *
     * @return int The number of channels of the image.
     */
    int getChannels() const {return num_channels;}

    /**
     * @brief Gets the normalized grayscale pixel values stored in the object as an Eigen matrix.
     *
     * This method gets the normalized grayscale pixel values stored in the object locally in the form of an
     * Eigen matrix of doubles.
     *
     * @return Eigen::MatrixXd The matrix containing the pixel values.
     *
     */
    Eigen::MatrixXd getData() override { return ImageData;}

private:
    int width;                 ///< The width of the image in pixels.
    int height;                ///< The height of the image in pixels.
    int num_channels;          ///< The number of channels in the image
    Eigen::MatrixXd ImageData; ///< Matrix to store the pixel values.

    /**
     * @brief Converts the pixel values to grayscale.
     *
     * This method converts a colored image to grayscale using the luminance formula and normalizes the pixel values.
     *
     * @param Image A pointer to the raw pixel values in the form of an unsigned char array (unsigned char*).
     * @return Eigen::MatrixXd A matrix containing the normalized grayscale pixel values.
     * @throws INVALID_PNG_NUM_CHANNELS if the image has more than 3 channels.
     * @see ImageExceptions.h for more details
     */
    Eigen::MatrixXd convertToGrayscale(const unsigned char* Image) const;
};

#endif // PNGINPUT_H
