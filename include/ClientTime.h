#ifndef YADRO_CLIENT_TIME_H
#define YADRO_CLIENT_TIME_H

#include <string>
#include <iostream>

class ClientTime {
public:
    ClientTime() = default;
    ClientTime(int h, int m);
    static ClientTime parse(const std::string& str);

    [[nodiscard]] int toMinutes() const;

    bool operator<(const ClientTime& other) const;
    bool operator>(const ClientTime& other) const;
    bool operator<=(const ClientTime& other) const;
    bool operator>=(const ClientTime& other) const;
    bool operator==(const ClientTime& other) const;
    int operator-(const ClientTime& other) const;

    friend std::ostream& operator<<(std::ostream& os, const ClientTime& t);

private:
    int _minutes = 0;
};

#endif