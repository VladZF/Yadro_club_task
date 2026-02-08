#include "InputParser.h"
#include "Constants.h"
#include <sstream>
#include <stdexcept>
#include <unordered_set>

InputParser::InputParser(std::istream& input)
    : _input(input)
    , _timeRegex(R"(^\d{2}:\d{2}$)")
    , _nameRegex(R"(^[a-z0-9_-]+$)")
    , _intRegex(R"(^\d+$)")
{}

std::unordered_set<int> incomingEventIdsInt = {
    static_cast<int>(EventId::ClientArrived),
    static_cast<int>(EventId::ClientSat),
    static_cast<int>(EventId::ClientWaiting),
    static_cast<int>(EventId::ClientLeft)
};

ClientTime InputParser::parseTimeToken(const std::string& token, const std::string& fullLine) {
    if (!std::regex_match(token, _timeRegex)) {
        throw std::runtime_error(fullLine + " (Error: Invalid time format '" + token + "')");
    }
    try {
        return ClientTime::parse(token);
    } catch (...) {
        throw std::runtime_error(fullLine + " (Error: Invalid time logic '" + token + "')");
    }
}

int InputParser::parseIntToken(const std::string& token, const std::string& fullLine, const std::string& fieldName) {
    if (!std::regex_match(token, _intRegex)) {
        throw std::runtime_error(fullLine + " (Error: " + fieldName + " is not a positive integer '" + token + "')");
    }
    try {
        return std::stoi(token);
    } catch (...) {
        throw std::runtime_error(fullLine + " (Error: " + fieldName + " number too big)");
    }
}

void InputParser::validateNameToken(const std::string& token, const std::string& fullLine) {
    if (!std::regex_match(token, _nameRegex)) {
        throw std::runtime_error(fullLine + " (Error: Invalid characters in client name '" + token + "'. Allowed: a-z, 0-9, _, -)");
    }
}

ClubConfig InputParser::parseConfig() {
    ClubConfig config;
    std::string line;

    if (!std::getline(_input, line)) throw std::runtime_error("Input is empty");
    config.numTables = parseIntToken(line, line, "Number of tables");
    if (config.numTables <= 0) 
        throw std::runtime_error(line + " (Error: Number of tables must be > 0)");

    if (!std::getline(_input, line)) throw std::runtime_error("Unexpected EOF while reading hours");
    {
        std::stringstream ss(line);
        std::string startStr, endStr, extra;
        
        if (!(ss >> startStr >> endStr)) 
            throw std::runtime_error(line + " (Error: Expected two time values)");
        
        if (ss >> extra) 
            throw std::runtime_error(line + " (Error: Unexpected data after closing time '" + extra + "')");

        config.openTime = parseTimeToken(startStr, line);
        config.closeTime = parseTimeToken(endStr, line);

        if (config.openTime >= config.closeTime) {
            throw std::runtime_error(line + " (Error: Open time must be earlier than close time)");
        }
    }

    if (!std::getline(_input, line)) throw std::runtime_error("Unexpected EOF while reading price");
    config.hourlyRate = parseIntToken(line, line, "Hourly rate");
    if (config.hourlyRate < 0) 
        throw std::runtime_error(line + " (Error: Price cannot be negative)");

    return config;
}

std::optional<Event> InputParser::nextEvent(const ClubConfig& config) {
    std::string line;
    while (std::getline(_input, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string timeStr, idStr, nameStr, tableStr, extra;

        Event e;
        e.originalLine = line;

        if (!(ss >> timeStr)) {
            continue; 
        }
        e.time = parseTimeToken(timeStr, line);

        if (!(ss >> idStr)) {
             throw std::runtime_error(line + " (Error: Missing event ID)");
        }
        int idVal = parseIntToken(idStr, line, "Event ID");
        if (!incomingEventIdsInt.contains(idVal)) {
             throw std::runtime_error(line + " (Error: wrong incoming Event ID)");
        }
        e.id = static_cast<EventId>(idVal);

        if (!(ss >> nameStr)) {
             throw std::runtime_error(line + " (Error: Missing client name)");
        }
        validateNameToken(nameStr, line);
        e.clientName = nameStr;

        if (e.id == EventId::ClientSat) {
            if (!(ss >> tableStr)) {
                throw std::runtime_error(line + " (Error: Event ID 2 requires a table number)");
            }
            e.tableNumber = parseIntToken(tableStr, line, "Table number");
            if (e.tableNumber <= 0) {
                 throw std::runtime_error(line + " (Error: Table number must be positive)");
            }
            if (e.tableNumber > config.numTables) {
                throw std::runtime_error(line + " (Error: Table number is bigger than tables count)");
            }
        }
        
        if (ss >> extra) {
            if (e.id != EventId::ClientSat) {
                throw std::runtime_error(line + " (Error: Unexpected argument '" + extra + "' for Event ID " + idStr + ")");
            } else {
                throw std::runtime_error(line + " (Error: Unexpected data at end of line '" + extra + "')");
            }
        }

        return e;
    }
    return std::nullopt;
}