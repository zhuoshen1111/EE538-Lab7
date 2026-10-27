#include "log_chain.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Independent extra-credit console program. Complete src/log_chain.cpp.
// Enter HELP for commands. The SHA-256 helper is already implemented.
int main() {
    lab::LogChain log;
    lab::crypto::Digest trusted = lab::LogChain::initial_digest();
    bool has_saved_digest = false;
    // Print bool results as true/false instead of 1/0.
    std::cout << std::boolalpha;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream input(line);
        std::string command;
        if (!(input >> command)) {
            continue;
        }

        try {
            if (command == "APPEND") {
                std::string record;
                // std::quoted reads text with spaces when it is in double quotes.
                if (!(input >> std::quoted(record))) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }
                log.append(record);
            } else if (command == "DIGEST") {
                std::cout << lab::crypto::to_hex(log.final_digest()) << '\n';
            } else if (command == "SAVE") {
                trusted = log.final_digest();
                has_saved_digest = true;
            } else if (command == "VERIFY") {
                if (!has_saved_digest) {
                    std::cout << "ERROR: use SAVE first\n";
                    continue;
                }
                std::cout << lab::LogChain::verify(log.entries(), trusted) << '\n';
            } else if (command == "CHECK_EDIT") {
                int index;
                std::string record;
                if (!(input >> index >> std::quoted(record))) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }
                if (!has_saved_digest) {
                    std::cout << "ERROR: use SAVE first\n";
                    continue;
                }
                if (index < 0 || static_cast<std::size_t>(index) >= log.size()) {
                    std::cout << "ERROR: record index out of range\n";
                    continue;
                }
                // Edit a copy so this test does not change the original log.
                std::vector<lab::LogEntry> edited = log.entries();
                edited[index].record = record;
                std::cout << lab::LogChain::verify(edited, trusted) << '\n';
            } else if (command == "SIZE") {
                std::cout << log.size() << '\n';
            } else if (command == "EMPTY") {
                std::cout << log.empty() << '\n';
            } else if (command == "CLEAR") {
                log = lab::LogChain();
                has_saved_digest = false;
            } else if (command == "HELP") {
                std::cout << "APPEND record\n"
                             "DIGEST\n"
                             "SAVE\n"
                             "VERIFY\n"
                             "CHECK_EDIT index record\n"
                             "SIZE\n"
                             "EMPTY\n"
                             "CLEAR\n"
                             "HELP\n"
                             "QUIT\n"
                             "Use uppercase commands, one command per line.\n"
                             "Quote records containing spaces; use \"\" for an empty record.\n"
                             "Indices start at 0. SAVE keeps a trusted digest in memory.\n"
                             "CHECK_EDIT verifies a modified copy; CLEAR resets the log and saved digest.\n";
            } else if (command == "QUIT") {
                break;
            } else {
                std::cout << "ERROR: unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "ERROR: " << error.what() << '\n';
        }
    }
    return 0;
}
