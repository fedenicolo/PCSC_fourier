//A H file for the Fourier Transform class.

#ifndef FOURIER_H
#define FOURNER_H

#include "Eigen/Dense"
#include <complex>
#include <vector>
#include <iostream>
#include <stdexcept>

template <typename T>
class Fourier{
    T PI = 3.141592653589793238460;
    
    public:
        Fourier(std::vector<T>& input_signal);
        bool load_signal(std::vector<T>& input_signal);
        bool transform();
        void print_signal();    
    private:
        Eigen::Matrix<T, 1, -1> signal;
        Eigen::Matrix<std::complex<T>, 1, -1> fft_result;
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
        Fourier<T>::signal.resize(Eigen::NoChange_t, input_signal.size())
    }

    Fourier<T>::signal = Eigne::Map<Eigen::Matrix<T, 1, input_signal.size()>> (input_signal.data(), 1, input_signal.size());
    return true
}

template <typename T>
bool transform(){
    if
}

template <typename T>
void Fourier<T>::print_signal(){
    std::cout << signal << std::endl;
}

#endif