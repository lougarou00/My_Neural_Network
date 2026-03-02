/*
** EPITECH PROJECT, 2025
** G-CNA-500-COT-5-1-neuralnetwork-16
** File description:
** Utils
*/

#pragma once

#include "librairies.hpp"
#include "DLLoader.hpp"
#include "NeuralNetwork.hpp"
#include "ExperimentRunner.hpp"
#include <chrono>
#include <thread>
#include <iomanip>

class Utils {
    public:
        std::string _training_cofig;
        Utils() = default;
        ~Utils() = default;
        void getISOCurrentTime() {
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::tm* utc_tm = std::gmtime(&now_c);
            char buffer[25];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", utc_tm);
            if (_start_date.length() == 0) {
                _start_time = std::chrono::high_resolution_clock::now();
                _start_date = std::string(buffer);
            } else {
                _end_date = std::string(buffer);
                _end_time = std::chrono::high_resolution_clock::now();
            }
        }
        void writeStartDate(std::string nn) {
            std::ofstream file("benchmark_value/benchmark_data.txt", std::ios::app);
            if (file.is_open()) {
                file << "Neural_Network: " << nn << "\n";
                file << "start_time: " << _start_date << "\n";
                file << "end_time: " << _end_date << "\n";
                file << "training_duration_sec: " << std::chrono::duration_cast<std::chrono::seconds>(_end_time - _start_time).count() << "\n";
                file << "multithreaded: false" << "\n";
                file << "threads_used: 1" << "\n";
                file << "training_config: " << _training_cofig << "\n";
                file.close();
            } else {
                std::cerr << "Error: Could not open file for writing" << std::endl;
            }
        }

    private:
        std::string _start_date;
        std::string _end_date;
        std::chrono::_V2::system_clock::time_point _start_time;
        std::chrono::_V2::system_clock::time_point _end_time;
};
