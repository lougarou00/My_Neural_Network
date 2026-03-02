# Neural Network Benchmark JSON Convention

To track experiments, visualize benchmarks, and compare performance, we use a standardized JSON format that contains:

- Network configuration (architecture, activation functions, hyperparameters)
- Performance metrics
- Training metadata (dataset size, duration, threading)
- Timestamps for tracking evolution over time

## Example JSON Structure

```json
{
  "experiment_id": "exp_001",
  "date": "2025-12-16T14:35:00Z",
  "network_config": {
    "num_layers": 4,
    "layers": [
      {"type": "input", "size": 784, "neuron_type": "perceptron"},
      {"type": "hidden", "size": 128, "neuron_type": "relu"},
      {"type": "hidden", "size": 64, "neuron_type": "sigmoid"},
      {"type": "output", "size": 10, "neuron_type": "sigmoid"}
    ],
    "learning_rate": 0.01,
    "batch_size": 32
  },
  "training_data_size": 60000,
  "performance": {
    "accuracy": 0.945,
    "loss": 0.21,
    "precision": 0.93,
    "recall": 0.92,
    "f1_score": 0.925,
    "training_duration_sec": 125.4,
    "multithreaded": true,
    "threads_used": 8
  },
  "timestamps": {
    "start_time": "2025-12-16T14:35:00Z",
    "end_time": "2025-12-16T14:37:05Z"
  },
  "notes": "Comparison of ReLU vs Sigmoid in hidden layers."
}

```

**Explanation of Fields**

| Field                             | Description                                                                 |
|----------------------------------|-----------------------------------------------------------------------------|
| experiment_id                     | Unique identifier for the experiment/run.                                   |
| date                              | Date and time when the experiment was executed (ISO 8601 format).           |
| network_config                    | Network architecture and hyperparameters.                                   |
| network_config.num_layers         | Total number of layers in the network.                                      |
| network_config.layers             | Array describing each layer: type (`input`, `hidden`, `output`), size (number of neurons), and neuron activation type. |
| network_config.learning_rate      | Learning rate used during training.                                         |
| network_config.batch_size         | Mini-batch size used during training.                                       |
| training_data_size                | Number of samples in the training dataset.                                  |
| performance                       | Metrics and results of the training, using standard ML metrics.            |
| performance.accuracy              | Accuracy of the model on the evaluation dataset (proportion of correct predictions). |
| performance.loss                  | Loss function value after training (e.g., cross-entropy, MSE).             |
| performance.precision             | Precision score for classification tasks (optional, if applicable).        |
| performance.recall                | Recall score for classification tasks (optional, if applicable).           |
| performance.f1_score              | F1-score for classification tasks (optional, if applicable).               |
| performance.training_duration_sec | Total duration of the training in seconds.                                  |
| performance.multithreaded         | Boolean flag indicating whether multi-threading was enabled.                |
| performance.threads_used          | Number of threads used during training (0 or 1 for single-threaded, >1 for multithreaded). |
| timestamps                        | Start and end timestamps for the training session.                          |
| notes                             | Optional notes for additional context or comparisons.                       |
