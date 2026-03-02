#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "../../include/nlohmann/json.hpp"

using json = nlohmann::json;

struct BenchmarkEntry {
    std::string neural_network;
    std::string start_time;
    std::string end_time;
    int training_duration_sec = 0;
    bool multithreaded = false;
    int threads_used = 0;
    int total_samples = 0;
    double accuracy = 0.0;
    double loss = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1_score = 0.0;
    std::string training_config_path;
};

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return str;
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void process_data() {
    std::string benchmark_txt_path = "benchmark_value/benchmark_data.txt";
    std::string target_json_path = "neural-benchmark-dashboard/public/benchmark-data.json";
    
    std::ifstream input_file(benchmark_txt_path);
    if (!input_file.is_open()) return;

    std::vector<BenchmarkEntry> entries;
    std::string line;
    BenchmarkEntry current_entry;
    bool has_data = false;

    while (std::getline(input_file, line)) {
        line = trim(line);
        if (line == "----" || line.empty()) {
            if (has_data && !current_entry.neural_network.empty()) {
                entries.push_back(current_entry);
            }
            current_entry = BenchmarkEntry();
            has_data = false;
            continue;
        }

        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;

        std::string key = trim(line.substr(0, pos));
        std::string value = trim(line.substr(pos + 1));

        if (key == "Neural_Network") current_entry.neural_network = value;
        else if (key == "start_time") current_entry.start_time = value;
        else if (key == "end_time") current_entry.end_time = value;
        else if (key == "training_duration_sec") current_entry.training_duration_sec = std::stoi(value);
        else if (key == "multithreaded") current_entry.multithreaded = (value == "true");
        else if (key == "threads_used") current_entry.threads_used = std::stoi(value);
        else if (key == "total_samples") current_entry.total_samples = std::stoi(value);
        else if (key == "accuracy") current_entry.accuracy = std::stod(value);
        else if (key == "loss") current_entry.loss = std::stod(value);
        else if (key == "precision") current_entry.precision = std::stod(value);
        else if (key == "recall") current_entry.recall = std::stod(value);
        else if (key == "f1_score") current_entry.f1_score = std::stod(value);
        else if (key == "training_config") current_entry.training_config_path = value;
        
        has_data = true;
    }
    if (has_data && !current_entry.neural_network.empty()) {
        entries.push_back(current_entry);
    }
    input_file.close();

    json final_data_array = json::array();
    std::ifstream existing_json_file(target_json_path);
    if (existing_json_file.is_open()) {
        existing_json_file >> final_data_array;
        existing_json_file.close();
    }

    for (const auto& entry : entries) {
        json training_json;
        std::ifstream t_file(entry.training_config_path);
        if (t_file.is_open()) {
            t_file >> training_json;
            t_file.close();
        } else {
            continue;
        }

        json config_json;
        std::string config_path = "NN_Config/" + entry.neural_network + ".json";
        std::ifstream c_file(config_path);
        if (c_file.is_open()) {
            c_file >> config_json;
            c_file.close();
        } else {
            continue;
        }

        json new_experiment;
        new_experiment["experiment_id"] = entry.neural_network;
        new_experiment["date"] = entry.start_time;
        
        new_experiment["network_config"]["num_layers"] = config_json["layers"].size();
        new_experiment["network_config"]["layers"] = config_json["layers"];
        new_experiment["network_config"]["learning_rate"] = training_json.value("eta", 0.0);
        new_experiment["network_config"]["batch_size"] = training_json.value("mini_batch_size", 0);
        
        new_experiment["training_data_size"] = entry.total_samples;
        
        new_experiment["performance"]["accuracy"] = entry.accuracy;
        new_experiment["performance"]["loss"] = entry.loss;
        new_experiment["performance"]["precision"] = entry.precision;
        new_experiment["performance"]["recall"] = entry.recall;
        new_experiment["performance"]["f1_score"] = entry.f1_score;
        new_experiment["performance"]["training_duration_sec"] = entry.training_duration_sec;
        new_experiment["performance"]["multithreaded"] = entry.multithreaded;
        new_experiment["performance"]["threads_used"] = entry.threads_used;
        
        new_experiment["timestamps"]["start_time"] = entry.start_time;
        new_experiment["timestamps"]["end_time"] = entry.end_time;
        new_experiment["notes"] = "Automated benchmark import";

        final_data_array.push_back(new_experiment);
    }

    std::ofstream out_json(target_json_path);
    out_json << std::setw(4) << final_data_array << std::endl;
    out_json.close();

    std::ofstream clear_txt(benchmark_txt_path, std::ios::trunc);
    clear_txt.close();
}

int main() {
    process_data();
    return 0;
}