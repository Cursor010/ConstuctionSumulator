#include "realestateagency.h"
#include "gameconfig.h"
#include <algorithm>
#include <QDebug>

QList<RealEstateAgency::HousingOffer> RealEstateAgency::collectAllOffers(const QList<Player*>& allPlayers) {
    QList<HousingOffer> allOffers;

    for (int i = 0; i < allPlayers.size(); ++i) {
        Player* player = allPlayers[i];
        QList<Building*> buildings = player->getBuildings();
        for (int j = 0; j < buildings.size(); ++j) {
            Building* building = buildings[j];
            // Предлагаем жилье из построенных домов
            if ((building->getType() == Building::HOUSE_CONCRETE ||
                 building->getType() == Building::HOUSE_WOOD ||
                 building->getType() == Building::HOUSE_BRICK) &&
                building->getIsCompleted()) {

                // Доступная площадь для продажи (все непроданное)
                double availableArea = building->getTotalArea() - building->getSoldArea();
                if (availableArea > 0) {
                    HousingOffer offer;
                    offer.player = player;
                    offer.building = building;
                    offer.pricePerSqm = building->getPricePerSqm();
                    offer.availableArea = availableArea;
                    offer.cellIndex = building->getCellIndex();
                    allOffers.append(offer);
                }
            }
        }
    }

    return allOffers;
}

void RealEstateAgency::sortOffersByPrice(QList<HousingOffer>& offers) {
    std::sort(offers.begin(), offers.end(),
              [](const HousingOffer& a, const HousingOffer& b) {
                  return a.pricePerSqm < b.pricePerSqm;
              });
}

void RealEstateAgency::satisfyDemand(double totalDemand, QList<HousingOffer>& offers, const QList<Player*>& allPlayers) {
    double remainingDemand = totalDemand;

    for (int i = 0; i < offers.size(); ++i) {
        HousingOffer& offer = offers[i];
        if (remainingDemand <= 0) break;

        // Учитываем соседние магазины для увеличения спроса на этот дом
        int neighborMarkets = offer.player->countNeighborMarkets(offer.cellIndex, allPlayers);
        double neighborBonus = 1.0;
        if (neighborMarkets > 0) {
            neighborBonus = 1.0 + (neighborMarkets * GameConfig::HOUSE_NEIGHBOR_BONUS);
        }

        double effectiveDemand = remainingDemand * neighborBonus;

        // Продаем столько, сколько можем из этого предложения
        double canSell = qMin(offer.availableArea, effectiveDemand);
        double revenue = canSell * offer.pricePerSqm;

        // Обновляем состояние
        offer.building->setSoldArea(offer.building->getSoldArea() + canSell);
        offer.player->addMoney(revenue);
        offer.building->addToMonthlyProfit(revenue);

        remainingDemand -= canSell / neighborBonus; // Уменьшаем общий спрос

        qDebug() << "Sold" << canSell << "sqm from cell" << offer.cellIndex
                 << "for" << revenue << "at price" << offer.pricePerSqm
                 << "(neighbor markets:" << neighborMarkets << ")";
    }

    if (remainingDemand > 0) {
        qDebug() << "Unsatisfied demand:" << remainingDemand << "sqm";
    }
}

void RealEstateAgency::processHousingSales(double totalDemand, const QList<Player*>& allPlayers) {
    if (totalDemand <= 0) return;

    QList<HousingOffer> offers = collectAllOffers(allPlayers);

    if (offers.isEmpty()) {
        qDebug() << "No housing offers available";
        return;
    }

    sortOffersByPrice(offers);
    satisfyDemand(totalDemand, offers, allPlayers);
}
