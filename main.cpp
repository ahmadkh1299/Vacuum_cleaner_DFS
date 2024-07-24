#include "Simulation.h"
#include "MyAlgorithm.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string houseFilePath = argv[1];
    MyAlgorithm algo;
    Simulation simulator;

    if (!simulator.readHouseFile(houseFilePath)) {
        return EXIT_FAILURE;
    }

    simulator.setAlgorithm(algo);
    simulator.run();
    size_t lastDot = houseFilePath.find_last_of('.');
    std::string baseName = (lastDot == std::string::npos) ? houseFilePath : houseFilePath.substr(0, lastDot);
    std::string outputFilePath = baseName + "_output.txt";
    simulator.writeOutputFile(outputFilePath);

    return 0;
}
