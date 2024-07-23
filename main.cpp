#include <iostream>
#include "Simulation.h"
#include "MyAlgorithm.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <house_input_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string houseFilePath = argv[1];
    Simulation simulator;
    simulator.readHouseFile(houseFilePath);

    MyAlgorithm algo;
    simulator.setAlgorithm(algo);
    simulator.run();

    return 0;
}
