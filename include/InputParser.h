#ifndef YADRO_INPUT_PARSER_H
#define YADRO_INPUT_PARSER_H

#include <iostream>
#include <regex>
#include <optional>
#include "BaseStructs.h"

class InputParser {
public:
    explicit InputParser(std::istream& input);

    ClubConfig parseConfig();
    std::optional<Event> nextEvent();

private:
    std::istream& _input;
    const std::regex _eventRegex;
};

#endif