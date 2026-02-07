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

        outputStream << config.openTime << std::endl;

        while (auto eventOpt = parser.nextEvent()) {
            Event event = *eventOpt;
            outputStream << event.originalLine << std::endl;
            club.processEvent(event);
        }

        club.closeClub();

    } catch (const std::runtime_error& e) {
        outputStream << e.what() << std::endl;
    }

    return 0;
}