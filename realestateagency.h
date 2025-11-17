#ifndef REALESTATEAGENCY_H
#define REALESTATEAGENCY_H

#include <QList>
#include "player.h"
#include "building.h"

class RealEstateAgency
{
public:
    struct HousingOffer {
        Player* player;
        Building* building;
        double pricePerSqm;
        double availableArea;
        int cellIndex;
    };

    static void processHousingSales(double totalDemand, const QList<Player*>& allPlayers);

private:
    static QList<HousingOffer> collectAllOffers(const QList<Player*>& allPlayers);
    static void sortOffersByPrice(QList<HousingOffer>& offers);
    static void satisfyDemand(double totalDemand, QList<HousingOffer>& offers, const QList<Player*>& allPlayers); // ИСПРАВЛЕНО: добавлен allPlayers
};

#endif // REALESTATEAGENCY_H
