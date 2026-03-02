#include "header.hpp"


void inputThread()
{
    while (running)
    {
        std::string fen;
        std::cout << "> ";
        std::getline(std::cin, fen);

        if (!running) break;
        if (fen.empty()) continue;

        std::lock_guard<std::mutex> lock(boardMutex);
        loadFEN(fen, board);
    }
}


void loadFEN(const std::string& fen, char board[8][8])
{
    int row = 0, col = 0;

    for (char c : fen)
    {
        if (c == ' ') break;

        if (c == '/')
        {
            row++;
            col = 0;
        }
        else if (isdigit(c))
        {
            int empty = c - '0';
            for (int i = 0; i < empty; i++)
                board[row][col++] = '.';
        }
        else
        {
            board[row][col++] = c;
        }
    }
    std::cout << "Board updated to FEN: " << fen << std::endl;
}

std::string pieceToFile(char piece)
{
    switch (piece)
    {
        case 'P': return "pieces/wp.png";
        case 'R': return "pieces/wr.png";
        case 'N': return "pieces/wn.png";
        case 'B': return "pieces/wb.png";
        case 'Q': return "pieces/wq.png";
        case 'K': return "pieces/wk.png";

        case 'p': return "pieces/bp.png";
        case 'r': return "pieces/br.png";
        case 'n': return "pieces/bn.png";
        case 'b': return "pieces/bb.png";
        case 'q': return "pieces/bq.png";
        case 'k': return "pieces/bk.png";
    }
    return "";
}

void drawBoard(sf::RenderWindow& window, char board[8][8])
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            // ---- Dessin de la case ----
            sf::RectangleShape square(
                sf::Vector2f(TILE_SIZE, TILE_SIZE)
            );
            square.setPosition(col * TILE_SIZE, row * TILE_SIZE);

            if ((row + col) % 2 == 0)
                square.setFillColor(sf::Color(240, 217, 181));
            else
                square.setFillColor(sf::Color(181, 136, 99));

            window.draw(square);

            // ---- Dessin de la pièce ----
            char piece = board[row][col];
            if (piece != '.')
            {
                sf::Sprite sprite;
                sprite.setTexture(textures[piece]);

                sprite.setPosition(
                    col * TILE_SIZE,
                    row * TILE_SIZE
                );

                // Mise à l’échelle automatique
                float scale =
                    static_cast<float>(TILE_SIZE) /
                    textures[piece].getSize().x;

                sprite.setScale(scale, scale);

                window.draw(sprite);
            }
        }
    }
}

