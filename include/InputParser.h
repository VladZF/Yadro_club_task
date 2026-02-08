#ifndef YADRO_INPUT_PARSER_H
#define YADRO_INPUT_PARSER_H

#include <iostream>
#include <string>
#include <optional>
#include <regex>
#include "BaseStructs.h"

class InputParser {
public:
    explicit InputParser(std::istream& input);

    ClubConfig parseConfig();
    std::optional<Event> nextEvent(const ClubConfig& config);

private:
    std::istream& _input;

    const std::regex _timeRegex;
    const std::regex _nameRegex;
    const std::regex _intRegex;

    ClientTime parseTimeToken(const std::string& token, const std::string& fullLine);
    int parseIntToken(const std::string& token, const std::string& fullLine, const std::string& fieldName);
    void validateNameToken(const std::string& token, const std::string& fullLine);
};

#endif