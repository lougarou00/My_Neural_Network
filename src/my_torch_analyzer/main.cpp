#include "../../include/MyTorchAnalyzer.hpp"

int main(int argc, char **argv) {
    try {
        if (argc != 4) {
            MyTorchAnalyzer::display_help();
            return 84;
        }
        if (argc == 2 && strcmp(argv[1], "--help")) {
            MyTorchAnalyzer::display_help();
            return 0;
        }
        MyTorchAnalyzer analyzer(
            "./librairies/libNeuralNetwork.so",
            "./librairies/libExperimentRunner.so"
        );
        std::string flag = argv[1];
        if (flag == "--predict") {
            analyzer.predict(argv[3]);
            return 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 84;
    }
    return 0;
}