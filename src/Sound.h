#ifndef SOUND_H
#define SOUND_H

#include "Input.h"

class Sound : public Input {
    using Input::Input;
public:
        virtual int getSampleRate() const = 0;
        virtual int getNumChannels() const = 0;
};
#endif