#include "EventHandlers.h"
#include "Club.h"
#include "Constants.h"
#include <algorithm>

void ClientArrivedHandler::handle(Club& club, const Event& e) {
    if (club._clients.count(e.clientName)) {
        club.logError(e.time, ErrorMessages::YouShallNotPass);
        return;
    }

    if (e.time < club._openTime || e.time > club._closeTime) {
        club.logError(e.time, ErrorMessages::NotOpenYet);
        return;
    }

    club._clients.emplace(e.clientName, Client(e.clientName));
}

void ClientSatHandler::handle(Club& club, const Event& e) {
    if (!club._clients.count(e.clientName)) {
        club.logError(e.time, ErrorMessages::ClientUnknown);
        return;
    }

    int tableIdx = e.tableNumber - 1;
    if (club._tables[tableIdx].isBusy()) {
        club.logError(e.time, ErrorMessages::PlaceIsBusy);
        return;
    }

    Client& client = club._clients.at(e.clientName);

    if (client.getState() == ClientState::Sitting) {
        int oldTableIdx = client.getTableIndex();
        club._tables[oldTableIdx].vacate(e.time, club._hourlyRate);
        
        club.sitClient(client, tableIdx, e.time);
        
        club.checkQueueAndOccupy(oldTableIdx, e.time);
    } 
    else {
        if (client.getState() == ClientState::Waiting) {
            auto it = std::find(club._queue.begin(), club._queue.end(), e.clientName);
            if (it != club._queue.end()) club._queue.erase(it);
        }
        club.sitClient(client, tableIdx, e.time);
    }
}

void ClientWaitingHandler::handle(Club& club, const Event& e) {
    if (!club._clients.count(e.clientName)) {
        club.logError(e.time, ErrorMessages::ClientUnknown);
        return;
    }

    bool hasFree = false;
    for (const auto& t : club._tables) {
        if (!t.isBusy()) { hasFree = true; break; }
    }

    if (hasFree) {
        club.logError(e.time, ErrorMessages::ICanWaitNoLonger);
        return;
    }

    if (club._queue.size() >= (size_t)club._numTables) {
        club.logEvent(e.time, EventId::OutClientLeft, e.clientName);
        Client& client = club._clients.at(e.clientName);
        club.clientLeavesClub(client, e.time);
    } else {
        club._queue.push_back(e.clientName);
        club._clients.at(e.clientName).setState(ClientState::Waiting);
    }
}

void ClientLeftHandler::handle(Club& club, const Event& e) {
    if (!club._clients.count(e.clientName)) {
        club.logError(e.time, ErrorMessages::ClientUnknown);
        return;
    }
    
    Client& client = club._clients.at(e.clientName);
    club.clientLeavesClub(client, e.time);
}