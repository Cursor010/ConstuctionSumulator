#ifndef GAMECONFIG_H
#define GAMECONFIG_H

class GameConfig {
public:
    // Начальные деньги (в у.е.)
    static const double INITIAL_MONEY; // 2 млн у.е.

    // Стоимость строительства
    static const double CONCRETE_HOUSE_COST;  // 500 тыс. у.е.
    static const double WOOD_HOUSE_COST;      // 300 тыс. у.е.
    static const double BRICK_HOUSE_COST;     // 400 тыс. у.е.
    static const double MARKET_COST;          // 600 тыс. у.е.

    // Время строительства (в месяцах)
    static const int CONCRETE_HOUSE_BUILD_TIME;
    static const int WOOD_HOUSE_BUILD_TIME;
    static const int BRICK_HOUSE_BUILD_TIME;
    static const int MARKET_BUILD_TIME;

    // Параметры домов
    static const double CONCRETE_HOUSE_AREA;     // 10,000 кв.м.
    static const double WOOD_HOUSE_AREA;         // 8,000 кв.м.
    static const double BRICK_HOUSE_AREA;        // 9,000 кв.м.

    // Базовые цены за кв.м. (в у.е.)
    static const double CONCRETE_HOUSE_BASE_PRICE;
    static const double WOOD_HOUSE_BASE_PRICE;
    static const double BRICK_HOUSE_BASE_PRICE;

    // Базовые показатели
    static const double BASE_HOUSE_DEMAND;       // 50,000 кв.м. в месяц
    static const double BASE_MARKET_REVENUE;     // 500,000 у.е. в месяц

    // Коэффициенты влияния соседей
    static const double MARKET_NEIGHBOR_BONUS;   // +15% за каждый соседний дом
    static const double HOUSE_NEIGHBOR_BONUS;    // +10% за каждый соседний магазин

    // Коэффициенты сезонности для жилья
    static const double SPRING_HOUSE_MODIFIER;   // +10% весной
    static const double SUMMER_HOUSE_MODIFIER;   // +30% летом
    static const double AUTUMN_HOUSE_MODIFIER;   // +50% осенью
    static const double WINTER_HOUSE_MODIFIER;   // -20% зимой

    // Коэффициенты сезонности для магазинов
    static const double SPRING_MARKET_MODIFIER;  // без изменений весной
    static const double SUMMER_MARKET_MODIFIER;  // +10% летом
    static const double AUTUMN_MARKET_MODIFIER;  // +20% осенью
    static const double WINTER_MARKET_MODIFIER;  // +40% зимой

    // Дополнительные параметры для продажи квартир
    static const double BASE_HOUSE_SALES_RATE;   // 15% площади в месяц
    static const double PRICE_SENSITIVITY;       // чувствительность к цене
};

#endif // GAMECONFIG_H
