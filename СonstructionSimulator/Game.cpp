#include "Game.h"
#include <sstream>
#include <iostream>

Game::Game(int numPlayers, const std::vector<std::string>& playerNames, int months)
    : currentPlayerIndex(0), totalMonths(months), currentMonth(0), gameStarted(true),
    buildingHouseMode(false), buildingSupermarketMode(false), gen(rd()) {

    desktopMode = sf::VideoMode::getDesktopMode();
    window.create(desktopMode, "Investment Game", sf::Style::Fullscreen);

    distX = std::uniform_real_distribution<>(50, desktopMode.width - 100);
    distY = std::uniform_real_distribution<>(100, desktopMode.height - 100);

    std::vector<sf::Color> colors = {
        sf::Color::Red, sf::Color::Blue, sf::Color::Green,
        sf::Color::Yellow, sf::Color::Magenta
    };

    for (int i = 0; i < numPlayers; i++) {
        players.emplace_back(playerNames[i], colors[i], 37.0, true);
    }

    if (!font.loadFromFile("arial.ttf")) {
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
    }

    setupUI();
}

void Game::setupUI() {
    int baseFontSize = desktopMode.height / 40;
    int titleFontSize = desktopMode.height / 25;

    infoText.setFont(font);
    infoText.setCharacterSize(baseFontSize);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(20, 20);

    playerText.setFont(font);
    playerText.setCharacterSize(titleFontSize);
    playerText.setStyle(sf::Text::Bold);
    playerText.setPosition(desktopMode.width / 2, 20);

    // Удаляем старый buildingInfoText и создаем вектор для статистики
    buildingStats.clear();

    actionText.setFont(font);
    actionText.setCharacterSize(baseFontSize);
    actionText.setFillColor(sf::Color::Yellow);
    actionText.setPosition(20, 80);

    int buttonWidth = desktopMode.width / 8;
    int buttonHeight = desktopMode.height / 20;

    endTurnButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    endTurnButton.setPosition(desktopMode.width - buttonWidth - 20, desktopMode.height - buttonHeight - 20);
    endTurnButton.setFillColor(sf::Color::Blue);

    endTurnText.setFont(font);
    endTurnText.setString("End Turn");
    endTurnText.setCharacterSize(baseFontSize);
    endTurnText.setFillColor(sf::Color::White);
    sf::FloatRect endTurnTextBounds = endTurnText.getLocalBounds();
    endTurnText.setPosition(
        endTurnButton.getPosition().x + (buttonWidth - endTurnTextBounds.width) / 2,
        endTurnButton.getPosition().y + (buttonHeight - baseFontSize) / 2
    );

    buildHouseButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    buildHouseButton.setPosition(desktopMode.width - buttonWidth - 20, desktopMode.height - buttonHeight * 3 - 20);
    buildHouseButton.setFillColor(sf::Color::Red);

    buildHouseText.setFont(font);
    buildHouseText.setString("Build House (8M)");
    buildHouseText.setCharacterSize(baseFontSize);
    buildHouseText.setFillColor(sf::Color::White);
    sf::FloatRect buildHouseTextBounds = buildHouseText.getLocalBounds();
    buildHouseText.setPosition(
        buildHouseButton.getPosition().x + (buttonWidth - buildHouseTextBounds.width) / 2,
        buildHouseButton.getPosition().y + (buttonHeight - baseFontSize) / 2
    );

    buildSupermarketButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    buildSupermarketButton.setPosition(desktopMode.width - buttonWidth - 20, desktopMode.height - buttonHeight * 2 - 20);
    buildSupermarketButton.setFillColor(sf::Color::Green);

    buildSupermarketText.setFont(font);
    buildSupermarketText.setString("Build Market (2.5M)");
    buildSupermarketText.setCharacterSize(baseFontSize);
    buildSupermarketText.setFillColor(sf::Color::White);
    sf::FloatRect buildSupermarketTextBounds = buildSupermarketText.getLocalBounds();
    buildSupermarketText.setPosition(
        buildSupermarketButton.getPosition().x + (buttonWidth - buildSupermarketTextBounds.width) / 2,
        buildSupermarketButton.getPosition().y + (buttonHeight - baseFontSize) / 2
    );

    cursorHouse.setSize(sf::Vector2f(40, 40));
    cursorHouse.setFillColor(sf::Color::Transparent);
    cursorHouse.setOutlineThickness(2);
    cursorHouse.setOutlineColor(sf::Color::White);

    cursorSupermarket.setRadius(20);
    cursorSupermarket.setFillColor(sf::Color::Transparent);
    cursorSupermarket.setOutlineThickness(2);
    cursorSupermarket.setOutlineColor(sf::Color::White);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

bool Game::isPositionFree(sf::Vector2f position, float size) {
    sf::FloatRect newBuildingBounds(position.x, position.y, size, size);

    for (const auto& player : players) {
        for (const auto& building : player.buildings) {
            if (building->getBounds().intersects(newBuildingBounds)) {
                return false;
            }
        }
    }
    return true;
}

sf::Vector2f Game::findFreePosition(float size) {
    for (int attempt = 0; attempt < 100; attempt++) {
        float x = static_cast<float>(distX(gen));
        float y = static_cast<float>(distY(gen));

        if (isPositionFree(sf::Vector2f(x, y), size)) {
            return sf::Vector2f(x, y);
        }
    }
    return sf::Vector2f(-100, -100);
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }

        Player& currentPlayer = players[currentPlayerIndex];

        if (event.type == sf::Event::MouseMoved) {
            if (buildingHouseMode || buildingSupermarketMode) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x),
                    static_cast<float>(event.mouseMove.y));

                if (buildingHouseMode) {
                    cursorHouse.setPosition(mousePos.x - 20, mousePos.y - 20);
                }
                else if (buildingSupermarketMode) {
                    cursorSupermarket.setPosition(mousePos.x - 20, mousePos.y - 20);
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y));

            if (endTurnButton.getGlobalBounds().contains(mousePos)) {
                endTurn();
            }
            else if (buildHouseButton.getGlobalBounds().contains(mousePos) && currentPlayer.canBuildHouse()) {
                buildingHouseMode = true;
                buildingSupermarketMode = false;
            }
            else if (buildSupermarketButton.getGlobalBounds().contains(mousePos) && currentPlayer.canBuildSupermarket()) {
                buildingSupermarketMode = true;
                buildingHouseMode = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Left) {
                if (buildingHouseMode && currentPlayer.canBuildHouse()) {
                    if (isPositionFree(mousePos, 40)) {
                        currentPlayer.buildHouse(mousePos);
                        buildingHouseMode = false;
                    }
                }
                else if (buildingSupermarketMode && currentPlayer.canBuildSupermarket()) {
                    if (isPositionFree(mousePos, 40)) {
                        currentPlayer.buildSupermarket(mousePos);
                        buildingSupermarketMode = false;
                    }
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                buildingHouseMode = false;
                buildingSupermarketMode = false;
            }
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                buildingHouseMode = false;
                buildingSupermarketMode = false;
            }
            else if (event.key.code == sf::Keyboard::Space) {
                if (buildingHouseMode && currentPlayer.canBuildHouse()) {
                    sf::Vector2f freePos = findFreePosition(40);
                    if (freePos.x >= 0) {
                        currentPlayer.buildHouse(freePos);
                        buildingHouseMode = false;
                    }
                }
                else if (buildingSupermarketMode && currentPlayer.canBuildSupermarket()) {
                    sf::Vector2f freePos = findFreePosition(40);
                    if (freePos.x >= 0) {
                        currentPlayer.buildSupermarket(freePos);
                        buildingSupermarketMode = false;
                    }
                }
            }
        }
    }
}

void Game::update() {
    if (!gameStarted) return;

    Player& currentPlayer = players[currentPlayerIndex];

    std::stringstream info;
    info << "Month: " << currentMonth << "/" << totalMonths << "\n";
    info << "Players:\n";
    for (size_t i = 0; i < players.size(); i++) {
        info << (i == currentPlayerIndex ? "> " : "  ")
            << players[i].name << ": " << players[i].money << "M\n";
    }
    infoText.setString(info.str());

    std::stringstream playerInfo;
    playerInfo << "Current: " << currentPlayer.name << " (" << currentPlayer.money << "M)";
    playerText.setString(playerInfo.str());
    playerText.setFillColor(currentPlayer.color);

    sf::FloatRect textBounds = playerText.getLocalBounds();
    playerText.setOrigin(textBounds.left + textBounds.width / 2.0f, 0);
    playerText.setPosition(desktopMode.width / 2.0f, 20);

    // Обновляем статистику зданий с цветными именами игроков
    updateBuildingStats();

    buildHouseButton.setFillColor(currentPlayer.canBuildHouse() ? sf::Color::Red : sf::Color(128, 128, 128));
    buildSupermarketButton.setFillColor(currentPlayer.canBuildSupermarket() ? sf::Color::Green : sf::Color(128, 128, 128));
}

void Game::updateBuildingStats() {
    buildingStats.clear();

    int baseFontSize = desktopMode.height / 40;
    float startX = desktopMode.width - 350;
    float startY = 20;
    float lineHeight = 35; // Увеличили отступ между строками
    float sectionSpacing = 15; // Добавили дополнительный отступ между разделами

    // Заголовок статистики
    sf::Text header;
    header.setFont(font);
    header.setString("Buildings Statistics:");
    header.setCharacterSize(baseFontSize);
    header.setFillColor(sf::Color::White);
    header.setPosition(startX, startY);
    buildingStats.push_back(header);

    float currentY = startY + lineHeight * 1.5; // Увеличили отступ после заголовка

    for (const auto& player : players) {
        // Имя игрока его цветом
        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(player.name + ":");
        nameText.setCharacterSize(baseFontSize);
        nameText.setFillColor(player.color);
        nameText.setStyle(sf::Text::Bold); // Сделали имя жирным
        nameText.setPosition(startX, currentY);
        buildingStats.push_back(nameText);

        currentY += lineHeight;

        // Считаем дома и магазины
        int houseCount = 0, marketCount = 0;
        for (const auto& building : player.buildings) {
            if (building->type == "House") houseCount++;
            else if (building->type == "Supermarket") marketCount++;
        }

        // Статистика домов
        sf::Text houseText;
        houseText.setFont(font);
        houseText.setString("   Houses: " + std::to_string(houseCount));
        houseText.setCharacterSize(baseFontSize);
        houseText.setFillColor(sf::Color(200, 200, 200)); // Светло-серый цвет для текста
        houseText.setPosition(startX + 20, currentY);
        buildingStats.push_back(houseText);

        currentY += lineHeight;

        // Статистика магазинов
        sf::Text marketText;
        marketText.setFont(font);
        marketText.setString("   Markets: " + std::to_string(marketCount));
        marketText.setCharacterSize(baseFontSize);
        marketText.setFillColor(sf::Color(200, 200, 200)); // Светло-серый цвет для текста
        marketText.setPosition(startX + 20, currentY);
        buildingStats.push_back(marketText);

        currentY += lineHeight;

        // Общая стоимость построек
        float totalBuildingsValue = houseCount * 8.0f + marketCount * 2.5f;
        sf::Text valueText;
        valueText.setFont(font);
        valueText.setString("   Total value: " + std::to_string(static_cast<int>(totalBuildingsValue)) + "M");
        valueText.setCharacterSize(baseFontSize - 2);
        valueText.setFillColor(sf::Color(180, 180, 180)); // Более темный серый
        valueText.setPosition(startX + 20, currentY);
        buildingStats.push_back(valueText);

        currentY += lineHeight;

        // Увеличили отступ между игроками
        currentY += sectionSpacing;
    }
}

void Game::render() {
    window.clear(sf::Color::Green);

    for (auto& player : players) {
        player.draw(window);
    }

    if (buildingHouseMode) {
        window.draw(cursorHouse);
    }
    else if (buildingSupermarketMode) {
        window.draw(cursorSupermarket);
    }

    window.draw(infoText);
    window.draw(playerText);
    window.draw(actionText);

    // Отрисовываем статистику зданий
    for (const auto& statText : buildingStats) {
        window.draw(statText);
    }

    window.draw(endTurnButton);
    window.draw(endTurnText);
    window.draw(buildHouseButton);
    window.draw(buildHouseText);
    window.draw(buildSupermarketButton);
    window.draw(buildSupermarketText);

    window.display();
}

void Game::endTurn() {
    Player& currentPlayer = players[currentPlayerIndex];
    currentPlayer.updateMonthly();
    currentPlayer.resetTurn();

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

    buildingHouseMode = false;
    buildingSupermarketMode = false;

    if (currentPlayerIndex == 0) {
        currentMonth++;

        if (currentMonth >= totalMonths) {
            endGame();
        }
    }
}

void Game::endGame() {
    gameStarted = false;

    // Находим победителя
    Player* winner = &players[0];
    for (auto& player : players) {
        if (player.money > winner->money) {
            winner = &player;
        }
    }

    // Создаем праздничный экран окончания игры
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("RESULTS!");
    gameOverText.setCharacterSize(desktopMode.height / 10);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    sf::FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gameOverBounds.left + gameOverBounds.width / 2.0f,
        gameOverBounds.top + gameOverBounds.height / 2.0f);
    gameOverText.setPosition(desktopMode.width / 2.0f, desktopMode.height / 6);

    sf::Text winnerText;
    winnerText.setFont(font);
    winnerText.setString("Winner: " + winner->name + " with " + std::to_string(static_cast<int>(winner->money)) + "M");
    winnerText.setCharacterSize(desktopMode.height / 15);
    winnerText.setFillColor(winner->color);
    winnerText.setStyle(sf::Text::Bold);
    sf::FloatRect winnerBounds = winnerText.getLocalBounds();
    winnerText.setOrigin(winnerBounds.left + winnerBounds.width / 2.0f,
        winnerBounds.top + winnerBounds.height / 2.0f);
    winnerText.setPosition(desktopMode.width / 2.0f, desktopMode.height / 3);

    sf::Text resultsText;
    resultsText.setFont(font);
    std::stringstream results;

    results << '\n';
    for (size_t i = 1; i < players.size(); i++) {
        auto& player = players[i];
        results << player.name << ": " << static_cast<int>(player.money) << "M\n";
    }

    resultsText.setString(results.str());
    resultsText.setCharacterSize(desktopMode.height / 20);
    resultsText.setFillColor(sf::Color::White);
    resultsText.setStyle(sf::Text::Bold);
    sf::FloatRect resultsBounds = resultsText.getLocalBounds();
    resultsText.setOrigin(resultsBounds.left + resultsBounds.width / 2.0f,
        resultsBounds.top + resultsBounds.height / 2.0f);
    resultsText.setPosition(desktopMode.width / 2.0f, desktopMode.height / 2);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Press ENTER to restart the game");
    restartText.setCharacterSize(desktopMode.height / 25);
    restartText.setFillColor(sf::Color::Yellow);
    restartText.setStyle(sf::Text::Bold);
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setOrigin(restartBounds.left + restartBounds.width / 2.0f,
        restartBounds.top + restartBounds.height / 2.0f);
    restartText.setPosition(desktopMode.width / 2.0f, desktopMode.height * 3 / 4);

    // Анимированный фон
    sf::Clock animationClock;
    float hue = 0.0f;

    bool restartRequested = false;

    while (window.isOpen() && !restartRequested) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    restartRequested = true;
                }
            }
        }

        // Анимация фона (меняющийся цвет)
        float time = animationClock.getElapsedTime().asSeconds();
        hue = fmod(time * 50.0f, 360.0f);

        // Создаем цвет на основе HSL (оттенок, насыщенность, яркость)
        float saturation = 0.8f;
        float lightness = 0.3f;

        // Конвертируем HSL в RGB
        float c = (1 - fabs(2 * lightness - 1)) * saturation;
        float x = c * (1 - fabs(fmod(hue / 60.0f, 2) - 1));
        float m = lightness - c / 2;

        float r, g, b;
        if (hue < 60) { r = c; g = x; b = 0; }
        else if (hue < 120) { r = x; g = c; b = 0; }
        else if (hue < 180) { r = 0; g = c; b = x; }
        else if (hue < 240) { r = 0; g = x; b = c; }
        else if (hue < 300) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        sf::Color backgroundColor(static_cast<sf::Uint8>((r + m) * 255),
            static_cast<sf::Uint8>((g + m) * 255),
            static_cast<sf::Uint8>((b + m) * 255));

        window.clear(backgroundColor);

        // Добавляем блестящие частицы для праздничного эффекта
        sf::Clock particleClock;
        float particleTime = particleClock.getElapsedTime().asSeconds();

        for (int i = 0; i < 50; i++) {
            sf::CircleShape particle(2 + static_cast<float>(std::sin(particleTime * 3 + i) * 2));
            particle.setFillColor(sf::Color(255, 255, 255, 150 + static_cast<int>(std::sin(particleTime * 5 + i) * 105)));
            float x = static_cast<float>(std::sin(particleTime * 2 + i * 0.5) * desktopMode.width / 2 + desktopMode.width / 2);
            float y = static_cast<float>(std::cos(particleTime * 2 + i * 0.3) * desktopMode.height / 2 + desktopMode.height / 2);
            particle.setPosition(x, y);
            window.draw(particle);
        }

        window.draw(gameOverText);
        window.draw(winnerText);
        window.draw(resultsText);
        window.draw(restartText);

        window.display();
    }

    // Если запрошен перезапуск, закрываем окно и возвращаемся в main
    if (restartRequested) {
        window.close();
    }
}