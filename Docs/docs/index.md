---
layout: home

hero:
  name: MY_TORCH
  text: Where machine learning meets chess strategy.
  tagline: Neural Network Engine | FEN Analysis | Supervised Training | Model Benchmarking
  actions:
    - theme: brand
      text: Architecture Overview
      link: /Architecture/ARCHITECTURE
    - theme: alt
      text: Project details
      link: /MY_TOUCH

features:
  - icon: "🧱"
    title: System Architecture
    details: Clean MY_TOUCH architecture with data-oriented design for optimal performance and flexibility.
    link: /Architecture/ARCHITECTURE
  - icon: "🔧"
    title: Technical Study
    details: Comparative analysis of technologies, algorithms, and design decisions.
    link: /Technical/TECHNICAL_DOCUMENTATION
  - icon: "📘"
    title: Developer Guide
    details: Contributing guidelines, coding conventions, and best practices.
    link: /Developer/CONTRIBUTING
  - icon: "⚙️"
    title: Build & Deploy
    details: CMake build system.
    link: /Build/BUILD_GUIDE
---

<img src="/index_image.png" alt="MY_TOUCH" style="width: 1200px; height: auto; margin-top: 40px; margin-bottom: 80px" />

# MyTorch – Chess Neural Network Analyzer

**Predict the game before it is played.**
Custom Neural Network | Chess State Prediction | Supervised Learning | Benchmark Dashboard


## Overview

MyTorch is a machine-learning project focused on predicting the current state of a chess game using a **custom-built neural network** implemented from scratch.

The project is developed as part of an Epitech AI and systems curriculum and aims to demonstrate:

1. A deep understanding of neural network fundamentals
2. A full supervised learning pipeline without external ML frameworks
3. Robust training, evaluation, and benchmarking methodologies
4. Clear visualization of model evolution over time

Given a chessboard position encoded in **Forsyth–Edwards Notation (FEN)**, the system predicts the state of the game:

* Checkmate
* Check
* Nothing

This README documents the architecture, design choices, usage, and benchmarking strategy of the project.


# Table of Contents

1. Introduction
2. Global Architecture
3. Neural Network Design
4. Chessboard Representation
5. Training and Prediction
6. Benchmarking System
7. Benchmark Data Format
8. Benchmark Dashboard
9. Error Handling
10. Testing Strategy
11. Performance and Optimization
12. Future Improvements
13. Author


# 1. Introduction

MyTorch is built around a fully custom neural network engine, designed without using libraries such as PyTorch, TensorFlow, or Keras.

The project emphasizes:

* Mathematical correctness
* Explicit control over training behavior
* Transparency of learning evolution
* Reproducibility and benchmarking

The neural network is evaluated against a classical chess algorithm, making correctness measurable and verifiable.


# 2. Global Architecture

The project is divided into three main components:

### Neural Network Core

A reusable module responsible for network creation, training, and inference.

### Chess Analyzer

A command-line tool that loads a neural network and applies it to chessboard positions.

### Benchmark System

A structured benchmarking pipeline combined with a visual dashboard to track performance over time.

```
my_torch/
├── core/
│   ├── network/
│   ├── training/
│   ├── activation/
│   └── serialization/
├── analyzer/
│   └── my_torch_analyzer
├── benchmarks/
│   └── results.json
└── dashboard/
    └── react-benchmark-ui
```


# 3. Neural Network Design

The neural network is a feedforward multilayer perceptron supporting:

* Arbitrary number of layers
* Configurable layer sizes
* Multiple activation functions
* Adjustable learning rate and batch size
* Supervised training with backpropagation

All computations are explicit and deterministic.


# 4. Chessboard Representation

Chess positions are provided using **Forsyth–Edwards Notation (FEN)**.

Each FEN string is transformed into a numerical input vector representing:

* Piece positions
* Turn information
* Castling rights
* En passant availability

This encoding is consistent across training and prediction phases.


# 5. Training and Prediction

The analyzer can be launched in two modes:

### Training Mode

```
./my_torch_analyzer --train [--save SAVEFILE] LOADFILE CHESSFILE
```

Each line in `CHESSFILE` contains:

```
<FEN_STRING> <EXPECTED_STATE>
```

The network is trained using supervised learning.

### Prediction Mode

```
./my_torch_analyzer --predict LOADFILE CHESSFILE
```

Predictions are printed in the same order as inputs.


# 6. Benchmarking System

Benchmarking is a core part of the project.

Each training run produces structured metrics including:

* Accuracy
* Loss
* Precision
* Recall
* F1-score
* Training duration
* Multithreading information

These metrics allow objective comparison between different configurations.


# 7. Benchmark Data Format

Benchmarks are stored as JSON entries following a strict convention.

Each entry represents one epoch or training run:

```json
{
  "experiment_id": "exp_001",
  "date": "2025-12-16T14:35:00Z",
  "network_config": {
    "num_layers": 4,
    "learning_rate": 0.01,
    "batch_size": 32
  },
  "training_data_size": 60000,
  "performance": {
    "accuracy": 0.945,
    "loss": 0.21,
    "precision": 0.93,
    "recall": 0.92,
    "f1_score": 0.925
  }
}
```

Multiple entries may share the same `experiment_id`, allowing time-based analysis of model evolution.


# 8. Benchmark Dashboard

The benchmark dashboard is a web application built with:

* React JS
* Tailwind CSS
* A charting library for data visualization

### Features

* Experiment selection by ID
* Time-based performance graphs
* Accuracy and loss evolution
* Clickable epochs for detailed inspection
* Reload mechanism to fetch new benchmark entries
* Non-destructive data updates

This dashboard provides visual proof of learning quality and training progress.


# 9. Error Handling

All errors must:

1. Print a clear message to stderr
2. Stop execution immediately
3. Exit with code 84

Handled errors include:

* Invalid network files
* Invalid FEN notation
* Dimension mismatches
* Invalid training data
* Numerical instability


# 10. Testing Strategy

The project includes:

* Unit tests for core math functions
* Validation of forward and backward propagation
* Integration tests for training and prediction
* Benchmark consistency checks

Testing ensures correctness, stability, and reproducibility.


# 11. Performance and Optimization

Several strategies are explored:

* Learning rate tuning
* Batch size experimentation
* Weight initialization strategies
* Multithreaded training
* Overfitting detection

Benchmark results are used to justify all design decisions.


# 12. Future Improvements

Planned extensions include:

* Checkmate side prediction (White / Black)
* Stalemate detection
* Advanced optimization techniques
* Real-time benchmark updates
* Deeper architectures
* Full chess-playing AI