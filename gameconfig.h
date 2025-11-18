#ifndef GAMECONFIG_H
#define GAMECONFIG_H

class GameConfig {
public:
    // Начальные настройки
    static const int INITIAL_MONEY = 37000000; // 37 млн как в требованиях
    static const int TOTAL_MONTHS = 24;

    // Стоимость строительства (в млн у.е.)
    static const int CONCRETE_HOUSE_COST = 12000000;  // 12 млн
    static const int WOOD_HOUSE_COST = 8000000;       // 8 млн
    static const int BRICK_HOUSE_COST = 10000000;     // 10 млн
    static const int MARKET_COST = 2500000;           // 2.5 млн

    // Время строительства (месяцев)
    static const int CONCRETE_HOUSE_BUILD_TIME = 7;
    static const int WOOD_HOUSE_BUILD_TIME = 5;
    static const int BRICK_HOUSE_BUILD_TIME = 6;
    static const int MARKET_BUILD_TIME = 5;

    // Параметры домов (площадь в кв.м.)
    static const double CONCRETE_HOUSE_AREA;
    static const double WOOD_HOUSE_AREA;
    static const double BRICK_HOUSE_AREA;

    // Базовые цены за кв.м.
    static const double CONCRETE_HOUSE_BASE_PRICE;
    static const double WOOD_HOUSE_BASE_PRICE;
    static const double BRICK_HOUSE_BASE_PRICE;

    // Базовые показатели
    static const double BASE_HOUSE_DEMAND;    // спрос на жилье (кв.м. в месяц)
    static const double BASE_MARKET_REVENUE;  // базовая прибыль магазина

    // Коэффициенты влияния соседей
    static const double MARKET_NEIGHBOR_BONUS; // бонус за каждый соседний дом
    static const double HOUSE_NEIGHBOR_BONUS;  // бонус за каждый соседний магазин

    // Коэффициенты сезонности для жилья (спрос)
    static const double SPRING_HOUSE_MODIFIER;
    static const double SUMMER_HOUSE_MODIFIER;
    static const double AUTUMN_HOUSE_MODIFIER;
    static const double WINTER_HOUSE_MODIFIER;

    // Коэффициенты сезонности для магазинов (прибыль)
    static const double SPRING_MARKET_MODIFIER;
    static const double SUMMER_MARKET_MODIFIER;
    static const double AUTUMN_MARKET_MODIFIER;
    static const double WINTER_MARKET_MODIFIER;

    // Дополнительные параметры для продажи квартир
    static const double BASE_HOUSE_SALES_RATE; // базовая скорость продажи квартир (% от площади в месяц)
    static const double PRICE_SENSITIVITY;     // чувствительность спроса к цене
};

#endif // GAMECONFIG_H
