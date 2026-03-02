# Neural Network Generation Guide

This document explains how to generate a neural network using our system and JSON configuration format.

---

## 1. Overview

The neural network generator allows you to create a blank network with configurable hidden layers and output layer.  
Some constraints apply to ensure compatibility with the training data and evaluation pipeline.

---

## 2. JSON Configuration Format

The network is defined using a JSON file that specifies **only the hidden layers and the output layer**.  
The structure looks like this:

```json
{
  "layers": [
    {"type": "hidden", "size": 784, "neuron_type": "PERCEPTRON"},
    {"type": "hidden", "size": 128, "neuron_type": "RELU"},
    {"type": "hidden", "size": 64, "neuron_type": "SIGMOID"},
    {"type": "output", "size": 5, "neuron_type": "SIGMOID"}
  ]
}
```

## 3. Layer Constraints

### Input Layer
- Automatically created with **65 neurons**.
- Cannot be customized (size, weights, or biases).
- No weights or biases are assigned.
- The input layer exists only to preserve indexing for hidden layers.

### Hidden Layers
- Can specify the number of neurons (**size**).
- Can specify the activation function (**neuron_type**).
- Supported neuron types (must be uppercase):
  - `SIGMOID`
  - `RELU`
  - `TANH`
  - `PERCEPTRON`
- Weights and biases are automatically generated randomly if not provided.

### Output Layer
- Always contains **5 neurons**.
- The number of neurons cannot be changed.
- Activation function (**neuron_type**) can be customized.

## 4. Neuron Type Options

| Neuron Type | Description                     |
| ----------- | ------------------------------- |
| SIGMOID     | Sigmoid activation function     |
| RELU        | Rectified Linear Unit activation|
| TANH        | Hyperbolic tangent activation   |
| PERCEPTRON  | Step function activation        |

## 5. Generation Process

1. Create a JSON file following the format described above.
2. Call the `generate` method in the `ExperimentRunner` class with the path to the JSON file.

The system will automatically:

- Add the input layer (65 neurons, no weights/biases)
- Create hidden layers with randomly initialized weights and biases
- Create the output layer with 5 neurons, customizable activation

## 6. Notes

- Users cannot change the number of input neurons or output neurons.
- Hidden layers can be fully customized within the limits above.
- Random initialization ensures the network is ready for training.
- A provided script can automatically generate the JSON configuration files, so manual editing is optional.

This setup ensures that networks remain compatible with the data format while allowing flexibility in the hidden layers and output activation.

