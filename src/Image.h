#ifndef IMAGE_H
#define IMAGE_H

#include "Input.h"

/**
 * @class Image
 * @brief A base class for representing image data.
 *
 * This class is as wrapper class for functionalities related to image pixel values in the project.
 * It inherits from the `Input` class.
 *
 * The `Image` class is designed to be inherited by other classes (e.g., `PNGInput`) that handle specific
 * image file formats.
 *
 * @see Input
 */

class Image : public Input{
    using Input::Input; ///< Inherit the constructor from the Input class.
};
#endif