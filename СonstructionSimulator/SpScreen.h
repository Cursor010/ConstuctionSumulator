#ifndef SPSCREEN_H
#define SPSCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class SetupScreen {
private:
    sf::RenderWindow window;
    sf::VideoMode desktopMode;
    sf::Font font;

    // Текстовые элементы
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text startText;

    // Элементы ввода
    std::vector<sf::Text> inputLabels;
    std::vector<sf::RectangleShape> inputBoxes;
    std::vector<sf::Text> inputTexts;
    std::vector<std::string> inputValues;

    // Кнопка старта
    sf::RectangleShape startButton;

    // Данные формы
    int currentInputIndex;
    int numPlayers;
    std::vector<std::string> playerNames;  // Добавлено: отдельное хранилище для имен игроков
    std::string monthsValue;               // Добавлено: отдельное хранилище для количества месяцев

public:
    SetupScreen();
    void run();

private:
    void setupUI();
    void createPlayerInputs();
    void updateFieldColors();
    void handleEvents();
    void render();
    void startGame();

    // Добавленные методы
    int getMonthsValue();
    void validateAndUpdateMonths();
};

#endif // SPSCREEN_H