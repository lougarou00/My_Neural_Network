#pragma once
#include "librairies.hpp"
#include "DLLoader.hpp"
#include "NeuralNetwork.hpp"
#include "ExperimentRunner.hpp"

class MyTorchAnalyzer {
    private:
    
        std::unique_ptr<DLLoader<NeuralNetwork>> nnLoader;
        std::unique_ptr<DLLoader<ExperimentRunner>> runnerLoader;
    
        NeuralNetwork *currentNN;
        ExperimentRunner* runner;
    
    public:
        MyTorchAnalyzer(const std::string& nnLibraryPath,
            const std::string& runnerLibraryPath);
        ~MyTorchAnalyzer();
    
        void predict(const std::string &filepath);
        static void display_help();
};