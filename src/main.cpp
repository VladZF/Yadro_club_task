#include <iostream>
#include <fstream>
#include "InputParser.h"
#include "Club.h"

int main(int argc, char* argv[]) {
    std::istream* inputStream = &std::cin;
    std::ifstream fileStream;

    if (argc >= 2) {
        fileStream.open(argv[1]);
        if (!fileStream.is_open()) {
            std::cerr << "File not found" << std::endl;
            return 0;
        }
        inputStream = &fileStream;
    }

    std::ostream& outputStream = std::cout;

    InputParser parser(*inputStream);

    try {
        ClubConfig config = parser.parseConfig();
        Club club(config, outputStream);

        std::vector<Event> events;

        while (auto eventOpt = parser.nextEvent(config)) {
            Event event = *eventOpt;
            events.push_back(event);
        }

        outputStream << config.openTime << std::endl;

        for (auto event : events) {
            outputStream << event.originalLine << std::endl;
            club.processEvent(event);
        }

        club.closeClub();

    } catch (const std::runtime_error& e) {
        outputStream << e.what() << std::endl;
    }

    return 0;
}