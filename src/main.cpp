#include "Image.h"
#include "Sound.h"
#include "WAVInput.h"
#include "PNGInput.h"
#include "BMPInput.h"
#include "MP3Output.h"
#include "PNGOutput.h"
#include "CSVOutput.h"
#include <iostream>
#include <string>
#include <exception>
#include "AudioExceptions.h"
#include "Fourier.h"
#include "ImageExceptions.h"
#include "Visualizer.h"
#include "SignalProcessor.h"
#include <regex>
#include <string>

enum SoundType{
    WAV
};



bool validateExtension(const std::string& filename, const std::string& expected_extension);
void menu(char& filetype_str);
void menu2(char& op_str);
void menu3(char& op_str);
void menu_restart(char& op_str);
void menu_fourier(char& op_str);

void get_filepath(std::string& filepath_str);

int main() {
    char filetype;
    enum SoundType sound_type;
    std::unique_ptr<Fourier> fft_(new Fourier());
    std::string filepath;
    Eigen::MatrixXd data;
    bool image = false;
    std::unique_ptr<Sound> sound_input;
    
    
    std::cout << "Welcome! What type of file would you like to open?" << std::endl;

    menu_op_1:
    menu(filetype);

    switch_case_1:
    switch(filetype){
        case 'a':{
            get_filepath(filepath);
            if (filepath == "z") goto menu_op_1;

            while(true){
                try{
                    std::unique_ptr<PNGInput> input(new PNGInput(filepath));    
                    input->readData();
                    data = input->getData();
                    break;
                }catch(const INVALID_PNG_READ& e){
                    std::cout << e.what() << std::endl;
                    get_filepath(filepath);
                    if (filepath == "z") goto menu_op_1;
                }catch(const std::exception& e){
                    std::cout << e.what() << std::endl;
                    get_filepath(filepath);
                    if (filepath == "z") goto menu_op_1;
                }
            }
            image=true;
            break;
        }
        case 'b':{
            get_filepath(filepath);
            if (filepath == "z") goto menu_op_1;
            
            while(true){
                try{
                    std::unique_ptr<BMPInput> input(new BMPInput(filepath));    
                    input->readData();
                    data = input->getData();
                    image = true;
                    break;
                }catch(const std::exception& e){
                    std::cout << e.what() << std::endl;
                    get_filepath(filepath);
                    if (filepath == "z") goto menu_op_1;
                }catch(INVALID_BMP_READ& e){
                    std::cout << e.what() << std::endl;
                    get_filepath(filepath);
                    if (filepath == "z") goto menu_op_1;
                }
            }
            break;
        }
        case 'c':{
            get_filepath(filepath);

            while(true){
                try{
                    sound_input = std::make_unique<WAVInput>(filepath);
                    sound_input->readData();
                    data = sound_input->getData();
                    image = false;
                    sound_type = WAV;
                    break;
                }catch(const INVALID_WAV_FILE_OPEN& e){
                    std::cout << e.what() << std::endl;
                    get_filepath(filepath);
                    if (filepath == "z") goto menu_op_1;
                }
            }

            break;
        }
        case 'q':{
            std::cout << "Goodbye!" << std::endl;
            return 0;
        }
        default: {
            std::cout << "Invalid input. Please try again." << std::endl;
            menu(filetype);
            goto switch_case_1;
            break;
        }
    }


    std::cout << "Computing the FFT of the signal for future use..." << std::endl;
    fft_->load_signal(data, image);
    fft_->transform();
    std::cout << "FFT computed. Continuing..." << std::endl;
    
    char op2;
    menu_op_2:
    menu2(op2);
    
    char graph_type;
    switch_case_2:
    switch(op2){
        case 'a':{
            //This is the case where the user has chosen to graph the signal
            new_plot:
            menu3(graph_type);
            Visualizer graph = Visualizer();
            
            switch_case_graph:
            switch(graph_type){
                case 'a':{
                    //Histogram Option
                    int num_bins = 10;
                    std::string save_path = "";
                    std::string save_bool = "y";
                    std::cout << "How many bins do you want to have (default = 10) " << std::endl;
                    std::cin >> num_bins;
                    std::cout << "Do you want to save this plot? (y/n)" << std::endl;
                    std::cin >> save_bool;

                    if(save_bool == "y"){
                        std::cout << "Please enter the save path: Eg: graph1.png" << std::endl;
                        std::cin >> save_path;
                    }

                    graph.plot(data, "histogram", num_bins, save_path);

                    break;
                }
                case 'b':{
                    //Line Plot Option
                    if(image){
                        std::cout << "You have chosen the line plot option. But the data loaded was an image. Choose a different plot option" << std::endl;
                        goto new_plot;
                    }

                    std::string save_path = "";
                    std::string save_bool = "y";
                    std::cout << "Do you want to save this plot? (y/n)" << std::endl;
                    std::cin >> save_bool;

                    if(save_bool == "y"){
                        std::cout << "Please enter the save path: Eg: graph1.png" << std::endl;
                        std::cin >> save_path;
                    }

                    graph.plot(data, "line", 0, save_path);
                    break;
                }
                case 'c':{
                    //Summary Information
                    graph.plot(data, "summary");
                    break;
                }
                case 'z':{
                    goto menu_op_2;
                }
                default: {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    menu3(graph_type);
                    goto switch_case_graph;
                    break;
                }
            }
            
            break;
        }
        case 'b':{
            //This is the case where we need to filter the signal
            char op_filter;
            std::cout << "Do you want to apply: " << std::endl;
            std::cout << "\t a) Low pass filter" << std::endl;
            std::cout << "\t b) High pass filter" << std::endl;
            //std::cout << "\t c) Band pass filter" << std::endl;
            std::cout << "\t z) Go to previous menu" << std::endl;
            std::cin >> op_filter;

            switch_case_filter:
            switch(op_filter) {
                case 'a': {
                    Eigen::MatrixXcd shifted = fft_->get_fft_result<double>();
                         
                    SignalProcessor sp = SignalProcessor(shifted, !image);
                    std::cout << "What cutoff frequency do you want to use? Remember this is a low-pass filter [0-1]" << std::endl;
                    double cutoff;
                    std::cin >> cutoff;
                    Eigen::MatrixXcd filtered = sp.applyLowPassFilter(cutoff);
                    
                    if(image){
                        fft_->load_transform<double>(filtered, true);
                    }else{
                        fft_->load_transform<double>(filtered, false);
                    }
                    
                    
                    fft_->inverse_transform();

                    char save_img;
                    save_low_pass_option:
                    std::cout << "Do you want to save the filted data? (y/n)" << std::endl;
                    std::cin >> save_img;
                    if(save_img == 'y') {
                        char save_format;
                        if(!image){
                            save_low_pass_format_option_sound:
                            std::cout << "Choose a format to save: (m) MP3 or (c) CSV" << std::endl;
                            std::cin >> save_format;

                            if(save_format == 'p') {
                                std::string mp3_path = "";
                                do {
                                    std::cout << "Please enter the save path for MP3 (e.g., output.mp3): " << std::endl;
                                    std::cin >> mp3_path;
                                    if (!validateExtension(mp3_path, "mp3")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .png." << std::endl;
                                    }
                                } while (!validateExtension(mp3_path, "mp3"));

                                MP3Output output = MP3Output(mp3_path, *sound_input);
                                
                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();
                                
                                output.save(filtered_data);
                            } else if(save_format == 'c') {
                                std::string csv_path = "";
                                do {
                                    std::cout << "Please enter the save path for CSV (e.g., output.csv): " << std::endl;
                                    std::cin >> csv_path;
                                    if (!validateExtension(csv_path, "csv")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .csv." << std::endl;
                                    }
                                } while (!validateExtension(csv_path, "csv"));

                                CSVOutput output = CSVOutput(csv_path);
                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();

                                output.save(filtered_data);
                            } else {
                                std::cout << "Invalid input. Please try again." << std::endl;
                                goto save_low_pass_format_option_sound;
                            }
                        }else{
                            save_low_pass_format_option_image:
                            std::cout << "Choose a format to save: (p) PNG or (c) CSV" << std::endl;
                            std::cin >> save_format;

                            if(save_format == 'p') {
                                std::string img_path = "";
                                do {
                                    std::cout << "Please enter the save path for PNG (e.g., output.png): " << std::endl;
                                    std::cin >> img_path;
                                    if (!validateExtension(img_path, "png")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .png." << std::endl;
                                    }
                                } while (!validateExtension(img_path, "png"));

                                PNGOutput output = PNGOutput(img_path);


                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();


                                output.save(filtered_data);
                            } else if(save_format == 'c') {
                                std::string csv_path = "";
                                do {
                                    std::cout << "Please enter the save path for CSV (e.g., output.csv): " << std::endl;
                                    std::cin >> csv_path;
                                    if (!validateExtension(csv_path, "csv")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .csv." << std::endl;
                                    }
                                } while (!validateExtension(csv_path, "csv"));

                                CSVOutput output = CSVOutput(csv_path);
                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();

                                output.save(filtered_data);
                            } else {
                                std::cout << "Invalid input. Please try again." << std::endl;
                                goto save_low_pass_format_option_image;
                            }
                        }
                    } else if(save_img != 'n') {
                        std::cout << "Invalid input. Please try again." << std::endl;
                        goto save_low_pass_option;
                    }

                    // Low pass filter
                    break;
                }
                case 'b': {
                    Eigen::MatrixXcd shifted = fft_->get_fft_result<double>();

                    SignalProcessor sp = SignalProcessor(shifted, !image);
                    std::cout << "What cutoff frequency do you want to use? Remember this is a high-pass filter [0-1]" << std::endl;
                    double cutoff;
                    std::cin >> cutoff;
                    Eigen::MatrixXcd filtered = sp.applyHighPassFilter(cutoff);
                    if(image){
                        fft_->load_transform<double>(filtered, true);
                    }else{
                        fft_->load_transform<double>(filtered, false);
                    }
                    fft_->inverse_transform();

                    char save_audio;
                    save_high_pass_option:
                    std::cout << "Do you want to save the filtered output? (y/n)" << std::endl;
                    std::cin >> save_audio;
                    if(save_audio == 'y') {
                        char save_format;

                        if(!image){
                            //Here the filtered data is actually a sound
                            save_high_pass_format_option:
                            std::cout << "Choose a format to save: (m) MP3 or (c) CSV" << std::endl;
                            std::cin >> save_format;

                            if(save_format == 'm') {
                                std::string mp3_path = "";
                                do {
                                    std::cout << "Please enter the save path for MP3 (e.g., output.mp3): " << std::endl;
                                    std::cin >> mp3_path;
                                    if (!validateExtension(mp3_path, "mp3")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .mp3." << std::endl;
                                    }
                                } while (!validateExtension(mp3_path, "mp3"));
                                MP3Output output = MP3Output(mp3_path, *sound_input);

                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();

                                std::cout << "Output rows: " << filtered_data.rows() << " Output cols" << filtered_data.cols() << std::endl;
                                std::cout << "Data Rows: " << data.rows() << " Data Cols: " << data.cols() << std::endl;


                                output.save(filtered_data);
                            } else if(save_format == 'c') {
                                std::string csv_path = "";
                                do {
                                    std::cout << "Please enter the save path for CSV (e.g., output.csv): " << std::endl;
                                    std::cin >> csv_path;
                                    if (!validateExtension(csv_path, "csv")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .csv." << std::endl;
                                    }
                                } while (!validateExtension(csv_path, "csv"));

                                CSVOutput output = CSVOutput(csv_path);
                                
                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();
                                output.save(filtered_data);
                            } else {
                                std::cout << "Invalid input. Please try again." << std::endl;
                                goto save_high_pass_format_option;
                            }
                        }else{
                            //Here the filtered data is actually an image
                            save_high_pass_format_option_image:
                            std::cout << "Choose a format to save: (p) PNG or (c) CSV" << std::endl;
                            std::cin >> save_format;
                            
                            if(save_format == 'p') {
                                std::string png_path = "";
                                do {
                                    std::cout << "Please enter the save path for PNG (e.g., output.png): " << std::endl;
                                    std::cin >> png_path;
                                    if (!validateExtension(png_path, "png")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .png." << std::endl;
                                    }
                                } while (!validateExtension(png_path, "png"));
                                PNGOutput output = PNGOutput(png_path);

                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();

                                output.save(filtered_data);
                            } else if(save_format == 'c') {
                                std::string csv_path = "";
                                do {
                                    std::cout << "Please enter the save path for CSV (e.g., output.csv): " << std::endl;
                                    std::cin >> csv_path;
                                    if (!validateExtension(csv_path, "csv")) {
                                        std::cout << "Invalid file extension. Please ensure the filename ends with .csv." << std::endl;
                                    }
                                } while (!validateExtension(csv_path, "csv"));

                                CSVOutput output = CSVOutput(csv_path);
                                
                                Eigen::MatrixXd filtered_data = fft_->get_inverse_result<double>();
                                output.save(filtered_data);
                            } else {
                                std::cout << "Invalid input. Please try again." << std::endl;
                                goto save_high_pass_format_option_image;
                            }
                        }
                    } else if(save_audio != 'n') {
                        std::cout << "Invalid input. Please try again." << std::endl;
                        goto save_high_pass_option;
                    }
                    break;
                }
                case 'z': {
                    goto menu_op_2;
                }
                default: {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    goto switch_case_2;
                    break;
                }
            }


            break;
        }
        case 'q':{
            std::cout << "Goodbye!" << std::endl;
            return 0;
        }
        case 'z':{
            goto menu_op_1;
        }
        default: {
            std::cout << "Invalid input. Please try again." << std::endl;
            menu2(op2);
            goto switch_case_2;
            break;
        }
    }
    
    
    //Here we need to have the option to go back to a certain menu.
    char op4;
    menu_restart(op4);

    switch_case_3:
    switch(op4){
    case 'a':{
        goto menu_op_1;
        break;
    }
    case 'b':{
        goto menu_op_2;
    }
    case 'q':{
        std::cout << "Goodbye!" << std::endl;
        return 0;
    } 
    default:
        std::cout << "Invalid input. Please try again." << std::endl;
        menu_restart(op4);
        goto switch_case_3;
        break;
    }
    return 0;

}

void get_filepath(std::string& filepath_str){
    std::cout << "Please enter the filepath or z if you want to go back:" << std::endl;
    std::cin >> filepath_str;
}

void menu(char& filetype_str){
    std::cout << "The options we support are:" << std::endl;
    std::cout << "Images:" << std::endl;
    std::cout << "\t a) PNG" << std::endl; 
    std::cout << "\t b) BMP" << std::endl; 
    std::cout << "Sounds:" << std::endl;
    std::cout << "\t c) WAV" << std::endl;
    std::cout << "\t q) Quit" <<std::endl;
    std::cout << "Type in the filetype you want to load (eg if you want to load a PNG type a)" << std::endl;
    std::cin >> filetype_str;
}
void menu2(char& op_str){
    std::cout << "Sucessfully read file. What would you like to do?" << std::endl;
    std::cout << "\t a) Graph the signal" << std::endl;
    std::cout << "\t b) Filter the signal" << std::endl;
    std::cout << "\t q) Quit" << std::endl;
    std::cout << "\t z) Go to previous menu" << std::endl;
    std::cin >> op_str;
}

void menu3(char& op_str){
    std::cout << "You have chosen the graph option. Which graph do you want:" << std::endl;
    std::cout << "\t a) Histogram (Image/Audio data)" << std::endl;
    std::cout << "\t b) Line plot (Audio data)" << std::endl;
    std::cout << "\t c) Summary (Images/Audio data)" << std::endl;
    std::cout << "\t z) Go to previous menu" << std::endl;

    std::cin >> op_str;
}

void menu_restart(char& op_str){
    std::cout << "You have reached the end of the program pipeline. You have the choice of going to a certain menu or quitting" << std::endl;
    std::cout << "The options are:" << std::endl;
    std::cout << "\t a) Load a new signal" << std::endl;
    std::cout << "\t b) Graph/Filter signal" << std::endl;
    std::cout << "\t q) Quit" << std::endl;
    std::cin >> op_str;

}

void menu_fourier(char& op_str){
    //Maybe do some stuff with the FFT here.
}

// Function to validate file extension
bool validateExtension(const std::string& filename, const std::string& expected_extension) {
    std::regex extension_pattern(".*\\." + expected_extension + "$");
    return std::regex_match(filename, extension_pattern);
}
