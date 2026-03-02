#include "header.hpp"

char board[8][8];
std::mutex boardMutex;
std::atomic<bool> running(true);
std::map<char, sf::Texture> textures;

int main()
{
    loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", board);

    sf::RenderWindow window(
        sf::VideoMode(TILE_SIZE * 8, TILE_SIZE * 8),
        "Echecs - FEN Live"
    );

    std::thread input(inputThread);

    std::string pieces = "PRNBQKprnbqk";

    for (char p : pieces)
    {
        sf::Texture tex;
        tex.loadFromFile(pieceToFile(p));
        textures[p] = tex;
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
                window.close();
            }
        }

        window.clear();

        {
            std::lock_guard<std::mutex> lock(boardMutex);
            drawBoard(window, board);
        }

        window.display();
    }

    running = false;
    input.join();
    return 0;
}
