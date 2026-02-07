#ifndef YADRO_CLUB_H
#define YADRO_CLUB_H

#include <vector>
#include <deque>
#include <map>
#include <memory>
#include <iostream>

#include "Table.h"
#include "Client.h"
#include "BaseStructs.h"
#include "Constants.h"
#include "EventHandlers.h"


class Club {
public:
    Club(const ClubConfig& config, std::ostream& output);

    void processEvent(const Event& event);
    void closeClub();

    friend class ClientArrivedHandler;
    friend class ClientSatHandler;
    friend class ClientWaitingHandler;
    friend class ClientLeftHandler;

private:
    void sitClient(Client& client, int tableIdx, ClientTime time);
    void clientLeavesClub(Client& client, ClientTime time);
    void checkQueueAndOccupy(int tableIdx, ClientTime time);
    
    void logError(ClientTime time, const std::string& msg);
    void logEvent(ClientTime time, EventId id, const std::string& msg);

    std::ostream& _out;

    int _numTables;
    ClientTime _openTime;
    ClientTime _closeTime;
    int _hourlyRate;

    std::vector<Table> _tables;
    std::map<std::string, Client> _clients; 
    std::deque<std::string> _queue;

    std::map<EventId, std::unique_ptr<EventHandler>> _handlers;
};

#endif