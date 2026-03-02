#include "../../include/RobotShell.hpp"

int main() {
    try {
        RobotShell shell(
            "./banners/shell_banner.txt",
            "./librairies/libNeuralNetwork.so",
            "./librairies/libExperimentRunner.so"
        );
        shell.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
