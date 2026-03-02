#ifndef DISPLAY_HEADER_HPP
#define DISPLAY_HEADER_HPP

#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>

const int TILE_SIZE = 80;
const int BOARD_SIZE = 8;

#include <thread>
#include <mutex>
#include <atomic>

extern char board[8][8];
extern std::mutex boardMutex;
extern std::atomic<bool> running;
extern std::map<char, sf::Texture> textures;

void loadFEN(const std::string& fen, char board[8][8]);
std::string pieceToFile(char piece);
void inputThread();
void drawBoard(sf::RenderWindow& window, char board[8][8]);

#endif // DISPLAY_HEADER_HPP