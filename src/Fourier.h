//A H file for the Fourier Transform class.
#ifndef FOURIER_H
#define FOURNER_H

#include "Eigen/Dense"
#include "Image.h"
#include "Sound.h"
#include <complex>
#include <math.h>
#include <vector>
#include <iostream>
#include "FourierExceptions.h"
#include <tuple>

/** @class Fourier
 *  @brief This is a class created to compute the Fourier Transform of an image or a signal. It can accept both Image and Sound
 * classes as well as a generic Eigen Matrix. However if you provide an Eigen matrix you also need to specify whether
 * the data provided is an image or a sound, as computing the fourier transforms of those are different. 
 */
class Fourier{
    double PI = 3.141592653589793238460;
    
    public:
        Fourier();
        Fourier(Image& input);
        Fourier(Sound& input);
        Fourier(Eigen::MatrixXd input, bool isImage);
        
        template <typename T>
        void load_signal(const Eigen::MatrixBase<T>& input, bool isImage);
        void transform(std::tuple<int, int> padding);

        template <typename T>
        void load_transform(const Eigen::Matrix<std::complex<T>, -1, -1>& input, bool isImage);
        void inverse_transform();
        
        template <typename T>
        Eigen::Matrix<std::complex<T>, -1, -1> get_fft_result();

        template <typename T>
        Eigen::Matrix<T, -1, -1> get_inverse_result_padded();

        template <typename T>
        Eigen::Matrix<T, -1, -1> get_inverse_result();


        Eigen::Matrix<std::complex<double>, -1, -1> shift(const Eigen::Matrix<std::complex<double>, -1, -1>& matrix);

        Eigen::Matrix<std::complex<double>, -1, -1> unshift(const Eigen::Matrix<std::complex<double>, -1, -1>& matrix);
        
        template <typename T>
        Eigen::Matrix<T, -1, -1> get_signal();
        void print_signal();    
        void __pad_signal(std::tuple<int, int> padding);
        bool __is_power_of_2(int v);
        unsigned int __next_power_of_2(unsigned int x);
    
    private:
        //Private variables
        Eigen::MatrixXd signal;
        Eigen::MatrixXcd fft_result;
        Eigen::MatrixXcd inverse_result;
        int signal_rows;
        int signal_cols;

        //True for image false for sound
        bool image;

        //Private helper functions
        void __fft1d(Eigen::Matrix<std::complex<double>, 1, -1>& arr);
        void __ifft1d(Eigen::Matrix<std::complex<double>, 1, -1>& arr);
};

/** @brief This is the base constructor for the Fourier Class. This takes no inputs but you must load a signal using
the load_signal function 
 */
Fourier::Fourier(){};

/** @brief This constructor gets called if you pass in a class that is a daughter of the Image class. This can throw and
error if the input you provided cannot be read. For more information on these errors have a look at the
"ImageExceptions.h" file.
 * @param input an Image object or any image that is a daughter of the Image class.
 * @throws exception if something goes wrong. Have a look at the "ImageExceptions.h" file for all the failure modes.
 */
Fourier::Fourier(Image& input){
    input.readData();
    Fourier::load_signal(input.getData(), true);
}


/** @brief This constructor gets called if you pass in a class that is a daughter of the Sound class. This can throw and
error if the input you provided cannot be read. For more information on these errors have a look at the
"AudioExceptions.h" file.
 * @param input a Sound object or any image that is a daughter of the Image class.
 * @throws exception if reading the file goes wrong. Have a look at the "AudioExceptions.h" file for all the failure modes.
 */
Fourier::Fourier(Sound& input){ 
    input.readData();
    Fourier::load_signal(input.getData(), false);
}

/** @brief This constructor gets called if you pass in an Eigen::MatrixXd. When calling this you should also specify
whether this data is an image or if it is a sound. This greatly affects how the FFT is computed. If you want to load
another type of Eigen matrix you need to call the default constructor first and then call the load_signal function.
 * @param input an Eigen::MatrixXd matrix
*/
Fourier::Fourier(Eigen::MatrixXd input, bool isImage) : signal(input), signal_rows(input.rows()), signal_cols(input.cols()), image(image) {};
template <typename T>
void Fourier::load_signal(const Eigen::MatrixBase<T>& input, bool isImage){
    Fourier::signal.resize(input.rows(), input.cols());
    Fourier::signal = input.template cast<double>();
    Fourier::signal_rows = Fourier::signal.rows();
    Fourier::signal_cols = Fourier::signal.cols();
    Fourier::image = isImage;
}



/** @brief This is a function that can be used to load an already computed fourier transform to compute it's inverse.
 *  
 *  @tparam T Type of the std::complex values.
 *  @param input An Eigen::Matrix<std::complex<T>, -1, -1> matrix, containing the transform of which you want to compute
 *  the inverse of. 
 *  @param isImage A bool to indicate if the transform is of an image (true) or if it is a sound (false) 
 */
template <typename T>
void Fourier::load_transform(const Eigen::Matrix<std::complex<T>, -1, -1>& input, bool isImage){ 
    Fourier::fft_result.resize(input.rows(), input.cols());
    Fourier::fft_result.setZero();
    Fourier::fft_result = input.template cast<std::complex<double>>();
    Fourier::image = image;
}


/**
 * @brief A helper function to check if a number is a power of 2
 * 
 * @param v an INTEGER to check 
 * @return true if the number is a power of 2
 * @return false if the number if not a power of 2
 */
bool Fourier::__is_power_of_2(int v){
    return v && !(v & (v - 1));
}

//This only really works for < 2^32

/**
 * @brief A helper funciton to compute the closest power of 2 that is larger than the provided number. It accepts and
 * returns an unsigned int. It is up to the user to ensure that the number is less than 2^32 and also that the datatype
 * is an unsigned int otherwise the behaviour is undefined.
 * 
 * @param x an UNSIGNED INT.
 * @return unsigned int
 */
unsigned int Fourier::__next_power_of_2(unsigned int x){
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

/**
 * @brief This accepts an std::tuple<int, int> and zero-pads a signal such that its length this a power of 2. A power of
 * 2 length is needed to compute the FFT. If 0 is provided as padding then the signal will be zero-padded to the next
 * closest power of 2. If some padding is provided, the function will try to pad the signal with that padding. If the
 * signal is still not a power of 2, then the function will add additional padding such that the resulting signal is a
 * power of 2.
 * 
 * @param padding a std::tupe<int, int> where the first numbber is the padding for the rows and the second number is the
 * padding for the columns.
 */
void Fourier::__pad_signal(std::tuple<int, int> padding){
    //Essentially check if the rows or cols are a power of 2. If not, pad it with zeros.
    int padding_rows = std::get<0>(padding);
    int padding_cols = std::get<1>(padding);
    
    
    
    if((padding_rows <= 0 && padding_cols <= 0 ) && (!(__is_power_of_2((int) Fourier::signal.cols())) || !(__is_power_of_2((int) Fourier::signal.rows())))){
        //This is the case where the user hasnt specified padding but the signal length is not a power of 2
        std::cout << "Signal length is not a power of 2 and no or negative padding was provided. Zero padding to the next closest power of 2" << std::endl; 
        
        unsigned int old_col_length = (unsigned int) Fourier::signal.cols();
        unsigned int new_col_length =  __next_power_of_2(old_col_length);
        

        //Note: With conservative resize the new values are uninitalized, not zero!! So we should explicitly set them to zero.
        if(Fourier::signal.rows() == 1){
            Fourier::signal.conservativeResize(Eigen::NoChange_t(), new_col_length);
            Fourier::signal.block(0, old_col_length, 1, (new_col_length-old_col_length)) = Eigen::MatrixXd::Zero(1, (new_col_length-old_col_length));
        }else{
            unsigned int old_row_length = (unsigned int) Fourier::signal.rows();
            unsigned int new_row_length = __next_power_of_2(old_row_length);
            Fourier::signal.conservativeResize(new_row_length, new_col_length);
            Fourier::signal.block(old_row_length, old_col_length, (new_row_length-old_row_length), (new_col_length-old_col_length)) = Eigen::MatrixXd::Zero((new_row_length-old_row_length), (new_col_length-old_col_length));
        }
    }else if((padding_rows != 0 || padding_cols != 0 )  && (!(__is_power_of_2((int) Fourier::signal.cols())) || !(__is_power_of_2((int) Fourier::signal.rows())))){
        //This is the case where the user has provided some padding. But I guess we still need to check if the resulting
        //size will be a power of 2.
        unsigned int old_row_length = (unsigned int) Fourier::signal.rows();
        unsigned int new_row_length = (unsigned int) Fourier::signal.rows();
        
        unsigned int old_col_length = (unsigned int) Fourier::signal.cols();
        unsigned int new_col_length = (unsigned int) Fourier::signal.cols();

        //The user has specified row_padding
        if(padding_rows > 0){

            if(__is_power_of_2(old_row_length + padding_rows)){
                unsigned int new_row_length = old_row_length + padding_rows;
            }else{
                std::cout << "The row padding you have provided is not sufficient. Rounding length+padding to the next closest power of 2" << std::endl;
                unsigned int new_col_length = __next_power_of_2((unsigned int) old_row_length + padding_rows);
            }
        }

        //The user has specified col_padding        
        if(padding_cols > 0){
            if(__is_power_of_2(old_col_length + padding_cols)){
                unsigned int new_col_length = old_col_length + padding_cols;
            }else{
                std::cout << "The column padding you have provided is not sufficient. Rounding length+padding to the next closest power of 2" << std::endl;
                unsigned int new_col_length = __next_power_of_2((unsigned int) old_col_length + padding_cols);
            }
        }


        //conservativeResize leaves the new values uninitialized. They have no gaurentee to be zero.
        //So we have to explicitly set them to zero.
        Fourier::signal.conservativeResize(new_row_length, new_col_length);
        Fourier::signal.block(old_row_length, old_col_length, (new_row_length-old_row_length), (new_col_length-old_col_length)) = Eigen::MatrixXd::Zero((new_row_length-old_row_length), (new_col_length-old_col_length));
    }else{
        std::cout << "The signal length is already a power of 2. Continuing..." << std::endl;
    }
}

/**
 * @brief Shifts the zero-frequency component to the center of the matrix.
 *        Useful for visualization and aligning frequency components.
 * @param matrix The input matrix to shift.
 * @return The shifted matrix.
 */
Eigen::Matrix<std::complex<double>, -1, -1> Fourier::shift(const Eigen::Matrix<std::complex<double>, -1, -1>& matrix){
    Eigen::Matrix<std::complex<double>, -1, -1> shifted = matrix;
    int rows = matrix.rows();
    int cols = matrix.cols();
    int halfRows = rows / 2;
    int halfCols = cols / 2;

    // Swap quadrants to bring low frequencies to the center
    shifted.block(0, 0, halfRows, halfCols).swap(shifted.block(halfRows, halfCols, halfRows, halfCols));
    shifted.block(halfRows, 0, halfRows, halfCols).swap(shifted.block(0, halfCols, halfRows, halfCols));

    return shifted;
}

/**
 * @brief Unshifts the matrix, moving the zero-frequency component back to the corners.
 *        This is the inverse of the `shift` operation.
 * @param matrix The input matrix to unshift.
 * @return The unshifted matrix.
 */
Eigen::Matrix<std::complex<double>, -1, -1> Fourier::unshift(const Eigen::Matrix<std::complex<double>, -1, -1>& matrix){
    Eigen::MatrixXcd unshifted = matrix;
    int rows = matrix.rows();
    int cols = matrix.cols();
    int halfRows = rows / 2;
    int halfCols = cols / 2;

    // Swap quadrants to bring low frequencies back to the corners
    unshifted.block(0, 0, halfRows, halfCols).swap(unshifted.block(halfRows, halfCols, halfRows, halfCols));
    unshifted.block(halfRows, 0, halfRows, halfCols).swap(unshifted.block(0, halfCols, halfRows, halfCols));

    return unshifted;
}




/**
 * @brief This function computes the FFT of the loaded signal. Provided you have a signal loaded you can call this
 * without any arguments to just compute the transform. The function will implicitly pad the signal to a power of 2 if
 * its length is not already a power of 2. Otherwise you can specify the padding you want to apply and it will try to
 * pad the signal with that padding. If the signal is stil not a power of 2, the function will add additional padding
 * such that the length in both the columns and rows is a power of 2.
 * 
 * @param padding an std::tuple<int, int> specifying the padding in the rows and the columns respectively
 * @throw EMPTY_SIGNAL if the signal is empty
 * @throw NOT_POWER_OF_TWO if the signal is not a power of 2
 */
void Fourier::transform(std::tuple<int, int> padding = std::make_tuple(0, 0)){
    if(Fourier::signal.rows() == 0 || Fourier::signal.cols() == 0){
        throw EMPTY_SIGNAL();
    }
    
    
    Fourier::__pad_signal(padding); 
    //At this point out signal length should be a power of 2. But we should assert it regardless
    if(!(__is_power_of_2(Fourier::signal.cols()))){
        throw NOT_POWER_OF_TWO();
    }

    if(!(__is_power_of_2(Fourier::signal.rows()))){
        throw NOT_POWER_OF_TWO();
    }

    //Cast the signal to a complex double
    Fourier::fft_result = Fourier::signal.template cast<std::complex<double>>();

    //We always have to fo the fft over the rows        
    for(int i=0; i < Fourier::fft_result.rows(); i++){
        Eigen::Matrix<std::complex<double>, 1, -1> matrix_type_row_vec = Fourier::fft_result.row(i);
        __fft1d(matrix_type_row_vec);
        Fourier::fft_result.row(i) = matrix_type_row_vec;
    }


    //Only if it is an image do we have to go over the columns
    if(image){
        for(int i=0; i < Fourier::fft_result.cols(); i++){
            Eigen::Matrix<std::complex<double>, 1, -1> matrix_type_col_vec = Fourier::fft_result.col(i).transpose();
            __fft1d(matrix_type_col_vec);
            Fourier::fft_result.col(i) = matrix_type_col_vec.transpose();
        }
    }

    // std::cout << "FFT result" << std::endl;
    // std::cout << fft_result << std::endl;
}

/**
 * @brief A helper function to compute the UNSHIFTED FFT of a 1D row vector inplace.
 * 
 * @param arr A 1D std::complex<double> 1D row vector from Eigen (Eigen::Matrix<std::complex<double>>, 1, -1). 
 */
void Fourier::__fft1d(Eigen::Matrix<std::complex<double>, 1, -1>& arr){
    if(arr.cols() <= 1){
        return;
    }

    Eigen::Matrix<std::complex<double>, 1, -1> odd = Eigen::Matrix<std::complex<double>, 1, -1>::Zero(1, arr.cols()/2);
    Eigen::Matrix<std::complex<double>, 1, -1> even = Eigen::Matrix<std::complex<double>, 1, -1>::Zero(1, arr.cols()/2);

    for(int i = 0; i < arr.cols()/2; i++){
        even(0, i) = arr(0, 2*i);
        odd(0, i)= arr(0, 2*i+1);
    }

    __fft1d(even);
    __fft1d(odd);

    for(int i=0; i < arr.cols()/2; i++){
        std::complex<double> twiddle_factor = exp(std::complex<double>(0, -2*Fourier::PI*i / arr.cols())) * odd(0, i);
        arr(0, i) = even(0, i) + twiddle_factor;
        arr(0, (arr.cols()/2) + i) = even(0, i) - twiddle_factor;
    }
}


/**
 * @brief A function to compute the inverse transform provided that there is a transform to compute the inverse of. This
 * function will throw an EMPTY_FFT_RESULT if there is no transformed signal to compute the inverse of.
 * 
 * @throws EMPTY_FFT_RESULT if there is no transformed signal to compute the inverse of.
 */
void Fourier::inverse_transform(){
    if(Fourier::fft_result.cols() == 0 || Fourier::fft_result.rows() == 0){
        throw EMPTY_FFT_RESULT();
    }

    Fourier::inverse_result = Fourier::fft_result;
    
    for(int i=0; i < Fourier::inverse_result.rows(); i++){
        Eigen::Matrix<std::complex<double>, 1, -1> matrix_type_row_vec = Fourier::inverse_result.row(i);
        __ifft1d(matrix_type_row_vec);
        Fourier::inverse_result.row(i) = matrix_type_row_vec;
    }


    //Only if it is an image do we have to go over the columns
    if(image){
        for(int i=0; i < Fourier::inverse_result.cols(); i++){
            Eigen::Matrix<std::complex<double>, 1, -1> matrix_type_col_vec = Fourier::inverse_result.col(i).transpose();
            __ifft1d(matrix_type_col_vec);
            Fourier::inverse_result.col(i) = matrix_type_col_vec.transpose();
        }
    }
    
    Fourier::inverse_result /= (inverse_result.cols() * inverse_result.rows());
}

/**
 * @brief This is a helper function to compute the inverse fft of a 1D row vector inplace
 * 
 * @param arr THis is a 1D std::complex<double> 1D row vector from Eigen (Eigen::Matrix<std::complex<double>, 1, -1>)
 */
void Fourier::__ifft1d(Eigen::Matrix<std::complex<double>, 1, -1>& arr){
    if(arr.cols() <= 1){
        return;
    }
    
    
    Eigen::Matrix<std::complex<double>, 1, -1> odd = Eigen::Matrix<std::complex<double>, 1, -1>::Zero(1, arr.cols()/2);
    Eigen::Matrix<std::complex<double>, 1, -1> even = Eigen::Matrix<std::complex<double>, 1, -1>::Zero(1, arr.cols()/2);

    for(int i = 0; i < arr.cols()/2; i++){
        even(0, i) = arr(0, 2*i);
        odd(0, i)= arr(0, 2*i+1);
    }

    __ifft1d(even);
    __ifft1d(odd);

    for(int i=0; i < arr.cols()/2; i++){
        std::complex<double> twiddle_factor = exp(std::complex<double>(0, 2*Fourier::PI*i / arr.cols())) * odd(0, i);
        arr(0, i) = even(0, i) + twiddle_factor;
        arr(0, (arr.cols()/2) + i) = even(0, i) - twiddle_factor;
    }
}

/**
 * @brief A getter function to get the FFT result. The user should specify what the return type of the complex matrix
 * should be. For example if you wanted the FFT result as Eigen::Matrix<std::complex<float>, -1, -1> then you would call
 * fourier.get_fft_result<float>(). This function will throw an EMPTY_FFT_RESULT if the FFT result is empty.
 * 
 * @tparam T the type of the returned std::complex values
 * @return Eigen::Matrix<std::complex<T>, -1, -1>
 * @throws EMPTY_FFT_RESULT if the FFT result is empty 
 */
template <typename T>
Eigen::Matrix<std::complex<T>, -1, -1> Fourier::get_fft_result(){
    if(Fourier::fft_result.cols() == 0 || Fourier::fft_result.rows() == 0){
        throw EMPTY_FFT_RESULT();
    }

    return Fourier::fft_result.template cast<std::complex<T>>();
}

/**
 * @brief A getter function to get the padded version of the inverse FFT result. The user should specify what the return
 * type of the matrix should be. For example if you wanted the FFT result as Eigen::Matrix<float, -1, -1> then you would call fourier.get_inverse_result_padded<float>()
 * 
 * @tparam T the type of the returned matrix.
 * @return Eigen::Matrix<T, -1, -1> 
 * @throws EMPTY_INVERSE_RESULT if the inverse result is empty
 */
template <typename T>
Eigen::Matrix<T, -1, -1> Fourier::get_inverse_result_padded(){
    if(Fourier::inverse_result.cols() == 0 || Fourier::inverse_result.rows() == 0){
        throw EMPTY_INVERSE_RESULT();
    }


    return Fourier::inverse_result.real().template cast<T>();
}

/**
 * @brief A getter function to return the truncated version of the inverse FFT result. The user should specify what the return
 * type of the matrix should be. For example if you wanted the FFT result as Eigen::Matrix<float, -1, -1> then you would
 * call fourier.get_inverse_result<float>(). The inverse result is truncated to the size of the original signal.
 * 
 * @tparam T the type of the returned matrix.
 * @return Eigen::Matrix<T, -1, -1> 
 * @throws EMPTY_INVERSE_RESULT if the inverse result is empty
 */
template <typename T>
Eigen::Matrix<T, -1, -1> Fourier::get_inverse_result(){
    if(Fourier::inverse_result.cols() == 0 || Fourier::inverse_result.rows() == 0){
        throw EMPTY_INVERSE_RESULT();
    } 
    return Fourier::inverse_result.block(0, 0, Fourier::signal_rows, Fourier::signal_cols).real().template cast<T>();
}


/**
 * @brief A getter function to return the loaded signal. Will throw a EMPTY_SIGNAL if the signal is empty
 * 
 * @tparam T 
 * @return Eigen::Matrix<T, -1, -1> 
 * @throws EMPTY_SIGNAL if the signal is empty
 */
template <typename T>
Eigen::Matrix<T, -1, -1> Fourier::get_signal(){
    if(Fourier::signal.cols() == 0 || Fourier::signal.rows() == 0){
        throw EMPTY_SIGNAL();
    }

    return Fourier::signal.template cast<T>();
}


/**
 * @brief A function to print the loaded signal. Will throw a EMPTY_SIGNAL if the signal is empty
 * @throws EMPTY_SIGNAL if the signal is empty
 * 
 */
void Fourier::print_signal(){
    if(Fourier::signal.cols() == 0 || Fourier::signal.rows() == 0){
        throw EMPTY_SIGNAL();
    }
    
    std::cout << signal << std::endl;
}

#endif