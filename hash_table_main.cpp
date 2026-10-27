#include "chained_hash_table.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

// Enter one uppercase command per line. HELP lists the commands.
// This simple driver assumes the documented input format and ignores extra tokens.
// Complete the hash-table algorithms in src/chained_hash_table.cpp.
int main() {
    lab::ChainedHashTable table;
    std::string line;

    // Print boolean results as true or false instead of 1 or 0.
    std::cout << std::boolalpha;

    while (std::getline(std::cin, line)) {
        std::istringstream input(line);
        std::string command;
        if (!(input >> command)) {
            continue;
        }

        try {
            if (command == "PUT") {
                std::string key;
                int value;
                // std::quoted accepts bare keys, "keys with spaces", and "".
                if (!(input >> std::quoted(key) >> value)) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }
                table.put(key, value);
            } else if (command == "FIND") {
                std::string key;
                if (!(input >> std::quoted(key))) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }
                // An optional contains a value only when the key was found.
                std::optional<int> result = table.find(key);
                if (result.has_value()) {
                    std::cout << result.value() << '\n';
                } else {
                    std::cout << "NOT_FOUND\n";
                }
            } else if (command == "ERASE") {
                std::string key;
                if (!(input >> std::quoted(key))) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }
                std::cout << table.erase(key) << '\n';
            } else if (command == "BUCKET") {
                std::string key;
                if (!(input >> std::quoted(key))) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }
                std::cout << table.bucket_index(key) << '\n';
            } else if (command == "SIZE") {
                std::cout << table.size() << '\n';
            } else if (command == "EMPTY") {
                std::cout << table.empty() << '\n';
            } else if (command == "CLEAR") {
                table = lab::ChainedHashTable();
            } else if (command == "HELP") {
                std::cout << "PUT key value\n"
                             "FIND key\n"
                             "ERASE key\n"
                             "BUCKET key\n"
                             "SIZE\n"
                             "EMPTY\n"
                             "CLEAR\n"
                             "HELP\n"
                             "QUIT\n"
                             "Keys may be single tokens or quoted strings, including \"\".\n";
            } else if (command == "QUIT") {
                break;
            } else {
                std::cout << "ERROR: unknown command\n";
            }
        } catch (const std::exception& error) {
            // Report unfinished TODO methods and allow the next command.
            std::cout << "ERROR: " << error.what() << '\n';
        }
    }

    return 0;
}
