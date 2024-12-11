#ifndef FOURIER_EXCEPTIONS_H
#define FOURIER_EXCEPTIONS_H

#include <exception>
#include <stdexcept>


class EMPTY_SIGNAL : public std::exception{
    public:
        const char* what(){
            return "The signal is empty.";
        }
};

class EMPTY_INVERSE_RESULT : public std::exception{
    public:
        const char* what(){
            return "The inverse result is empty.";
        }
};

class EMPTY_FFT_RESULT : public std::exception{
    public:
        const char* what(){
            return "The FFT Result is empty. Either no previous transform was taken or the transform has been deleted.";
        }
};

class NOT_POWER_OF_TWO : public std::exception{
    public:
        const char* what(){
            return "The signal is not a power of 2.";
        }
};

#endif