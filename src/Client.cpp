#include "Client.h"

Client::Client(std::string name)
    : _name(std::move(name))
    , _state(ClientState::InClub)
    , _tableIdx(-1)
{}

const std::string& Client::getName() const {
    return _name;
}

ClientState Client::getState() const {
    return _state;
}

int Client::getTableIndex() const {
    return _tableIdx;
}

void Client::setState(ClientState newState) {
    _state = newState;
    if (_state != ClientState::Sitting) {
        _tableIdx = -1;
    }
}

void Client::setTableIndex(int idx) {
    _tableIdx = idx;
    if (idx != -1) {
        _state = ClientState::Sitting;
    }
}