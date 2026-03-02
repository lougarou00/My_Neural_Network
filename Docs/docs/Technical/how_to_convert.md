# FEN to Neural Network Input Encoding Specification

## Purpose

This document specifies **exactly** how to convert a chess position written in **FEN format** into a **numerical representation suitable for a neural network**.

The goal is to transform each input line such as:

```
8/8/R2k4/4r1p1/8/5K2/5P2/8 b - - 7 59 Check White
```

into a **single output file** containing **69 numerical values on one line**, and a corresponding **output vector (label)** representing the game state:

* Check White
* Check Black
* Checkmate White
* Checkmate Black
* Nothing

This document is intended for a developer implementing the **FEN → numeric encoding** step.

---

## Input Format

Each input line contains:

```
<FEN_POSITION> <SIDE_TO_MOVE> <CASTLING> <EN_PASSANT> <HALF_MOVE> <FULL_MOVE> <LABEL>
```

Example:

```
8/8/R2k4/4r1p1/8/5K2/5P2/8 b - - 7 59 Check White
```

---

## Output Format

For **each input line**, the program must create **one output file** containing:

* **69 input values** (floating point or integers)
* **5 output values** (one-hot encoded label)

All values must appear **on a single line**, space-separated.

---

## 1. Board Encoding (64 values)

### Board Layout

* The chessboard is always encoded from **rank 8 to rank 1**, left to right (files a → h).
* This produces **64 values**, one per square.

### Piece Encoding

Each square is encoded as **one integer value** according to the piece occupying it:

| Piece            | Value |
| ---------------- | ----- |
| Empty square     | 0     |
| White Pawn (P)   | +1    |
| White Knight (N) | +2    |
| White Bishop (B) | +3    |
| White Rook (R)   | +4    |
| White Queen (Q)  | +5    |
| White King (K)   | +6    |
| Black Pawn (p)   | -1    |
| Black Knight (n) | -2    |
| Black Bishop (b) | -3    |
| Black Rook (r)   | -4    |
| Black Queen (q)  | -5    |
| Black King (k)   | -6    |

Example (partial):

```
R2k4
```

encodes as:

```
+4 0 0 -6 0 0 0 0
```

---

## 2. Additional Game-State Parameters (5 values)

After the 64 board values, append **5 additional parameters** corresponding exactly to the remaining FEN fields.

### 65. Side to Move

Encodes which player is to move.

| Value | Meaning             |
| ----- | ------------------- |
| 1     | White to move (`w`) |
| -1    | Black to move (`b`) |

---

### 66. Castling Rights (Aggregated)

4 digits encoding of castling availability:

| Value  | Meaning                       |
| ------ | ----------------------------- |
| 0      | No castling rights (`-`)      |
| 6      | White can kingside (`K`)      |
| 5      | White can queenside (`Q`)     |
| -6     | Black can kingside (`k`)      |
| -5     | Black can queenside (`q`)     |

---

### 67. En Passant Square

2 digits encoding the en-passant target square if it exists.

| Value | Meaning                            |
| ----- | ---------------------------------- |
| 0     | No en passant possible (`-`)       |
| x y   | Position of the en passant square  |

Note : 
```
x € {a=1, b=2, … h=8}
y € {1=1, 2=2, … 8=8}
```
---

### 68. Halfmove Clock

Number of half-moves since the last pawn move or capture.

* Integer value from FEN (typically 0–50)
* May be optionally normalized later by the ML pipeline

---

### 69. Fullmove Number

Full move count from the FEN.

* Positive integer
* Represents game progression (opening → endgame)

---


## Final Input Vector Size

| Component       | Size          |
| --------------- | ------------- |
| Board squares   | 64            |
| Side to move    | 1             |
| Castling rights | 4             |
| En passant square | 2             |
| Halfmove clock  | 1             |
| Fullmove number  | 1             |
| **TOTAL**       | **73 values** |

---

## 3. Output Label Encoding (Single Scalar)

The output is encoded as **five scalar values**, where the correct class is marked with `1` and all other positions are `0`.

### Label Mapping

| Game State      | Output Vector (5 values)         |
| --------------- | -------------------------------- |
| Check White     | [1, 0, 0, 0, 0]                 |
| Check Black     | [0, 1, 0, 0, 0]                 |
| Checkmate White | [0, 0, 1, 0, 0]                 |
| Checkmate Black | [0, 0, 0, 1, 0]                 |
| Nothing         | [0, 0, 0, 0, 1]                 |

This produces **five output neurons**, each representing one possible class. The network’s prediction sets `1` at the neuron corresponding to the predicted game state, and `0` elsewhere.


## Example (Simplified)

Input:

```
8/8/R2k4/4r1p1/8/5K2/5P2/8 b - - 7 59 Check White
```

Output (structure):

```
[64 board values] [parameters] -1 0 0 0 0 0 0 3 41 [output] 1 0 0 0 0 
```

---

## Summary

* One input line → one output file
* 73 numerical input values
* 5 one-hot encoded output values
* Deterministic, stateless conversion
* Ready for direct ingestion by a neural network

---

## Implementation Notes

* Do **not** infer chess rules (check, mate) during conversion
* Labels are provided explicitly in input
* Encoding must be consistent across all samples
* Output files must preserve ordering of values

---

If implemented exactly as specified, the resulting dataset will be **clean, deterministic, and suitable for neural network training in C++ or Python**.
