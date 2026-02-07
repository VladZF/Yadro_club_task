#ifndef YADRO_BASE_STRUCTS_H
#define YADRO_BASE_STRUCTS_H

#include <string>
#include "ClientTime.h"
#include "Constants.h"

struct ClubConfig {
    int numTables = 0;
    ClientTime openTime;
    ClientTime closeTime;
    int hourlyRate = 0;
};

struct Event {
    ClientTime time;
    EventId id;
    std::string clientName;
    int tableNumber = 0;
    std::string originalLine;
};

#endif