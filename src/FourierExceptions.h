#include <exception>
#include <stdexcept>


class EMPTY_FFT_RESULT : public std::exception{
    public:
        char* what(){
            return "The FFT Result is empty. Either no previous transform was taken or the transform has been deleted.";
        }
};

class NOT_POWER_OF_TWO : public std::exception{
    public:
        char* what(){
            return "The signal is not a power of 2.";
        }
};