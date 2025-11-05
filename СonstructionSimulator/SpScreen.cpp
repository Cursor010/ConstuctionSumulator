#include "SpScreen.h"
#include "Game.h"
#include <sstream>

SetupScreen::SetupScreen() : currentInputIndex(0), numPlayers(2) {
    desktopMode = sf::VideoMode::getDesktopMode();
    window.create(desktopMode, "Game Setup", sf::Style::Fullscreen);

    if (!font.loadFromFile("arial.ttf")) {
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    setupUI();
}

void SetupScreen::setupUI() {
    int titleFontSize = desktopMode.height / 20;
    int baseFontSize = desktopMode.height / 40;

    titleText.setFont(font);
    titleText.setString("Investment Game Setup");
    titleText.setCharacterSize(titleFontSize);
    titleText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(desktopMode.width / 2 - titleBounds.width / 2, desktopMode.height / 10);

    instructionText.setFont(font);
    instructionText.setString("Use TAB/Arrow keys to navigate, ENTER to confirm");
    instructionText.setCharacterSize(baseFontSize);
    instructionText.setFillColor(sf::Color::White);
    sf::FloatRect instructionBounds = instructionText.getLocalBounds();
    instructionText.setPosition(desktopMode.width / 2 - instructionBounds.width / 2, desktopMode.height / 6);

    // Инициализируем значения по умолчанию
    playerNames.clear();
    for (int i = 0; i < numPlayers; i++) {
        playerNames.push_back("Player" + std::to_string(i + 1));
    }
    monthsValue = "12";

    // Создаем поле для количества игроков
    createPlayerInputs();

    startButton.setSize(sf::Vector2f(250, 70));
    startButton.setPosition(desktopMode.width / 2 - 122, desktopMode.height - 100);
    startButton.setFillColor(sf::Color::Green);

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(baseFontSize);
    startText.setFillColor(sf::Color::White);
    sf::FloatRect startTextBounds = startText.getLocalBounds();
    startText.setPosition(
        desktopMode.width / 2 - startTextBounds.width / 2,
        desktopMode.height - 100 + (50 - baseFontSize) / 2
    );
}

void SetupScreen::createPlayerInputs() {
    // Очищаем все поля
    inputLabels.clear();
    inputBoxes.clear();
    inputTexts.clear();
    inputValues.clear();

    float centerX = desktopMode.width / 2;
    float startY = desktopMode.height / 3;
    int stepY = 60;
    int baseFontSize = desktopMode.height / 40;

    // Создаем поле для количества игроков
    sf::Text playerCountLabel;
    playerCountLabel.setFont(font);
    playerCountLabel.setString("Players number:");
    playerCountLabel.setCharacterSize(baseFontSize);
    playerCountLabel.setFillColor(sf::Color::White);
    playerCountLabel.setPosition(centerX - 250, startY);
    inputLabels.push_back(playerCountLabel);

    sf::RectangleShape playerCountBox(sf::Vector2f(150, 50));
    playerCountBox.setPosition(centerX + 50, startY);
    playerCountBox.setFillColor(sf::Color::Yellow);
    playerCountBox.setOutlineThickness(2);
    playerCountBox.setOutlineColor(sf::Color::Black);
    inputBoxes.push_back(playerCountBox);

    sf::Text playerCountText;
    playerCountText.setFont(font);
    playerCountText.setString(std::to_string(numPlayers));
    playerCountText.setCharacterSize(baseFontSize);
    playerCountText.setFillColor(sf::Color::Black);
    playerCountText.setPosition(centerX + 60, startY + 4);
    inputTexts.push_back(playerCountText);

    inputValues.push_back(std::to_string(numPlayers));

    // Создаем поля для имен игроков
    for (int i = 0; i < numPlayers; i++) {
        sf::Text playerLabel;
        playerLabel.setFont(font);
        playerLabel.setString("Player " + std::to_string(i + 1) + " Name:");
        playerLabel.setCharacterSize(baseFontSize);
        playerLabel.setFillColor(sf::Color::White);
        playerLabel.setPosition(centerX - 250, startY + (i + 1) * stepY);
        inputLabels.push_back(playerLabel);

        sf::RectangleShape playerBox(sf::Vector2f(300, 50));
        playerBox.setPosition(centerX + 50, startY + (i + 1) * stepY);
        playerBox.setFillColor(sf::Color::White);
        playerBox.setOutlineThickness(2);
        playerBox.setOutlineColor(sf::Color::Black);
        inputBoxes.push_back(playerBox);

        sf::Text playerText;
        playerText.setFont(font);

        // Используем сохраненное имя игрока
        std::string playerName;
        if (i < playerNames.size()) {
            playerName = playerNames[i];
        }
        else {
            playerName = "Player" + std::to_string(i + 1);
            playerNames.push_back(playerName);
        }

        playerText.setString(playerName);
        playerText.setCharacterSize(baseFontSize);
        playerText.setFillColor(sf::Color::Black);
        playerText.setPosition(centerX + 60, startY + (i + 1) * stepY + 4);
        inputTexts.push_back(playerText);

        inputValues.push_back(playerName);
    }

    // Создаем поле для количества месяцев
    sf::Text monthsLabel;
    monthsLabel.setFont(font);
    monthsLabel.setString("Game Duration:");
    monthsLabel.setCharacterSize(baseFontSize);
    monthsLabel.setFillColor(sf::Color::White);
    monthsLabel.setPosition(centerX - 250, startY + (numPlayers + 1) * stepY);
    inputLabels.push_back(monthsLabel);

    sf::RectangleShape monthsBox(sf::Vector2f(150, 50));
    monthsBox.setPosition(centerX + 50, startY + (numPlayers + 1) * stepY);
    monthsBox.setFillColor(sf::Color::White);
    monthsBox.setOutlineThickness(2);
    monthsBox.setOutlineColor(sf::Color::Black);
    inputBoxes.push_back(monthsBox);

    sf::Text monthsText;
    monthsText.setFont(font);
    monthsText.setString(monthsValue);
    monthsText.setCharacterSize(baseFontSize);
    monthsText.setFillColor(sf::Color::Black);
    monthsText.setPosition(centerX + 60, startY + (numPlayers + 1) * stepY + 4);
    inputTexts.push_back(monthsText);

    inputValues.push_back(monthsValue);

    updateFieldColors();
}

void SetupScreen::updateFieldColors() {
    for (size_t i = 0; i < inputBoxes.size(); i++) {
        if (i == currentInputIndex) {
            inputBoxes[i].setFillColor(sf::Color::Yellow);
            inputBoxes[i].setOutlineThickness(3);
            inputBoxes[i].setOutlineColor(sf::Color::Red);
        }
        else {
            inputBoxes[i].setFillColor(sf::Color::White);
            inputBoxes[i].setOutlineThickness(2);
            inputBoxes[i].setOutlineColor(sf::Color::Black);
        }
    }
}

void SetupScreen::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void SetupScreen::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Tab || event.key.code == sf::Keyboard::Down) {
                currentInputIndex = (currentInputIndex + 1) % inputBoxes.size();
                updateFieldColors();
            }
            else if (event.key.code == sf::Keyboard::Up) {
                currentInputIndex = (currentInputIndex - 1 + inputBoxes.size()) % inputBoxes.size();
                updateFieldColors();
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                if (currentInputIndex == inputBoxes.size() - 1) {
                    startGame();
                }
                else {
                    currentInputIndex = (currentInputIndex + 1) % inputBoxes.size();
                    updateFieldColors();
                }
            }
            else if (event.key.code == sf::Keyboard::BackSpace) {
                if (!inputValues[currentInputIndex].empty()) {
                    inputValues[currentInputIndex].pop_back();
                    inputTexts[currentInputIndex].setString(inputValues[currentInputIndex]);

                    // Обновляем соответствующие данные
                    if (currentInputIndex == 0) {
                        // Количество игроков
                        try {
                            int newNum = std::stoi(inputValues[0]);
                            if (newNum >= 2 && newNum <= 5) {
                                numPlayers = newNum;
                            }
                        }
                        catch (...) {}
                    }
                    else if (currentInputIndex == inputBoxes.size() - 1) {
                        // Месяцы
                        monthsValue = inputValues[currentInputIndex];
                    }
                    else {
                        // Имена игроков
                        int playerIndex = currentInputIndex - 1;
                        if (playerIndex >= 0 && playerIndex < playerNames.size()) {
                            playerNames[playerIndex] = inputValues[currentInputIndex];
                        }
                    }
                }
            }
            // Обработка изменения количества игроков с помощью +/-
            else if (event.key.code == sf::Keyboard::Add || event.key.code == sf::Keyboard::Equal) {
                if (currentInputIndex == 0 && numPlayers < 5) {
                    numPlayers++;
                    inputValues[0] = std::to_string(numPlayers);
                    inputTexts[0].setString(inputValues[0]);

                    // Добавляем нового игрока с именем по умолчанию
                    if (playerNames.size() < numPlayers) {
                        playerNames.push_back("Player" + std::to_string(numPlayers));
                    }

                    createPlayerInputs();
                }
                // Обработка увеличения количества месяцев с помощью +
                else if (currentInputIndex == inputBoxes.size() - 1) {
                    int months = getMonthsValue();
                    if (months < 24) {
                        months++;
                        monthsValue = std::to_string(months);
                        inputValues[currentInputIndex] = monthsValue;
                        inputTexts[currentInputIndex].setString(monthsValue);
                    }
                }
            }
            else if (event.key.code == sf::Keyboard::Subtract || event.key.code == sf::Keyboard::Dash) {
                if (currentInputIndex == 0 && numPlayers > 2) {
                    numPlayers--;
                    inputValues[0] = std::to_string(numPlayers);
                    inputTexts[0].setString(inputValues[0]);

                    // Удаляем лишнего игрока
                    if (playerNames.size() > numPlayers) {
                        playerNames.pop_back();
                    }

                    createPlayerInputs();
                }
                // Обработка уменьшения количества месяцев с помощью -
                else if (currentInputIndex == inputBoxes.size() - 1) {
                    int months = getMonthsValue();
                    if (months > 1) {
                        months--;
                        monthsValue = std::to_string(months);
                        inputValues[currentInputIndex] = monthsValue;
                        inputTexts[currentInputIndex].setString(monthsValue);
                    }
                }
            }
        }

        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13 && event.text.unicode != 9) {
                char enteredChar = static_cast<char>(event.text.unicode);

                // Поле количества игроков
                if (currentInputIndex == 0) {
                    if (enteredChar >= '2' && enteredChar <= '5') {
                        numPlayers = enteredChar - '0';
                        inputValues[0] = enteredChar;
                        inputTexts[0].setString(inputValues[0]);

                        // Обновляем список имен игроков
                        while (playerNames.size() > numPlayers) {
                            playerNames.pop_back();
                        }
                        while (playerNames.size() < numPlayers) {
                            playerNames.push_back("Player" + std::to_string(playerNames.size() + 1));
                        }

                        createPlayerInputs();
                    }
                }
                // Поле месяцев
                else if (currentInputIndex == inputBoxes.size() - 1) {
                    if (enteredChar >= '0' && enteredChar <= '9') {
                        std::string newValue = inputValues[currentInputIndex] + enteredChar;
                        if (newValue.length() <= 2) {
                            int months = std::stoi(newValue);
                            if (months >= 1 && months <= 24) {
                                monthsValue = newValue;
                                inputValues[currentInputIndex] = monthsValue;
                                inputTexts[currentInputIndex].setString(monthsValue);
                            }
                        }
                    }
                }
                // Поля имен игроков
                else {
                    inputValues[currentInputIndex] += enteredChar;
                    inputTexts[currentInputIndex].setString(inputValues[currentInputIndex]);

                    // Обновляем соответствующее имя игрока
                    int playerIndex = currentInputIndex - 1;
                    if (playerIndex >= 0 && playerIndex < playerNames.size()) {
                        playerNames[playerIndex] = inputValues[currentInputIndex];
                    }
                }
            }
        }
    }
}

void SetupScreen::render() {
    window.clear(sf::Color(30, 30, 120));

    window.draw(titleText);
    window.draw(instructionText);

    // Отрисовываем все элементы интерфейса
    for (size_t i = 0; i < inputLabels.size(); i++) {
        window.draw(inputLabels[i]);
        window.draw(inputBoxes[i]);
        window.draw(inputTexts[i]);
    }

    window.draw(startButton);
    window.draw(startText);

    // Подсказки управления
    int helpFontSize = desktopMode.height / 50;
    sf::Text helpText1;
    helpText1.setFont(font);
    helpText1.setString("TAB/DOWN - Next field, UP - Previous field, ENTER - Confirm/Start");
    helpText1.setCharacterSize(helpFontSize);
    helpText1.setFillColor(sf::Color::White);
    sf::FloatRect helpBounds1 = helpText1.getLocalBounds();
    helpText1.setPosition(desktopMode.width / 2 - helpBounds1.width / 2, desktopMode.height - 180);
    window.draw(helpText1);

    sf::Text helpText2;
    helpText2.setFont(font);
    helpText2.setString("+/- to change values, ESC - Exit");
    helpText2.setCharacterSize(helpFontSize);
    helpText2.setFillColor(sf::Color::White);
    sf::FloatRect helpBounds2 = helpText2.getLocalBounds();
    helpText2.setPosition(desktopMode.width / 2 - helpBounds2.width / 2, desktopMode.height - 150);
    window.draw(helpText2);

    // Отображение текущего выбора
    if (currentInputIndex == 0) {
        sf::Text playerHelp;
        playerHelp.setFont(font);
        playerHelp.setString("Use +/- to change player count or type 2-5");
        playerHelp.setCharacterSize(helpFontSize);
        playerHelp.setFillColor(sf::Color::Yellow);
        playerHelp.setPosition(desktopMode.width / 2 - 300, desktopMode.height / 4);
        window.draw(playerHelp);
    }
    else if (currentInputIndex == inputBoxes.size() - 1) {
        sf::Text monthsHelp;
        monthsHelp.setFont(font);
        monthsHelp.setString("Use +/- to change months (1-24) or type number");
        monthsHelp.setCharacterSize(helpFontSize);
        monthsHelp.setFillColor(sf::Color::Yellow);
        monthsHelp.setPosition(desktopMode.width / 2 - 300, desktopMode.height / 4);
        window.draw(monthsHelp);
    }

    window.display();
}

int SetupScreen::getMonthsValue() {
    try {
        return std::stoi(monthsValue);
    }
    catch (...) {
        return 12; // Значение по умолчанию при ошибке
    }
}

void SetupScreen::startGame() {
    int months = getMonthsValue();

    // Гарантируем, что месяцы в диапазоне 1-24
    if (months < 1) months = 1;
    if (months > 24) months = 24;

    // Используем сохраненные имена игроков
    std::vector<std::string> finalPlayerNames;
    for (int i = 0; i < numPlayers; i++) {
        if (i < playerNames.size() && !playerNames[i].empty()) {
            finalPlayerNames.push_back(playerNames[i]);
        }
        else {
            finalPlayerNames.push_back("Player" + std::to_string(i + 1));
        }
    }

    window.close();
    Game game(numPlayers, finalPlayerNames, months);
    game.run();
}