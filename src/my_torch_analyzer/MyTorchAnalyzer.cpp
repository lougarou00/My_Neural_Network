#include "../../include/MyTorchAnalyzer.hpp"

MyTorchAnalyzer::MyTorchAnalyzer(const std::string& nnLibraryPath,
    const std::string& runnerLibraryPath)
: currentNN(nullptr)
{
    // Load NeuralNetwork dynamically
    nnLoader = std::make_unique<DLLoader<NeuralNetwork>>(nnLibraryPath);
    runnerLoader = std::make_unique<DLLoader<ExperimentRunner>>(runnerLibraryPath);

    currentNN = nullptr;
    runner = runnerLoader->getInstance();

    currentNN = new NeuralNetwork(runner->loadNetwork("128-64-RELU-SOFTMAX"));
}

MyTorchAnalyzer::~MyTorchAnalyzer()
{
    delete currentNN;
    delete runner;
}

void MyTorchAnalyzer::predict(const std::string &filepath)
{
    runner->predict(*currentNN, filepath);
    return;
}

void MyTorchAnalyzer::display_help()
{
    std::cout << "USAGE" << std::endl;
    std::cout << "  ./my_torch_analyzer [--predict | --train [--save SAVEFILE]] LOADFILE CHESSFILE" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "DESCRIPTION" << std::endl;
    std::cout << "  --train Launch the neural network in training mode. Each chessboard in FILE must" << std::endl;
    std::cout << "contain inputs to send to the neural network in FEN notation and the expected output" << std::endl;
    std::cout << "separated by space. If specified, the newly trained neural network will be saved in" << std::endl;
    std::cout << "SAVEFILE. Otherwise, it will be saved in the original LOADFILE." << std::endl;
    std::cout << "  --predict Launch the neural network in prediction mode. Each chessboard in FILE must" << std::endl;
    std::cout << "contain inputs to send to the neural network in FEN notation, and optionally an expected" << std::endl;
    std::cout << "output." << std::endl;
    std::cout << "  --save Save neural network into SAVEFILE. Only works in train mode." << std::endl;
    std::cout << "" << std::endl;
    std::cout << "LOADFILE File containing an artificial neural network" << std::endl;
    std::cout << "CHESSFILE File containing chessboards" << std::endl;
    return;
}
