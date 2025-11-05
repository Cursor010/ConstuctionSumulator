#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include "Player.h"

class Game {
private:
    sf::RenderWindow window;
    sf::VideoMode desktopMode;
    sf::Font font;

    // Текстовые элементы
    sf::Text infoText;
    sf::Text playerText;
    sf::Text actionText;
    std::vector<sf::Text> buildingStats;  // Вектор для статистики зданий

    // Кнопки
    sf::RectangleShape endTurnButton;
    sf::Text endTurnText;
    sf::RectangleShape buildHouseButton;
    sf::Text buildHouseText;
    sf::RectangleShape buildSupermarketButton;
    sf::Text buildSupermarketText;

    // Курсоры для строительства
    sf::RectangleShape cursorHouse;
    sf::CircleShape cursorSupermarket;

    // Игроки и состояние игры
    std::vector<Player> players;
    int currentPlayerIndex;
    int totalMonths;
    int currentMonth;
    bool gameStarted;

    // Режимы строительства
    bool buildingHouseMode;
    bool buildingSupermarketMode;

    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> distX;
    std::uniform_real_distribution<> distY;

public:
    Game(int numPlayers, const std::vector<std::string>& playerNames, int months);
    void run();

private:
    void setupUI();
    void handleEvents();
    void update();
    void render();
    void endTurn();
    void endGame();

    // Вспомогательные методы
    bool isPositionFree(sf::Vector2f position, float size);
    sf::Vector2f findFreePosition(float size);
    void updateBuildingStats();  // Новый метод для обновления статистики
};

#endif // GAME_H