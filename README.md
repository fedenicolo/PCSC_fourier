# PCSC_fourier


## Histogram Class
Purpose
The Histogram class computes and visualizes histograms for numerical data, such as:
- Pixel intensities in images.
- Amplitude values in sound data.

The class uses Gnuplot for graphical visualization.

---

Features
- Numerical Data Support: Handles 1D array (sound and image data).
- Configurable Bins: Allows flexible bin sizes and data ranges.
- Normalized Output: Provides histograms as raw frequency counts or normalized probability distributions.
- Visualization: Uses Gnuplot to generate professional-looking bar charts.

---

Class Documentation

Public Methods
1. Histogram(int numBins, double minRange, double maxRange)
   Initializes the histogram with the specified number of bins and data range.
   Parameters:
   - numBins: Number of bins.
   - minRange: Minimum value in the data range.
   - maxRange: Maximum value in the data range.

2. void compute(const std::vector<double>& data)
   Computes the histogram by populating frequency counts for the input data.
   Parameter: data - Vector of numerical values.

3. std::vector<double> getNormalizedHistogram() const
   Returns the histogram as a normalized probability distribution (values between 0 and 1).

4. const std::vector<int>& getBins() const
   Provides raw frequency counts for each bin.

5. void visualize() const
   Generates a graphical representation of the histogram using Gnuplot.


## Instructions to build
1. cd into the root directory of the project. Update the submodules by running `git submodule update --init --recursive`
2. If you have previously built something in this directory, run `make clean` and `./clean.sh` to clear out any generated files. 
3. Run the command `cmake -DTESTS=ON -DDOCUMENTATION=ON .` to generate the Makefile for building.
4. To build the tests, run `make tests`. You may want to also set the `-j<NUM_THREADS>` flag to use multiple threads to
   compile. It is completely normal for long compilation times if you are compiling from scratch.
5. To run the tests run the command `make -i run_tests`
6. To build the main file run `make main`
7. To build the doxygen documentation, run `make doc_doxygen`. This will build the documentation in the ``docs` directory.
8. The main program and all test files will be in the `build` directory.
