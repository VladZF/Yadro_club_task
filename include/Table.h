#ifndef YADRO_TABLE_H
#define YADRO_TABLE_H

#include "ClientTime.h"

class Table {
public:
    explicit Table(int number);

    void occupy(ClientTime startTime);

    long long vacate(ClientTime endTime, int hourlyRate);

    [[nodiscard]] bool isBusy() const;
    [[nodiscard]] int getNumber() const;
    [[nodiscard]] long long getRevenue() const;
    [[nodiscard]] ClientTime getTotalBusyTime() const;

private:
    int _number;
    bool _isBusy;
    ClientTime _lastStartTime;
    long long _revenue;
    int _totalBusyMinutes;
};

#endif