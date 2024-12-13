# Project name : PCSC_fourier
The goal of the PCSC_fourier project is to provide a set of easy-to-use tools for basic audio and image processing, leveraging the power of Fourier transforms and customizable filters, using object-oriented C++.

## Instructions to Build the Project

1. Navigate to the root directory of the project. Update the submodules by running:
   ```bash
   git submodule update --init --recursive
   ```
2. Make the clean.sh file executable by running
   ```bash
   chmod +x ./clean.sh
   ````
3. If you have previously built something in this directory, clear out any generated files by running:
   ```bash
   make clean
   ./clean.sh
   ```

4. Generate the Makefile for building by running:
   ```bash
   cmake -DTESTS=ON -DDOCUMENTATION=ON .
   ```

5. To build the tests, run:
   ```bash
   make tests
   ```
   You may also set the `-j<NUM_THREADS>` flag to use multiple threads for faster compilation. Note that compiling from scratch may take a while.

6. Run the tests with the command:
   ```bash
   make -i run_tests
   ```

7. To build the main program, run:
   ```bash
   make main
   ```

8. To generate the Doxygen documentation, run:
   ```bash
   mkdir docs
   make doc_doxygen
   ```
   The documentation will be created in the `docs` directory.

9. The main program and all test files will be located in the `build` directory.

## Typical program execution and usage

1. Run the program with the command:
   ```bash
   make -i run_tests ///THIS NEEDS TO BE EDITED
   ```  

## Features

- **Input Handling**:
  - `BMPInput`, `PNGInput`, `WAVInput`: Classes specialized in reading BMP images, PNG images, and WAV audio files respectively.
  These classes provide easy-to-use methods to read audio and image data from different types of file. They also provide methods
  to extract basic metadata about the data signals such as the sampling rate for audio data and the width and height for images.

- **Output Generation**:
  - `MP3Output`, `PNGOutput`, `CSVOutput`: Classes used to save processed audio or image data in MP3, PNG, or CSV formats.
  These classes provide intuitive methods to encode and save audio and image data to different types of files.

- **Processing**:
  - `Fourier`: This class handles features related to the computation of the Fast Fourier Transform for both audio signals and images.
  It is also used to compute the inverse FFT. Most of the edge cases are treated internally (e. g. padding of the signal before computing its FFT).
  The Fourier class has multiple constructors and provides several templated methods allowing great flexibility in its usage.

  - `SignalProcessor`: This class provides methods to filter FFT data and therefore process audio and image data. The filters that are available to the user
  are easy-to-use low-pass and high-pass filters. The filtering methods require a cutoff value (between 0 and 1 both for image and sound)(normalized frequencies; 2*fcutoff/fsample) in order to work. The class automatically handles the creation of frequency grids tailored to the input type, whether it is a 2D image or a 1D sound signal.

- **Visualization and Analysis**:
  - `Histogram`: This class provides methods for computing histograms. It supports computing histograms of real-valued data, such as pixel intensity probabilities, and includes specialized functionality for handling complex-valued data. When an FFT matrix is provided, the class computes the power spectrum and generates a histogram of its values.

  - `Visualizer`: This class provides easy-to-use methods for visualizing data using different plot types with Gnuplot integration.

- **Error Handling**:
  - The project uses custom exception classes extensively to ensure robust error reporting and handling. Most exceptions are thrown in situations
  that would lead to unavoidable critical failure of the program, therefore most exceptions are handeled by simply terminating the program.

- **Third-party Libraries**:
  - `LAME`: For MP3 encoding.
  - `stb_image`: For reading and writing PNG images.
  - `GNUplot`: For generating plots.
  - `Eigen`: For matrix and vector computations.
  - `Google Test`: For implementing and managing tests.

## Tests using Google Tests
**Input Handling**: All google tests for the input classes pass. The true data of the file, extracted using trusted libraries on python,
is compared to the audio and image data extracted using the methods of the project.

**Output Generation**: We could not think of a relevant quantitative test using Google Tests for output classes since they only take an existing data
matrix and save it to the file. Therefore, the methods were tested by reading a file and saving it to a new file making sure the two files are the
same at the end. Such a test is implemented using Google Tests for the MP3Output class.

**Processing**: Since the Fourier class is the base building block of the project, all of its functionnalities are tested. The 1D FFT test
passes and the 2D FFT test passes. Tests on the FFT method are equality tests between the FFT obtained using our methods and one computed
using trusted tools. All methods used for the padding of the signal pass the tests. The inverse FFT method is assessed by comparing
the original signal with the result of the inverse FFT method, all tests pass. Finally, the loading methods are also assessed and pass the tests.

Since the filter implementation is using filters that are not smooth, the results have lots of artifacts. Because of this, the tests on the
filter implementation are not absolute tests where the results of the methods are compared with results of trusted filtering tools.
Instead the methods are used on predefined test signals, the results are inspected to make sure they still make sens even if they are not perfect. 
These results are then used as a test to make sure that the implementation of the filters stayed consitent between updates of the project.

**Visualization**: The histogram class has been tested by generating two images using python, then creating histograms on python using the
same pipeline that we use in the histogram class but with tools from packages. Then we use Google Tests to assess if they are similar.
The test also assesses if the sum of pixel probabilities equals to 1.


## Todos and Limitations

### Todos
- **Support Additional Input Types**: Extend support to more input formats beyond WAV, BMP, and PNG.
- **Expand Filtering Options**: Implement new filters, both with and without Fourier transform, to compare results. For example, add contour extraction without Fourier transform, based on pixel intensity gradients. Other types
of filters could also be implemented such as band-pass or band-stop. Finally, more advanced features such as noise canceling could be added.
- **Improve Visualization**: Add more visualization options for improved data interpretation. Stem plots could be implemented to visualize the amplitude of the FFT and show it to the user allowing him to chose a cutoff
frequency based on it.
- **Executable Input Option**: Add support for running the program with a single configuration file as input, alongside the current menu-driven interface.

### Current Limitations
- **Input Formats**: Supports only WAV, BMP, and PNG; more formats need to be added. Also, the WAV reader only supports the most common uncompressed PCM audio formats. Similarly, the PNG reader only supports PNG images with up to 3 color channels. Other audio formats could be added for more 
flexibility.
- **Output Generation**: The same thing can be said for output generation, the MP3Output class has only one saving format (PCM 32bit float).
Different opttions could be implemented along with a menu to select the encoding format the user wants to use. Finally, some other file types
could also be implemented.
- **Menu-Driven Interface**: While user-friendly, it may not be ideal for automated or batch processing workflows.
- **Static Bin and Filter Configurations**: Histogram bins and filter configurations are fixed and cannot adapt dynamically to the input data. Currenty filters may introduce numerical artifacts, which smoother designs can mitigate.




