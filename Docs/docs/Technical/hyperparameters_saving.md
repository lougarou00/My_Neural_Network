# Neural Network Persistence System Documentation

This document describes the complete persistence system used to **save**, **organize**, and **reload** neural networks in this project. It covers directory layout, configuration storage, binary formats for weights and biases, and the full lifecycle from training to restoration.

---

## 1. Overview

The goal of the persistence system is to:

* Store trained neural networks in a **reproducible and inspectable** way
* Allow **exact reconstruction** of a network (architecture + parameters)
* Separate **configuration metadata** from **numerical parameters**
* Support benchmarking, versioning, and experiment tracking

Each neural network is saved inside its own directory, uniquely identified by a network name.

---

## 2. Directory Structure

All saved neural networks are stored inside a root directory:

```
Neural_Networks/
```

For each network named `<network_name>`, the following structure is created:

```
Neural_Networks/
└── <network_name>/
    ├── <network_name>.json
    ├── weights/
    │   ├── weights-0.bin
    │   ├── weights-1.bin
    │   └── ...
    └── biases/
        ├── biases-0.bin
        ├── biases-1.bin
        └── ...
```

### Directory Responsibilities

| Path                  | Purpose                                        |
| --------------------- | ---------------------------------------------- |
| `<network_name>.json` | Global configuration and architecture metadata |
| `weights/`            | Binary weight parameters per layer             |
| `biases/`             | Binary bias parameters per layer               |

All directories are **automatically created if missing**.

If files already exist, they are **overwritten or deleted** before saving new data.

---

## 3. Configuration File (`<network_name>.json`)

The configuration file stores all **non-numerical** information required to rebuild the network structure and understand how it was trained.

**Note : The informations about the input layer are not saved. This layer has no weights and bias and just basically forward the information received**

### 3.1 Stored Information

* Training hyperparameters
* Number of layers
* Layer ordering
* Layer types (hidden / output)
* Neuron activation functions per layer
* Number of neurons per layer

### 3.2 JSON Structure

```json
{
  "epochs": 30,
  "mini_batch_size": 32,
  "learning_rate": 0.01,
  "layers": [
    { "index": 1, "layer_type": "HIDDEN", "activation": "SIGMOID", "number": 20 },
    { "index": 2, "layer_type": "HIDDEN", "activation": "RELU", "number": 20 },
    { "index": 3, "layer_type": "OUTPUT", "activation": "TANH", "number": 20 }
  ]
}
```

### 3.3 Explanation of Fields

| Field             | Description                                       |
| ----------------- | ------------------------------------------------- |
| `epochs`          | Number of training epochs used                    |
| `mini_batch_size` | Mini-batch size during SGD                        |
| `learning_rate`   | Learning rate (η)                                 |
| `layers`          | Ordered array describing network architecture     |
| `layers[index]`   | Index of the layer (order matters)                |
| `layer_type`      | `INPUT`, `HIDDEN`, or `OUTPUT`                    |
| `activation`      | Activation function used by neurons in that layer |
| `number`      | Number of neurons on the layer |

Supported activation types:

* `SIGMOID`
* `RELU`
* `TANH`
* `PERCEPTRON`

---

## 4. Binary Storage Format

Numerical parameters are stored in **binary format** for efficiency, compactness, and fast loading.

Weights and biases are stored **separately** and **per layer**.

---

## 4.1 Weights Files (`weights/weights-<layer>.bin`)

Each file corresponds to **one layer** and stores the weights of all neurons in that layer.

### Binary Layout (per neuron)

```
[int n_weights]
[float w0][float w1]...[float w(n_weights-1)]
```

### File Structure

```
Neuron 0:
  [int][float][float]...
Neuron 1:
  [int][float][float]...
...
Neuron N
```

### Notes

* `n_weights` indicates the number of inputs to the neuron
* Neurons are stored **in layer order**
* This format allows variable input sizes if needed

---

## 4.2 Biases Files (`biases/biases-<layer>.bin`)

Each file corresponds to **one layer** and stores the biases of all neurons.

### Binary Layout

```
[float bias_0][float bias_1]...[float bias_N]
```

### Notes

* One `float` per neuron
* Order **must match** the neuron order in the corresponding weights file

---

## 5. Saving Workflow

The saving process is split into dedicated methods:

1. `save_config(name)`
2. `save_weights(name)`
3. `save_biases(name)`
4. `save(name)` (calls all above)

### Key Behaviors

* Directories are created if missing
* Old files are deleted before writing
* Configuration is always overwritten
* Binary files are written using `std::ios::binary`

---

## 6. Loading / Restoring a Network

To restore a neural network:

1. Read `<network_name>.json`
2. Reconstruct the architecture:

   * Number of layers
   * Layer types
   * Activation functions
3. Create layers and neurons accordingly
4. Load `weights-<layer>.bin` and assign weights
5. Load `biases-<layer>.bin` and assign biases

This guarantees **exact reconstruction** of the trained model.

---

## 7. Design Rationale

This system was designed to:

* Decouple architecture from parameters
* Enable easy experimentation and benchmarking
* Support future extensions (dropout, batch-norm, etc.)
* Allow inspection without loading the full model
* Be framework-agnostic and lightweight

---

## 8. Extensibility

The system can be extended to store:

* Optimizer state
* Training metrics
* Dataset metadata
* Multithreading configuration
* Checkpoints per epoch

Without breaking backward compatibility.

---

## 9. Summary

* One directory per neural network
* JSON for architecture + hyperparameters
* Binary files for weights and biases
* One file per layer
* Deterministic, efficient, and scalable design

This persistence system enables reliable training, evaluation, benchmarking, and long-term reuse of neural networks.
