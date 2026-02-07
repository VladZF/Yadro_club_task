#ifndef YADRO_EVENT_HANDLERS_H
#define YADRO_EVENT_HANDLERS_H

#include "BaseStructs.h"

class Club;

class EventHandler {
public:
    virtual ~EventHandler() = default;
    virtual void handle(Club& club, const Event& event) = 0;
};

class ClientArrivedHandler : public EventHandler {
public:
    void handle(Club& club, const Event& event) override;
};

class ClientSatHandler : public EventHandler {
public:
    void handle(Club& club, const Event& event) override;
};

class ClientWaitingHandler : public EventHandler {
public:
    void handle(Club& club, const Event& event) override;
};

class ClientLeftHandler : public EventHandler {
public:
    void handle(Club& club, const Event& event) override;
};

#endif