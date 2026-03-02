#ifndef ROBOTSHELL_HPP
#define ROBOTSHELL_HPP

#include "Utils.hpp"

class RobotShell {
private:
    std::string bannerFile;
    std::string bannerText;

    std::unique_ptr<DLLoader<NeuralNetwork>> nnLoader;
    std::unique_ptr<DLLoader<ExperimentRunner>> runnerLoader;

    NeuralNetwork *currentNN;
    ExperimentRunner* runner;
    std::string _nn;
    

    // Utils
    Utils _utils;
    std::vector<std::string> split(const std::string& str) const;
    void loadBanner();
    void printBanner() const;
    void printPrompt() const;

    // Commands
    void handleCommand(const std::vector<std::string>& tokens);

public:
    RobotShell(const std::string& bannerPath,
               const std::string& nnLibraryPath,
               const std::string& runnerLibraryPath);
    ~RobotShell();

    void run();
};

#endif
