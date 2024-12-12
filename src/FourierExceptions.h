#ifndef FOURIER_EXCEPTIONS_H
#define FOURIER_EXCEPTIONS_H

#include <exception>
#include <stdexcept>

/** @class EMPTY_SIGNAL
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a program is trying to compute the FFT of an empty signal.
 */
class EMPTY_SIGNAL : public std::exception{
    public:
        const char* what(){
            return "The signal is empty.";
        }
};

/** @class EMPTY_INVERSE_RESULT
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when a signal computed using the inverse FFT algorithm is empty.
 */
class EMPTY_INVERSE_RESULT : public std::exception{
    public:
        const char* what(){
            return "The inverse result is empty.";
        }
};

/** @class EMPTY_FFT_RESULT
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the computed FFT of a signal is empty.
 */
class EMPTY_FFT_RESULT : public std::exception{
    public:
        const char* what(){
            return "The FFT Result is empty. Either no previous transform was taken or the transform has been deleted.";
        }
};

/** @class NOT_POWER_OF_TWO
 *  @brief This is an exception class that inherits from std::exception. It is used to throw a custom exception
 *  when the number of samples of a signal is not a power of two.
 */
class NOT_POWER_OF_TWO : public std::exception{
    public:
        const char* what(){
            return "The signal is not a power of 2.";
        }
};

#endif