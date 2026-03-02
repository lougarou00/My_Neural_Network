# Neural Network Training System Documentation

## 1. Overview

The training system is designed to allow users to train neural networks for chess evaluation based on board states. It separates responsibilities into two main components:

- **ExperimentRunner**: Handles training, network generation, evaluation, and loading/saving of networks.
- **DataLoader**: Handles conversion of raw data files into `TrainingSample`s for training.

Users provide:

1. A **JSON configuration file** describing hyperparameters.
2. A **training dataset file** in the standardized input format.

The system automates the rest, including shuffling, mini-batch creation, and network updates.

---

## 2. Input Data Format

Each line in the dataset represents a single board state with associated output labels:

| Component       | Size |
| --------------- | ---- |
| Board squares   | 64   |
| Side to move    | 1    |
| Output labels   | 5    |
| **TOTAL**       | 65 + 5 = 70 values |

- **Board squares**: Encodes the pieces on the board.
- **Side to move**: Indicates which player's turn it is (0 or 1).
- **Output labels**: One-hot encoding of the game state (check, checkmate, nothing, etc.)

`DataLoader` parses this into `TrainingSample`s:

```cpp
using TrainingSample = std::pair<std::vector<float>, std::vector<float>>;
```

First 65 values → input.  

Last 5 values → output.  

A separate executable/script converts raw FEN data into this standardized format.  

## 3. Hyperparameters JSON

The training hyperparameters are defined in a JSON file with the following structure:

```json
{
    "epochs": 50,
    "mini_batch_size": 32,
    "eta": 0.01
}
```

## Field Descriptions

| Field           | Description                                           |
|-----------------|-------------------------------------------------------|
| epochs          | Number of full passes over the training dataset      |
| mini_batch_size | Size of mini-batches for stochastic gradient descent |
| eta             | Learning rate used for gradient descent updates      |

A provided script can generate this JSON automatically, so manual editing is optional.

## 4. Training Process

1. **Load Hyperparameters:** `ExperimentRunner` reads the JSON file and retrieves `epochs`, `mini_batch_size`, and `eta`.

2. **Load Dataset:** `DataLoader::loadData` parses the training file and generates a vector of `TrainingSample`s.

3. **Shuffle Data:** The dataset is randomly shuffled to ensure stochasticity.

4. **Mini-Batch Creation:** The dataset is split into mini-batches of size `mini_batch_size`.

5. **Network Update:** For each mini-batch, `NeuralNetwork::update_mini_batch` is called using the learning rate `eta`.

6. **Epoch Completion:** Steps 3–5 are repeated for the number of epochs defined.

7. **Progress Tracking:** Optionally, the system can display the epoch number or evaluation metrics.

## 5. Output and Logging

- After training, the network can be saved using `NeuralNetwork::save`.

- Optional metrics can be logged after each epoch (accuracy, loss) if evaluation data is provided.

## 6. Notes

- Users cannot modify the input or output layer neuron counts, as they are tied to the data format:
  - **Input layer:** 65 neurons
  - **Output layer:** 5 neurons

- Hidden layers can be fully customized in size and activation.

- All weights and biases are initialized randomly if not provided.

- Scripts are provided for:
  - Generating hyperparameters JSON files automatically
  - Converting raw FEN data into the standardized training format

This design ensures that networks remain compatible with the input/output conventions while allowing flexibility in training hidden layers.
