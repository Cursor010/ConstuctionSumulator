#include "gameconfig.h"

// Параметры домов
const double GameConfig::CONCRETE_HOUSE_AREA = 10000.0;     // 10,000 кв.м.
const double GameConfig::WOOD_HOUSE_AREA = 8000.0;          // 8,000 кв.м.
const double GameConfig::BRICK_HOUSE_AREA = 9000.0;         // 9,000 кв.м.

// Базовые цены за кв.м. (в у.е.)
const double GameConfig::CONCRETE_HOUSE_BASE_PRICE = 1500.0;
const double GameConfig::WOOD_HOUSE_BASE_PRICE = 1200.0;
const double GameConfig::BRICK_HOUSE_BASE_PRICE = 1400.0;

// Базовые показатели
const double GameConfig::BASE_HOUSE_DEMAND = 50000.0;       // 50,000 кв.м. в месяц
const double GameConfig::BASE_MARKET_REVENUE = 500000.0;    // 500,000 у.е. в месяц

// Коэффициенты влияния соседей
const double GameConfig::MARKET_NEIGHBOR_BONUS = 0.15;      // +15% за каждый соседний дом
const double GameConfig::HOUSE_NEIGHBOR_BONUS = 0.10;       // +10% за каждый соседний магазин

// Коэффициенты сезонности для жилья (спрос возрастает с весны, максимален осенью)
const double GameConfig::SPRING_HOUSE_MODIFIER = 1.1;       // +10% весной
const double GameConfig::SUMMER_HOUSE_MODIFIER = 1.3;       // +30% летом
const double GameConfig::AUTUMN_HOUSE_MODIFIER = 1.5;       // +50% осенью
const double GameConfig::WINTER_HOUSE_MODIFIER = 0.8;       // -20% зимой

// Коэффициенты сезонности для магазинов (прибыль возрастает с осени, максимальна зимой)
const double GameConfig::SPRING_MARKET_MODIFIER = 1.0;      // без изменений весной
const double GameConfig::SUMMER_MARKET_MODIFIER = 1.1;      // +10% летом
const double GameConfig::AUTUMN_MARKET_MODIFIER = 1.2;      // +20% осенью
const double GameConfig::WINTER_MARKET_MODIFIER = 1.4;      // +40% зимой

// Дополнительные параметры для продажи квартир
const double GameConfig::BASE_HOUSE_SALES_RATE = 0.15;      // 15% площади в месяц (увеличили)
const double GameConfig::PRICE_SENSITIVITY = 0.001;         // чувствительность к цене
