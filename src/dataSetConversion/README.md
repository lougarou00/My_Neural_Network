
# Chess Dataset Converter

A C++ utility to convert chess position data from FEN (Forsyth-Edwards Notation) format to integer representation for machine learning applications.

## Features

- Converts FEN notation to numeric vectors
- Encodes chess pieces with signed integers
- Extracts board state information (castling rights, en passant, move counters)
- Supports game state labels (Check, Checkmate, Nothing)
- Unit tests included with Google Test framework

## Building

```bash
make all        # Build converter and tests
make tests      # Run unit tests
make clean      # Remove object files
make fclean     # Remove all generated files
```

## Usage

```bash
./converter --convert <input_file> [output_file]
./converter --tests <input_file> [output_file]
./converter --help
```

### Options

- `--convert`: Convert dataset to integer format (default output: `dataSetInt.txt`)
- `--tests`: Extract board info without labels (default output: `boardTest.txt`)
- `--help`: Display usage information

## Input Format

FEN notation with labels:
```
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 Check White
```

## Output Format

Space-separated integers representing:
- 64 board squares (piece values)
- Side to move
- Castling rights
- En passant square
- Halfmove clock and fullmove number
- Label encoding
