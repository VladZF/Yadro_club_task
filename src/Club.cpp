#include "Club.h"
#include <algorithm>

Club::Club(const ClubConfig& config, std::ostream& output)
    : _out(output)
    , _numTables(config.numTables)
    , _openTime(config.openTime)
    , _closeTime(config.closeTime)
    , _hourlyRate(config.hourlyRate)
{
    for(int i = 1; i <= _numTables; ++i) {
        _tables.emplace_back(i);
    }

    _handlers[EventId::ClientArrived] = std::make_unique<ClientArrivedHandler>();
    _handlers[EventId::ClientSat] = std::make_unique<ClientSatHandler>();
    _handlers[EventId::ClientWaiting] = std::make_unique<ClientWaitingHandler>();
    _handlers[EventId::ClientLeft] = std::make_unique<ClientLeftHandler>();
}

void Club::processEvent(const Event& event) {
    auto it = _handlers.find(event.id);
    if (it != _handlers.end()) {
        it->second->handle(*this, event);
    }
}

void Club::sitClient(Client& client, int tableIdx, ClientTime time) {
    _tables[tableIdx].occupy(time);
    client.setTableIndex(tableIdx);
}

void Club::checkQueueAndOccupy(int tableIdx, ClientTime time) {
    if (!_queue.empty()) {
        std::string nextClientName = _queue.front();
        _queue.pop_front();
        _out << time << " " << static_cast<int>(EventId::OutClientSat) 
             << " " << nextClientName << " " << (tableIdx + 1) << std::endl;
        
        Client& nextClient = _clients.at(nextClientName);
        sitClient(nextClient, tableIdx, time);
    }
}

void Club::clientLeavesClub(Client& client, ClientTime time) {
    std::string name = client.getName();
    ClientState state = client.getState();
    int tableIdx = client.getTableIndex();

    _clients.erase(name);

    if (state == ClientState::Sitting) {
        _tables[tableIdx].vacate(time, _hourlyRate);
        checkQueueAndOccupy(tableIdx, time);
    } else if (state == ClientState::Waiting) {
        auto it = std::find(_queue.begin(), _queue.end(), name);
        if (it != _queue.end()) _queue.erase(it);
    }
}

void Club::logError(ClientTime time, const std::string& msg) {
    _out << time << " " << static_cast<int>(EventId::Error) << " " << msg << std::endl;
}

void Club::logEvent(ClientTime time, EventId id, const std::string& msg) {
    _out << time << " " << static_cast<int>(id) << " " << msg << std::endl;
}

void Club::closeClub() {
    std::vector<std::string> names;
    names.reserve(_clients.size());
    for(const auto& pair : _clients) {
        names.push_back(pair.first);
    }

    for(const auto& name : names) {
        _out << _closeTime << " " << static_cast<int>(EventId::OutClientLeft) 
             << " " << name << std::endl;
        
        Client& client = _clients.at(name);
        if (client.getState() == ClientState::Sitting) {
            _tables[client.getTableIndex()].vacate(_closeTime, _hourlyRate);
        }
    }
    _clients.clear();
    _queue.clear();

    _out << _closeTime << std::endl;

    for(const auto& t : _tables) {
        _out << t.getNumber() << " " << t.getRevenue() << " " << t.getTotalBusyTime() << std::endl;
    }
}