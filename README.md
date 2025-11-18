# Construction Simulator 🏗️

[![Qt](https://img.shields.io/badge/Qt-6.0%2B-green.svg)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A strategic construction and investment simulation game built with C++ and Qt framework.

## 🎮 About the Game

Construction Simulator is a strategic business simulation where players compete in the real estate market. Build various types of buildings, manage advertising budgets, and make strategic decisions to become the most successful construction tycoon!

### Key Features
- **Multiple Building Types**: Concrete, wood, and brick houses, plus commercial markets
- **Seasonal Economy**: Dynamic demand changes based on seasons
- **Advertising System**: Strategic ad campaigns to boost sales
- **Multiplayer Support**: Play with up to 5 players
- **Visual Game Board**: 5x5 grid with colorful building representations
- **Real-time Statistics**: Track performance and compete with others

## 🏗️ Building Types

| Building Type | Cost | Build Time | Area | Base Price |
|---------------|------|------------|------|------------|
| Concrete House | 12M | 7 months | 10,000 sq.m | 1,500/sqm |
| Wood House | 8M | 5 months | 8,000 sq.m | 1,200/sqm |
| Brick House | 10M | 6 months | 9,000 sq.m | 1,400/sqm |
| Market | 2.5M | 5 months | - | - |

## 🎯 Game Mechanics

### Economic System
- **Initial Capital**: 37 million
- **Game Duration**: 24 months
- **Income Sources**: Apartment sales, market revenues
- **Expenses**: Construction costs, monthly payments, advertising

### Construction Process
1. Select building type and location
2. Pay construction costs over time
3. Complete building starts generating revenue
4. Manage ongoing costs and advertising

### Economic Parameters
- **Base Housing Demand**: 50,000 sq.m/month
- **Base Market Revenue**: 500,000/month
- **Base Sales Rate**: 15% of area per month
- **Price Sensitivity**: 0.001

### Seasonal Effects
| Season | Housing Modifier | Market Modifier |
|--------|------------------|-----------------|
| Spring | +10% (1.1) | 0% (1.0) |
| Summer | +30% (1.3) | +10% (1.1) |
| Autumn | +50% (1.5) | +20% (1.2) |
| Winter | -20% (0.8) | +40% (1.4) |

### Neighborhood Bonuses
- **Houses**: +10% per adjacent market
- **Markets**: +15% per adjacent house

### Sales Factors
- **Neighborhood**: Markets boost nearby house sales
- **Advertising**: Increased budgets improve sales rates
- **Pricing**: Strategic pricing affects demand
- **Seasonality**: Natural demand fluctuations

## 📥 Installation

### Prerequisites
- Qt 6.0 or higher
- C++17 compatible compiler
- CMake 3.16+

### Build Instructions
```bash
git clone https://github.com/yourusername/construction-simulator.git
cd construction-simulator
mkdir build && cd build
cmake ..
make
./ConstructionSimulator
