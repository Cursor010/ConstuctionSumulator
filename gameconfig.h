#ifndef GAMECONFIG_H
#define GAMECONFIG_H

class GameConfig {
public:
    // Начальные настройки
    static const int INITIAL_MONEY = 150000;
    static const int TOTAL_MONTHS = 24;

    // Стоимость строительства
    static const int CONCRETE_HOUSE_COST = 80000;
    static const int WOOD_HOUSE_COST = 50000;
    static const int BRICK_HOUSE_COST = 70000;
    static const int MARKET_COST = 60000;

    // Время строительства (месяцев)
    static const int CONCRETE_HOUSE_BUILD_TIME = 6;
    static const int WOOD_HOUSE_BUILD_TIME = 4;
    static const int BRICK_HOUSE_BUILD_TIME = 5;
    static const int MARKET_BUILD_TIME = 3;

    // Параметры домов
    static const double CONCRETE_HOUSE_AREA;
    static const double WOOD_HOUSE_AREA;
    static const double BRICK_HOUSE_AREA;

    static const double CONCRETE_HOUSE_BASE_PRICE;
    static const double WOOD_HOUSE_BASE_PRICE;
    static const double BRICK_HOUSE_BASE_PRICE;

    // Базовые доходы
    static const double BASE_HOUSE_DEMAND;
    static const double BASE_MARKET_REVENUE;

    // Коэффициенты влияния соседей
    static const double MARKET_NEIGHBOR_BONUS; // бонус за каждый соседний дом
    static const double HOUSE_NEIGHBOR_BONUS;  // бонус за каждый соседний магазин

    // Коэффициенты сезонности
    static const double SPRING_HOUSE_MODIFIER;
    static const double SUMMER_HOUSE_MODIFIER;
    static const double AUTUMN_HOUSE_MODIFIER;
    static const double WINTER_HOUSE_MODIFIER;

    static const double SPRING_MARKET_MODIFIER;
    static const double SUMMER_MARKET_MODIFIER;
    static const double AUTUMN_MARKET_MODIFIER;
    static const double WINTER_MARKET_MODIFIER;
};

#endif // GAMECONFIG_H
