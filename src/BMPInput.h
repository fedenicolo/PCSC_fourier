//
// Created by nicol on 24/11/2024.
//
#ifndef BMPINPUT_H
#define BMPINPUT_H

#include "Image.h"
#include <Eigen/Dense>
#include <vector>
#include <string>

/**
 * @class BMPInput
 * @brief A class to read and process ".bmp" image files.
 *
 * This class allows to easily extract pixel values from a ".bmp" file.
 * It provides methods to access the pixels values, as well as basic information about the image such as its height
 * and its width.
 *
 * This class inherits from the Image class and overrides its methods to provide specific functionalities for
 * ".bmp" files.
 *
 * @see Image
 */

class BMPInput : public Image {
public:

    /**
     * @brief Constructs a BMPInput object for the given file.
     *
     * @param filepath The path to the BMP file to be read (std::string).
     */
    explicit BMPInput(const std::string& filepath);

    /**
    * @brief Reads the image data from the BMP file.
    *
    * This method reads the pixel values from the ".bmp" file, converts the pixel values to grayscale if the image has
    * multiple channels and stores the normalized grayscale pixel values locally in the `ImageData` matrix.
    * The pixel data is accessed using the getData method.
    *
    * @throws INVALID_BMP_FILE_OPEN if the file could not be opened.
    * @throws INVALID_BMP_BM if the file is not a ".bmp" file.
    * @throws INVALID_BMP_READ() if there was an issue during file reading.
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
    int getWidth() const{return width;}

    /**
     * @brief Gets the height of the image.
     *
     * This method returns the height of the image in pixels.
     *
     * @return int The height of the image in pixels.
     */
    int getHeight() const{return height;}

    /**
     * @brief Gets the normalized grayscale pixel values stored in the object as an Eigen matrix.
     *
     * This method gets the normalized grayscale pixel values stored in the object locally in the form of an
     * Eigen matrix of doubles.
     *
     * @return Eigen::MatrixXd The matrix containing the pixel values.
     */
    Eigen::MatrixXd getData() override { return ImageData;}

private:
    int width;                 ///< The width of the image in pixels.
    int height;                ///< The height of the image in pixels.
    Eigen::MatrixXd ImageData; ///< Matrix to store the pixel values.

    /**
     * @brief Converts the pixel values to grayscale.
     *
     * This method converts a colored image to grayscale using the luminance formula and normalizes the pixel values.
     *
     * @param pixelData An array of raw pixel values in the form of an unsigned char vector (std::vector<unsigned char>).
     * @return Eigen::MatrixXd A matrix containing the normalized grayscale pixel values.
     */
    Eigen::MatrixXd convertToGrayscale(const std::vector<unsigned char>& pixelData) const;
};

#endif // BMPINPUT_H
