# Chess FEN Live (Display)

🔧 **Purpose**

A small graphical tool to display a chessboard from a FEN string in real time.

## Main files

- `main.cpp` — Starts the SFML window, loads piece textures, spawns an input thread (reading FEN from stdin) and continuously renders the board.
- `loadFen.cpp` — Contains the FEN parsing logic (only the piece placement part up to the first space), the asynchronous input thread (`inputThread()`), the piece-to-file mapping (`pieceToFile()`), and the rendering function (`drawBoard()`).

> Note: Access to the board (`board`) is protected by a `std::mutex` (`boardMutex`) because input is handled from a separate thread.

## Dependencies

- SFML (graphics, window, system) — version 2.5+ recommended
- A modern C++ compiler (g++ / clang++), with C++17 support

## Build

From the `Display/` folder:

```bash
make
```

The `Makefile` uses `pkg-config` to fetch compiler and linker flags for SFML. If `pkg-config` or SFML are not installed, install them or adjust the `Makefile` with the appropriate flags.

Useful targets:
- `make` or `make all` — builds the `chess` executable
- `make run` — builds (if necessary) and runs `./chess`
- `make clean` — removes object files and the executable

## Run

Execute:

```bash
./chess
```

A window will appear showing the chessboard. Enter a FEN string in the console (stdin) and press Enter. Example:

```
> rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
Board updated to FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
```

The display updates as soon as a FEN is received.

## Supported FEN format

Only the piece placement field (field 1) is parsed; parsing stops at the first space. Accepted characters:
- `PNBRQK` for white pieces
- `pnbrqk` for black pieces
- digits `1`..`8` for consecutive empty squares
- `/` to separate ranks

Empty squares are represented internally as `.`.

## Resources

Piece image files must be located in `Display/pieces/` and named as follows: `wp.png`, `wr.png`, `wn.png`, `wb.png`, `wq.png`, `wk.png` for white and `bp.png`, `br.png`, `bn.png`, `bb.png`, `bq.png`, `bk.png` for black.

## Notes

- `TILE_SIZE` is defined in `header.hpp` (default 80) and can be adjusted to change square size.
- The code assumes all textures exist and are valid; there is no comprehensive error handling for texture loading (this can be improved if needed).

---

If you want, I can:
- add texture loading error checks, or
- extend the FEN parser to handle the full FEN string (castling rights, en-passant, side to move, etc.), or
- make the `Makefile` more portable (SFML detection for Windows/macOS).
