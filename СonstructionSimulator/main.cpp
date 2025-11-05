#include "SpScreen.h"
#include <iostream>

int main() {
    bool restart = true;

    while (restart) {
        SetupScreen setup;
        setup.run();

        // После закрытия SetupScreen, спросим пользователя, хочет ли он перезапустить
        // Это запасной вариант на случай, если игра завершится не через экран окончания
        std::cout << "Do you want to restart the game? (1 for yes, 0 for no): ";
        std::cin >> restart;

        // Очистка буфера ввода
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return 0;
}