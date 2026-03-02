# Benchmark Import Script Documentation

## Overview

The `benchmark` script is a C++ utility designed to automate the collection of neural network training results. It parses raw text logs, cross-references them with specific configuration files, and appends the structured data into a central JSON database for dashboard visualization.

## Prerequisites

* **C++ Compiler**: A compiler supporting C++11 or higher (e.g., g++).
* **JSON Library**: `nlohmann/json` must be available in your include path.

## Project Structure

The script expects the following directory layout:

* `benchmark_value/benchmark_data.txt`: The source log file.
* `NN_Training_Config/`: Contains training parameters (epochs, learning rate).
* `NN_Config/`: Contains neural network architecture details.
* `neural-benchmark-dashboard/public/benchmark-data.json`: The destination database.
* `bin/`: Directory containing the compiled executable.

## Compilation

To compile the script and place the binary in the `bin` folder, run:

```bash
mkdir -p bin
g++ -std=c++11 main.cpp -o bin/benchmark

```

## Usage

Execute the script from the project root using:

```bash
./bin/benchmark

```

## Functional Logic

1. **Parsing**: The script reads `benchmark_value/benchmark_data.txt`. Each entry is separated by `----`.
2. **Validation**: If a data block does not contain a `Neural_Network` identifier, it is automatically ignored.
3. **Data Enrichment**:
* It opens the file specified in `training_config` to extract hyperparameters.
* It constructs a file path based on the `Neural_Network` value (e.g., `NN_Config/64-32-RELU-SIGMOID.json`) to retrieve the layer architecture.


4. **Integration**: The combined data is formatted into a new JSON object and appended to the existing list in `neural-benchmark-dashboard/public/benchmark-data.json`.
5. **Cleanup**: Once the processing is successful, the contents of `benchmark_value/benchmark_data.txt` are completely cleared.