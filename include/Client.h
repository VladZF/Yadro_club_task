#ifndef YADRO_CLIENT_H
#define YADRO_CLIENT_H

#include <string>

enum class ClientState {
    InClub,
    Waiting,
    Sitting
};

class Client {
public:
    explicit Client(std::string name);

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] ClientState getState() const;
    [[nodiscard]] int getTableIndex() const;

    void setState(ClientState newState);
    void setTableIndex(int idx);

private:
    std::string _name;
    ClientState _state;
    int _tableIdx;
};

#endif