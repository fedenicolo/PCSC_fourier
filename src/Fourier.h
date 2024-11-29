//A H file for the Fourier Transform class.

#ifndef FOURIER_H
#define FOURNER_H

#include "Eigen/Dense"
#include <complex>
#include <math.h>
#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class Fourier{
    T PI = 3.141592653589793238460;
    
    public:
        Fourier(std::vector<T>& input_signal);
        bool load_signal(std::vector<T>& input_signal);
        void pad_signal(int padding);
        bool transform(int padding);
        bool inverse_transform();
        std::vector<std::complex<T>> get_fft_result();
        std::vector<T> get_signal();
        void print_signal();    
    private:
        Eigen::Matrix<T, 1, -1> signal;
        Eigen::Matrix<std::complex<T>, 1, -1> fft_result;
        Eigen::Matrix<std::complex<T>, 1, -1> inverse_result;
        bool __is_power_of_2(int v);
        unsigned int __next_power_of_2(unsigned int x);
        void __fft(Eigen::Matrix<std::complex<T>, 1, -1>& arr);
        void __ifft(Eigen::Matrix<std::complex<T>, 1, -1>& arr);
};

template <typename T>
Fourier<T>::Fourier(std::vector<T>& input_signal) 
    : signal(Eigen::Map<Eigen::Matrix<T, 1, -1>>(input_signal.data(), 1, input_signal.size())) {}

template <typename T>
bool Fourier<T>::load_signal(std::vector<T>& input_signal){
    //This shouldn't fail? I see no reason why it should fail...
    //Might then change this to void if we cannot find a reason why this func should fail
    if(Fourier<T>::signal.cols() != input_signal.size()){
        //NoChange_t means that dimension stays unchanged
        Fourier<T>::signal.resize(Eigen::NoChange_t(), input_signal.size());
    }

    Fourier<T>::signal = Eigen::Map<Eigen::Matrix<T, 1, input_signal.size()>> (input_signal.data(), 1, input_signal.size());
    return true;
}
template <typename T>
bool Fourier<T>::__is_power_of_2(int v){
    return v && !(v & (v - 1));
}

template <typename T>
//This only really works for < 2^32
unsigned int Fourier<T>::__next_power_of_2(unsigned int x){
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

template <typename T>
void Fourier<T>::pad_signal(int padding){
    if(padding <= 0 && !(__is_power_of_2((int) Fourier<T>::signal.cols()))){
        //This is the case where the user hasnt specified padding but the signal length is not a power of 2
        std::cout << "Signal length is not a power of 2 and no or negative padding was provided. Zero padding to the next closest power of 2" << std::endl; 
        unsigned int old_length = (unsigned int) Fourier<T>::signal.cols();
        unsigned int new_length =  __next_power_of_2(old_length);

        //conservativeResize leaves the new values uninitialized. They have no gaurentee to be zero.
        //So we have to explicitly set them to zero.
        Fourier<T>::signal.conservativeResize(Eigen::NoChange_t(), new_length);
        Fourier<T>::signal.block(1, old_length, 1, (new_length-old_length)) = Eigen::Matrix<T, -1, -1>::Zero(1, (new_length-old_length));
    }else if(padding != 0 && !(__is_power_of_2((int) Fourier<T>::signal.cols()))){
        //This is the case where the user has provided some padding. But I guess we still need to check if the resulting
        //size will be a power of 2.
        unsigned int old_length = (unsigned int) Fourier<T>::signal.cols();
        unsigned int new_length = 0;
        if(__is_power_of_2(old_length + padding)){
            unsigned int new_length = old_length + padding;
        }else{
            std::cout << "The padding you have provided is not sufficient. Rounding length+padding to the next closest power of 2" << std::endl;
            unsigned int new_length = __next_power_of_2((unsigned int) old_length + padding);
        }

        //conservativeResize leaves the new values uninitialized. They have no gaurentee to be zero.
        //So we have to explicitly set them to zero.
        Fourier<T>::signal.conservativeResize(Eigen::NoChange_t(), new_length);
        Fourier<T>::signal.block(1, old_length, 1, (new_length-old_length)) = Eigen::Matrix<T, -1, -1>::Zero(1, (new_length-old_length));
    }else{
        std::cout << "The signal length is already a power of 2. Continuing..." << std::endl;
    }
}


template <typename T>
bool Fourier<T>::transform(int padding){
    //TODO: Explicitly check if the signal is zero and then break
    Fourier<T>::pad_signal(padding); 
    //At this point out signal length should be a power of 2. But we should assert it regardless
    if(!(__is_power_of_2(Fourier<T>::signal.cols()))){
        throw std::runtime_error("The signal length is not a power of 2");
        return false;
    }

    Fourier<T>::fft_result = Fourier<T>::signal.template cast<std::complex<T>>();
    __fft(fft_result);
    std::cout << "FFT result" << std::endl;
    std::cout << fft_result << std::endl;
    return true;
}

template <typename T>
void Fourier<T>::__fft(Eigen::Matrix<std::complex<T>, 1, -1>& arr){
    if(arr.cols() <= 1){
        return;
    }

    Eigen::Matrix<std::complex<T>, 1, -1> odd = Eigen::Matrix<std::complex<T>, 1, -1>::Zero(1, arr.cols()/2);
    Eigen::Matrix<std::complex<T>, 1, -1> even = Eigen::Matrix<std::complex<T>, 1, -1>::Zero(1, arr.cols()/2);

    for(int i = 0; i < arr.cols()/2; i++){
        even(0, i) = arr(0, 2*i);
        odd(0, i)= arr(0, 2*i+1);
    }

    __fft(even);
    __fft(odd);

    for(int i=0; i < arr.cols()/2; i++){
        std::complex<T> twiddle_factor = exp(std::complex<T>(0, -2*Fourier<T>::PI*i / arr.cols())) * odd(0, i);
        arr(0, i) = even(0, i) + twiddle_factor;
        arr(0, (arr.cols()/2) + i) = even(0, i) - twiddle_factor;
    }
}


template <typename T>
bool Fourier<T>::inverse_transform(){
    if(Fourier<T>::fft_result.cols() == 0){
        throw std::runtime_error("The FFT result is empty. Please run the 'transform' function before calling inverse_transform ");
        return false;
    }

    Fourier<T>::inverse_result = Fourier<T>::fft_result;
    __ifft(inverse_result);
    Fourier<T>::inverse_result /= inverse_result.cols();
    std::cout << "Inverse Transform Result: " << std::endl << inverse_result.real() << std::endl;
    return true;
}

template <typename T>
void Fourier<T>::__ifft(Eigen::Matrix<std::complex<T>, 1, -1>& arr){
    if(arr.cols() <= 1){
        return;
    }
    
    
    Eigen::Matrix<std::complex<T>, 1, -1> odd = Eigen::Matrix<std::complex<T>, 1, -1>::Zero(1, arr.cols()/2);
    Eigen::Matrix<std::complex<T>, 1, -1> even = Eigen::Matrix<std::complex<T>, 1, -1>::Zero(1, arr.cols()/2);

    for(int i = 0; i < arr.cols()/2; i++){
        even(0, i) = arr(0, 2*i);
        odd(0, i)= arr(0, 2*i+1);
    }

    __ifft(even);
    __ifft(odd);

    for(int i=0; i < arr.cols()/2; i++){
        std::complex<T> twiddle_factor = exp(std::complex<T>(0, 2*Fourier<T>::PI*i / arr.cols())) * odd(0, i);
        arr(0, i) = even(0, i) + twiddle_factor;
        arr(0, (arr.cols()/2) + i) = even(0, i) - twiddle_factor;
    }
}


template <typename T>
std::vector<std::complex<T>> Fourier<T>::get_fft_result(){
    std::vector<std::complex<T>> vec_result;
    //TODO: Wrap this in an if-block so this code can also be used for 2d matrices
    for(int i=0; i < Fourier<T>::fft_result.cols(); i++){
        vec_result.push_back(fft_result(0, i));
    }

    return vec_result;
}

template <typename T>
std::vector<T> Fourier<T>::get_signal(){
    std::vector<T> vec_result;
    //TODO: Wrap this in an if-block so this code can also be used for 2d matrices
    for(int i=0; i < Fourier<T>::signal.cols(); i++){
        vec_result.push_back(signal(0, i));
    }
    return vec_result;
}


template <typename T>
void Fourier<T>::print_signal(){
    std::cout << signal << std::endl;
}

#endif