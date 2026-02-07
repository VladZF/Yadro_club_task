#ifndef YADRO_CONSTANTS_H
#define YADRO_CONSTANTS_H

#include <string>

enum class EventId : int {
    ClientArrived = 1,
    ClientSat = 2,
    ClientWaiting = 3,
    ClientLeft = 4,
    
    OutClientLeft = 11,
    OutClientSat = 12,
    Error = 13
};

namespace ErrorMessages {
    const char* const YouShallNotPass = "YouShallNotPass";
    const char* const NotOpenYet = "NotOpenYet";
    const char* const PlaceIsBusy = "PlaceIsBusy";
    const char* const ClientUnknown = "ClientUnknown";
    const char* const ICanWaitNoLonger = "ICanWaitNoLonger!";
}

#endif