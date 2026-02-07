#include "InputParser.h"
#include "Constants.h"
#include <sstream>
#include <stdexcept>

InputParser::InputParser(std::istream& input)
    : _input(input)
    , _eventRegex(R"(^(\d{2}:\d{2})\s+(\d+)\s+([a-z0-9_-]+)(?:\s+(\d+))?$)")
{}

ClubConfig InputParser::parseConfig() {
    ClubConfig config;
    std::string line;

    if (!std::getline(_input, line)) throw std::runtime_error("Empty input");
    try {
        config.numTables = std::stoi(line);
        if (config.numTables <= 0) throw std::runtime_error("");
    } catch (...) { throw std::runtime_error(line); }

    if (!std::getline(_input, line)) throw std::runtime_error("Unexpected EOF");
    try {
        std::stringstream ss(line);
        std::string s1, s2;
        ss >> s1 >> s2;
        config.openTime = ClientTime::parse(s1);
        config.closeTime = ClientTime::parse(s2);
    } catch (...) { throw std::runtime_error(line); }

    if (!std::getline(_input, line)) throw std::runtime_error("Unexpected EOF");
    try {
        config.hourlyRate = std::stoi(line);
        if (config.hourlyRate <= 0) throw std::runtime_error("");
    } catch (...) { throw std::runtime_error(line); }

    return config;
}

std::optional<Event> InputParser::nextEvent() {
    std::string line;
    while (std::getline(_input, line)) {
        if (line.empty()) continue;

        std::smatch match;
        if (!std::regex_match(line, match, _eventRegex)) {
            throw std::runtime_error(line);
        }

        try {
            Event e;
            e.originalLine = line;
            e.time = ClientTime::parse(match[1]);
            
            int rawId = std::stoi(match[2]);
            e.id = static_cast<EventId>(rawId);
            
            e.clientName = match[3];

            if (match[4].matched) e.tableNumber = std::stoi(match[4]);

            bool formatError = (e.id == EventId::ClientSat && !match[4].matched) || 
                               (e.id != EventId::ClientSat && match[4].matched);
            if (formatError) throw std::runtime_error(line);

            return e;
        } catch (...) {
            throw std::runtime_error(line);
        }
    }
    return std::nullopt;
}