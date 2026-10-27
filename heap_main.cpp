#include "min_heap.hpp"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// This driver handles terminal input. Implement the heap in src/min_heap.cpp.
int main() {
    lab::MinHeap heap;
    std::string line;
    std::cout << std::boolalpha;  // Print bool values as true or false.

    // Read one line, then use a string stream to read its command and arguments.
    while (std::getline(std::cin, line)) {
        std::istringstream input(line);
        std::string command;
        if (!(input >> command)) {
            continue;
        }

        try {
            if (command == "BUILD") {
                int count;
                if (!(input >> count) || count < 0) {
                    std::cout << "ERROR: invalid arguments\n";
                    continue;
                }

                std::vector<int> values;
                bool complete = true;
                for (int i = 0; i < count; i++) {
                    int value;
                    if (!(input >> value)) {
                        complete = false;
                        break;
                    }
                    values.push_back(value);
                }
                if (complete) {
                    heap.build(values);
                } else {
                    std::cout << "ERROR: invalid arguments\n";
                }
            } else if (command == "PUSH") {
                int value;
                if (input >> value) {
                    heap.push(value);
                } else {
                    std::cout << "ERROR: invalid arguments\n";
                }
            } else if (command == "TOP") {
                std::cout << heap.top() << '\n';
            } else if (command == "POP") {
                std::cout << heap.pop() << '\n';
            } else if (command == "SIZE") {
                std::cout << heap.size() << '\n';
            } else if (command == "EMPTY") {
                std::cout << heap.empty() << '\n';
            } else if (command == "PRINT") {
                for (std::size_t i = 0; i < heap.size(); i++) {
                    if (i > 0) {
                        std::cout << ' ';
                    }
                    std::cout << heap.data()[i];
                }
                std::cout << '\n';
            } else if (command == "CLEAR") {
                heap = lab::MinHeap();
            } else if (command == "HELP") {
                std::cout << "BUILD n v1 ... vn\nPUSH value\nTOP\nPOP\n"
                             "SIZE\nEMPTY\nPRINT\nCLEAR\nHELP\nQUIT\n";
            } else if (command == "QUIT") {
                break;
            } else {
                std::cout << "ERROR: unknown command\n";
            }
        } catch (const std::out_of_range&) {
            std::cout << "ERROR: empty heap\n";
        } catch (const std::exception& error) {
            std::cout << "ERROR: " << error.what() << '\n';
        }
    }
    return 0;
}
