//
// Created by nicol on 29/11/2024.
//
#ifndef PNGOUTPUT_H
#define PNGOUTPUT_H

#include "Output.h"
#include <Eigen/Dense>
#include <string>

/**
 * @class PNGOutput
 * @brief A class for saving data to a ".png" image file.
 *
 * This class saves data stored in an Eigen Matrix to a ".png" image file.
 * It inherits from the `Output` class.
 *
 * The data matrix provided is interpreted as normalized pixel values of a grayscale image. Therefore, the class is
 * only able to handle grayscale images (one image channel).
 *
 * @see Output
 */

class PNGOutput : public Output {
public:
    /**
     * @brief Constructs a PNGOutput object with the given file path.
     *
     * @param path The path of the ".png" where the image data will be saved (std::string).
     */
    explicit PNGOutput(const std::string& path);

    /**
     * @brief Saves the provided data as a ".png" image file.
     *
     * This method saves normalized pixel values stored in an Eigen Matrix to a ".png" image file.
     *
     * The function overrides the `save` method from the `Output` class.
     *
     * @param data The Eigen Matrix of normalized pixel values to be saved in the ".png" image file (Eigen::MatrixXd).
     * @note Make sure the pixel data is normalized and has only one channel. If not convert your pixel values to
     * grayscale and then normalize the pixel values to [0,1].
     * @throws INVALID_PNG_DIMENSIONS if the dimension of the image are not valid.
     * @throws PNG_SAVE_FAIL if the library failed to save the image.
     * @see ImageExceptions.h for more details.
     */
    void save(const Eigen::MatrixXd& data) override;
};

#endif // PNGOUTPUT_H

