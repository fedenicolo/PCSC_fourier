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


class Fourier{
    double PI = 3.141592653589793238460;
    
    public:
        Fourier();
        Fourier(Image& input);
        Fourier(Sound& input);
        Fourier(Eigen::MatrixXd input, bool image);
        
        template <typename T>
        void load_signal(const Eigen::MatrixBase<T>& input, bool image);
        void transform(std::tuple<int, int> padding);

        template <typename T>
        void load_transform(const Eigen::MatrixBase<std::complex<T>>& input, bool image);
        void inverse_transform();
        
        template <typename T>
        Eigen::Matrix<std::complex<T>, -1, -1> get_fft_result();

        template <typename T>
        Eigen::Matrix<T, -1, -1> get_inverse_result_padded();

        template <typename T>
        Eigen::Matrix<T, -1, -1> get_inverse_result();
        
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

Fourier::Fourier(){};

Fourier::Fourier(Image& input){
    Fourier::load_signal(input.getData(), true);
}

Fourier::Fourier(Sound& input){ 
    Fourier::load_signal(input.getData(), false);
}

Fourier::Fourier(Eigen::MatrixXd input, bool image) : signal(input), signal_rows(input.rows()), signal_cols(input.cols()), image(image) {};
template <typename T>
void Fourier::load_signal(const Eigen::MatrixBase<T>& input, bool image){
    Fourier::signal.resize(input.rows(), input.cols());
    Fourier::signal = input.template cast<double>();
    Fourier::signal_rows = Fourier::signal.rows();
    Fourier::signal_cols = Fourier::signal.cols();
    Fourier::image = image;
}

template <typename T>
void Fourier::load_transform(const Eigen::MatrixBase<std::complex<T>>& input, bool image){
    Fourier::fft_result.resize(input.rows(), input.cols());
    Fourier::fft_result = input.template cast<std::complex<double>>();
    Fourier::signal_rows = Fourier::fft_result.rows();
    Fourier::signal_cols = Fourier::fft_result.cols();
    Fourier::image = image;
}


bool Fourier::__is_power_of_2(int v){
    return v && !(v & (v - 1));
}

//This only really works for < 2^32
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
            Fourier::signal.block(1, old_col_length, 1, (new_col_length-old_col_length)) = Eigen::MatrixXd::Zero(1, (new_col_length-old_col_length));
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



void Fourier::transform(std::tuple<int, int> padding = std::make_tuple(0, 0)){
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

void Fourier::inverse_transform(){
    if(Fourier::fft_result.cols() == 0){
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
    //std::cout << "Inverse Transform Result: " << std::endl << inverse_result.real() << std::endl;
}


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


template <typename T>
Eigen::Matrix<std::complex<T>, -1, -1> Fourier::get_fft_result(){
    return Fourier::fft_result.template cast<std::complex<T>>();
}

template <typename T>
Eigen::Matrix<T, -1, -1> Fourier::get_inverse_result_padded(){
    return Fourier::inverse_result.real().template cast<T>();
}

template <typename T>
Eigen::Matrix<T, -1, -1> Fourier::get_inverse_result(){
    return Fourier::inverse_result.block(0, 0, Fourier::signal_rows, Fourier::signal_cols).real().template cast<T>();
}

template <typename T>
Eigen::Matrix<T, -1, -1> Fourier::get_signal(){
    return Fourier::signal.template cast<T>();
}

void Fourier::print_signal(){
    std::cout << signal << std::endl;
}

#endif