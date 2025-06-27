#include <boost/math/distributions.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <algorithm> // For std::max_element and std::min_element
#include <map>       // For histogram


// Function to create a histogram
std::map<int, int> createHistogram(const std::vector<double>& data, int numBins, double minVal, double maxVal) {
    std::map<int, int> histogram;
    double binWidth = (maxVal - minVal) / numBins;

    for (double val : data) {
        if (val >= minVal && val <= maxVal) {
            int binIndex = static_cast<int>((val - minVal) / binWidth);
            // Ensure the last element falls into the last bin
            if (binIndex == numBins) {
                binIndex--;
            }
            histogram[binIndex]++;
        }
    }
    return histogram;
}


int main(void){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis1(0.0, 1.0);
    boost::math::beta_distribution<> dist(2, 5);
    
    double randuniform = dis1(gen);
    //parameters and the random value on (0,1) you drew
    double randFromDist = quantile(dist, randuniform);
    
    const int numSamples = 10000;
    const std::string dataFileName = "distribution_data.txt";
    const std::string scriptFileName = "plot_script.gp";
    const std::string outputFileName = "distribution_plot.png";

    std::vector<double> samples;
    samples.reserve(numSamples);

    // 1. Generate samples from the distribution
    std::cout << "Generating " << numSamples << " samples from the distribution..." << std::endl;
    for (int i = 0; i < numSamples; ++i) {
        double randuniform = dis1(gen);
    //parameters and the random value on (0,1) you drew
        samples.push_back(quantile(dist, randuniform));
    }

    // Determine min and max values for histogram
    double minVal = *std::min_element(samples.begin(), samples.end());
    double maxVal = *std::max_element(samples.begin(), samples.end());

    // Adjust min/max slightly to ensure all values fall into bins
    minVal -= (maxVal - minVal) * 0.01;
    maxVal += (maxVal - minVal) * 0.01;

    const int numBins = 50; // Number of bins for the histogram
    double binWidth = (maxVal - minVal) / numBins;

    // 2. Create histogram data
    std::map<int, int> histogram = createHistogram(samples, numBins, minVal, maxVal);

    // 3. Write histogram data to a file
    std::ofstream dataFile(dataFileName);
    if (!dataFile.is_open()) {
        std::cerr << "Error: Could not open data file " << dataFileName << std::endl;
        return 1;
    }
    std::cout << "Writing histogram data to " << dataFileName << "..." << std::endl;
    for (int i = 0; i < numBins; ++i) {
        double binCenter = minVal + (i + 0.5) * binWidth;
        dataFile << binCenter << " " << histogram[i] << std::endl;
    }
    dataFile.close();

    // 4. Generate gnuplot script
    std::ofstream scriptFile(scriptFileName);
    if (!scriptFile.is_open()) {
        std::cerr << "Error: Could not open gnuplot script file " << scriptFileName << std::endl;
        return 1;
    }
    std::cout << "Generating gnuplot script " << scriptFileName << "..." << std::endl;
    scriptFile << "set terminal pngcairo size 800,600 font 'Arial,12'\n";
    scriptFile << "set output '" << outputFileName << "'\n";
    scriptFile << "set title 'Distribution of randFromDist'\n";
    scriptFile << "set xlabel 'Value'\n";
    scriptFile << "set ylabel 'Frequency'\n";
    scriptFile << "set style fill solid 0.5 border -1\n";
    scriptFile << "set boxwidth " << binWidth * 0.9 << "\n"; // Slightly less than binWidth for separation
    scriptFile << "plot '" << dataFileName << "' using 1:2 with boxes title 'Histogram'\n";
    scriptFile.close();

    // 5. Execute gnuplot script
    std::string command = "gnuplot " + scriptFileName;
    std::cout << "Executing gnuplot command: " << command << std::endl;
    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Plot generated successfully as " << outputFileName << std::endl;
    } else {
        std::cerr << "Error: gnuplot command failed. Make sure gnuplot is installed and in your PATH." << std::endl;
        std::cerr << "Command executed: " << command << std::endl;
    }


    return 0;
} 
