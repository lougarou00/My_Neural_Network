# Neural Network Evaluation Documentation

## 1. Overview

The evaluation process allows you to measure how well a trained neural network performs on unseen data.  
It uses a test dataset (vector of `TrainingSample`) and computes standard machine learning metrics, as well as the confusion matrix.

---

## 2. Test Data Format

Each test sample should follow the same input/output convention as the training data:

- **Input**: First 65 values (board state + side to move)  
- **Output**: Last 5 values (one-hot encoded game state)

The `DataLoader::loadData` function can be used to load the dataset from a file.

---

## 3. Evaluation Method

1. **Load Test Data**: Use `DataLoader::loadData(test_file)` to parse the file and generate `TrainingSample` objects.
2. **Compute Metrics**: Pass the dataset to the network evaluation function, e.g., `network.evaluate(test_data)`.
3. **Output Metrics**: Print or log the following metrics for analysis:
    - Accuracy
    - Loss
    - Precision
    - Recall
    - F1 Score
4. **Confusion Matrix**: Construct a matrix to visualize prediction errors.

---

## 4. Metrics Definitions

Assuming `y_true` is the ground truth vector and `y_pred` is the network prediction:

### 4.1 Accuracy

Proportion of correct predictions:

\[
\text{Accuracy} = \frac{\text{Number of correct predictions}}{\text{Total number of predictions}}
\]

### 4.2 Loss (Cross-Entropy)

For multi-class classification with one-hot encoded labels:

\[
L = - \sum_{i=1}^{C} y_i \log(\hat{y}_i)
\]

Where:

- \(C\) = number of classes (5)
- \(y_i\) = 1 if the true class is \(i\), 0 otherwise
- \(\hat{y}_i\) = predicted probability for class \(i\)

Clamp \(\hat{y}_i\) in \([1e-7, 1-1e-7]\) to avoid log(0).

### 4.3 Precision

Precision for a class \(i\) is the proportion of correct positive predictions:

\[
\text{Precision}_i = \frac{TP_i}{TP_i + FP_i}
\]

Where:

- \(TP_i\) = true positives for class \(i\)
- \(FP_i\) = false positives for class \(i\)

Average over all classes for macro-precision.

### 4.4 Recall

Recall for a class \(i\) is the proportion of actual positives correctly predicted:

\[
\text{Recall}_i = \frac{TP_i}{TP_i + FN_i}
\]

Where:

- \(FN_i\) = false negatives for class \(i\)

Average over all classes for macro-recall.

### 4.5 F1 Score

Harmonic mean of precision and recall:

\[
F1_i = 2 \cdot \frac{\text{Precision}_i \cdot \text{Recall}_i}{\text{Precision}_i + \text{Recall}_i}
\]

Average over all classes for macro-F1.

---

## 5. Confusion Matrix

The confusion matrix shows how predictions compare to actual labels.  

For **5 classes**, the layout is:

| Predicted \ Actual | Class 0 | Class 1 | Class 2 | Class 3 | Class 4 |
|------------------|---------|---------|---------|---------|---------|
| **Class 0**      | TP0     | FP      | FP      | FP      | FP      |
| **Class 1**      | FN      | TP1     | FP      | FP      | FP      |
| **Class 2**      | FN      | FN      | TP2     | FP      | FP      |
| **Class 3**      | FN      | FN      | FN      | TP3     | FP      |
| **Class 4**      | FN      | FN      | FN      | FN      | TP4     |

- Rows = predicted classes  
- Columns = actual classes  
- Diagonal values = true positives  
- Off-diagonal values = misclassifications

This matrix helps identify which classes the network confuses the most.

---
