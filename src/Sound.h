#ifndef SOUND_H
#define SOUND_H

#include "Input.h"

/**
 * @class Sound
 * @brief A base class for representing sound data.
 *
 * This class is as wrapper class for functionalities related to audio signals in the project.
 * It inherits from the `Input` class and provides a common interface for retrieving basic information about audio
 * files, such as the sampling rate and the number of audio channels.
 *
 * The `Sound` class is designed to be inherited by other classes (e.g., `WAVInput`)
 * that handle specific sound file formats.
 *
 * @see Input
 */

class Sound : public Input {
    using Input::Input; ///< Inherit the constructor from the Input class.
public:
    /**
     * @brief Gets the sample rate of the audio file.
     *
     * This is a pure virtual function that needs to be implemented by derived classes.
     * It returns the sample rate of the audio file.
     *
     * @return int The sample rate of the audio file.
     */
    virtual int getSampleRate() const = 0;

    /**
     * @brief Gets the number of audio channels of the audio file.
     *
     * This is a pure virtual function that needs to be implemented by derived classes.
     * It returns the number of audio channels of the audio file.
     *
     * @return int The number of audio channels of the audio file.
     */
    virtual int getNumChannels() const = 0;
};
#endif