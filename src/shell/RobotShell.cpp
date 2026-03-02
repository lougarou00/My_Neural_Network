#include "../../include/RobotShell.hpp"

RobotShell::RobotShell(const std::string& bannerPath,
                       const std::string& nnLibraryPath,
                       const std::string& runnerLibraryPath)
    : bannerFile(bannerPath), currentNN(nullptr)
{
    // Load banner
    loadBanner();

    // Load NeuralNetwork dynamically
    nnLoader = std::make_unique<DLLoader<NeuralNetwork>>(nnLibraryPath);
    runnerLoader = std::make_unique<DLLoader<ExperimentRunner>>(runnerLibraryPath);

    currentNN = nullptr;
    runner = runnerLoader->getInstance();
}

RobotShell::~RobotShell()
{
    delete currentNN;
    delete runner;
}

std::vector<std::string> RobotShell::split(const std::string& str) const
{
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token)
        tokens.push_back(token);
    return tokens;
}

void RobotShell::loadBanner()
{
    std::ifstream file(bannerFile);
    if (!file.is_open()) {
        bannerText = "=== RobotAI Shell ===\n";
        return;
    }
    std::ostringstream oss;
    oss << file.rdbuf();
    bannerText = oss.str();
}

void RobotShell::printBanner() const
{
    std::cout << bannerText << "\n";
    std::cout << "Welcome to RobotAI Shell!\n";
    std::cout << "Here, we allow you to get a feel for the foundations of artificial intelligence.\n";
    std::cout << "Create, train, and save your own neural network to master chess game.\n";
    std::cout << "Type HELP for more informations...\n\n";
}

void RobotShell::printPrompt() const
{
    std::cout << "\033[1;36m[RobotAI]> \033[0m"; // cyan bold prompt
}

void RobotShell::handleCommand(const std::vector<std::string>& tokens)
{
    if (tokens.empty())
        return;

    std::string cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    try {
        if (cmd == "EXIT" || cmd == "QUIT") {
            if (currentNN)
                delete currentNN;
            exit(0);
        } 
        else if (cmd == "HELP") {
            std::cout << "Commands:\n";
            std::cout << "GENERATE <JSON_filename> - generate new NN\n";
            std::cout << "SAVE <filename>\nLOAD <filename>\nTRAIN <params_file> <training_file>\n";
            std::cout << "EVALUATE <dataset>\nPREDICT <input>\nEXIT or QUIT\n";
        } 
        else if (cmd == "GENERATE") {
            if (tokens.size() < 2) {
                std::cout << "Usage: GENERATE <param>\n";
                return;
            }
            delete currentNN;
            currentNN = new NeuralNetwork(runner->generate(tokens[1]));
            std::cout << "Neural Network generated.\n";
        } 
        else if (cmd == "SAVE") {
            if (tokens.size() < 2) {
                std::cout << "Usage: SAVE <filename>\n";
                return;
            }
            if (!currentNN) { std::cout << "No NN to save.\nCreate or load one first.\n"; return; }
            runner->saveNetwork(*currentNN, tokens[1]);
            std::cout << "Saved to " << tokens[1] << "\n";
        } 
        else if (cmd == "LOAD") {
            if (tokens.size() < 2) {
                std::cout << "Usage: LOAD <filename>\n";
                return;
            }
            delete currentNN;
            currentNN = new NeuralNetwork(runner->loadNetwork(tokens[1]));
            _nn = tokens[1];
            std::cout << "Loaded from " << tokens[1] << "\n";
        } 
        else if (cmd == "TRAIN") {
            if (tokens.size() < 3) {
                std::cout << "Usage: TRAIN <param_file> <training_file>\n";
                return;
            }
            if (!currentNN) { std::cout << "No NN to train.\nCreate or load one first.\n"; return; }
            _utils.getISOCurrentTime();
            runner->train(*currentNN, tokens[1], tokens[2]);
            _utils.getISOCurrentTime();
            _utils._training_cofig = tokens[1];
            _utils.writeStartDate(_nn);
            std::cout << "Training complete.\n";
        } 
        else if (cmd == "EVALUATE") {
            if (tokens.size() < 2) {
                std::cout << "Usage: EVALUATE <test_file>\n";
                return;
            }
            if (!currentNN) { std::cout << "No NN to evaluate.\nCreate or load one first.\n"; return; }
            runner->evaluate(*currentNN, tokens[1]);
        } 
        else if (cmd == "PREDICT") {
            if (tokens.size() < 2) {
                std::cout << "Usage: PREDICT <file>\n";
                return;
            }
            if (!currentNN) { std::cout << "No NN to predict.\n"; return; }
            runner->predict(*currentNN, tokens[1]);
        } 
        else {
            // fallback system command
            int ret = system(std::accumulate(tokens.begin(), tokens.end(), std::string(),
                                  [](std::string a, std::string b){ return a + " " + b; }).c_str());
            if (ret != 0)
                std::cerr << "Command exited with code " << ret << std::endl;
        }
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void RobotShell::run()
{
    printBanner();
    std::string line;

    while (true) {
        printPrompt();

        if (!std::getline(std::cin, line))
            break;

        auto tokens = split(line);
        handleCommand(tokens);
    }
}
