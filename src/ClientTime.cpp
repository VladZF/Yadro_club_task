#include "ClientTime.h"
#include <iomanip>
#include <stdexcept>

ClientTime::ClientTime(int h, int m) {
    if (h < 0 || h > 23 || m < 0 || m > 59) throw std::invalid_argument("Invalid time");
    _minutes = h * 60 + m;
}

ClientTime ClientTime::parse(const std::string& str) {
    if (str.length() != 5 || str[2] != ':') throw std::invalid_argument("Format error");
    try {
        int h = std::stoi(str.substr(0, 2));
        int m = std::stoi(str.substr(3, 2));
        return {h, m};
    } catch (...) {
        throw std::invalid_argument("Parse error");
    }
}

int ClientTime::toMinutes() const { return _minutes; }

bool ClientTime::operator<(const ClientTime& other) const { return _minutes < other._minutes; }
bool ClientTime::operator>(const ClientTime& other) const { return _minutes > other._minutes; }
bool ClientTime::operator<=(const ClientTime& other) const { return _minutes <= other._minutes; }
bool ClientTime::operator>=(const ClientTime& other) const { return _minutes >= other._minutes; }
bool ClientTime::operator==(const ClientTime& other) const { return _minutes == other._minutes; }

int ClientTime::operator-(const ClientTime& other) const {
    return _minutes - other._minutes;
}

std::ostream& operator<<(std::ostream& os, const ClientTime& t) {
    return os << std::setw(2) << std::setfill('0') << (t._minutes / 60) << ":"
              << std::setw(2) << std::setfill('0') << (t._minutes % 60);
}