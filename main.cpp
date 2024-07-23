#include <iostream>
#include <filesystem>
#include "Simulation.h"
#include "MyAlgorithm.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string houseFilePath = argv[1];
    std::string outputFilePath = std::filesystem::path(houseFilePath).stem().string() + "_output.txt";

    Simulation simulator;
    simulator.readHouseFile(houseFilePath);

    MyAlgorithm algo;
    simulator.setAlgorithm(algo);
    simulator.run();
    simulator.writeOutputFile(outputFilePath);

    std::cout << "Output written to: " << outputFilePath << std::endl;

    return 0;
}
