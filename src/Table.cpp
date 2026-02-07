#include "Table.h"

Table::Table(int number)
    : _number(number), _isBusy(false), _revenue(0), _totalBusyMinutes(0) {}

void Table::occupy(ClientTime startTime) {
    _isBusy = true;
    _lastStartTime = startTime;
}

long long Table::vacate(ClientTime endTime, int hourlyRate) {
    if (!_isBusy) return 0;

    int duration = endTime - _lastStartTime;
    if (duration < 0) duration = 0;

    _totalBusyMinutes += duration;
    _isBusy = false;

    long long hoursToPay = (duration + 59) / 60;
    long long cost = hoursToPay * hourlyRate;
    _revenue += cost;
    return cost;
}

bool Table::isBusy() const { return _isBusy; }
int Table::getNumber() const { return _number; }
long long Table::getRevenue() const { return _revenue; }

ClientTime Table::getTotalBusyTime() const {
    return { _totalBusyMinutes / 60, _totalBusyMinutes % 60 };
}