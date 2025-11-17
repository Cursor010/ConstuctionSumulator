#include "gameconfig.h"

// Параметры домов
const double GameConfig::CONCRETE_HOUSE_AREA = 6000.0;
const double GameConfig::WOOD_HOUSE_AREA = 4000.0;
const double GameConfig::BRICK_HOUSE_AREA = 5000.0;

const double GameConfig::CONCRETE_HOUSE_BASE_PRICE = 12000.0;
const double GameConfig::WOOD_HOUSE_BASE_PRICE = 8000.0;
const double GameConfig::BRICK_HOUSE_BASE_PRICE = 10000.0;

// Базовые доходы
const double GameConfig::BASE_HOUSE_DEMAND = 1000.0;
const double GameConfig::BASE_MARKET_REVENUE = 15000.0;

// Коэффициенты влияния соседей
const double GameConfig::MARKET_NEIGHBOR_BONUS = 0.3; // +30% за каждый соседний дом
const double GameConfig::HOUSE_NEIGHBOR_BONUS = 0.2;  // +20% за каждый соседний магазин

// Коэффициенты сезонности
const double GameConfig::SPRING_HOUSE_MODIFIER = 1.2;
const double GameConfig::SUMMER_HOUSE_MODIFIER = 1.1;
const double GameConfig::AUTUMN_HOUSE_MODIFIER = 1.5;
const double GameConfig::WINTER_HOUSE_MODIFIER = 0.8;

const double GameConfig::SPRING_MARKET_MODIFIER = 1.0;
const double GameConfig::SUMMER_MARKET_MODIFIER = 1.1;
const double GameConfig::AUTUMN_MARKET_MODIFIER = 1.3;
const double GameConfig::WINTER_MARKET_MODIFIER = 1.6;
