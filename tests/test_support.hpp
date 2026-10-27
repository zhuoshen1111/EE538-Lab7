#ifndef EE538_TEST_SUPPORT_HPP
#define EE538_TEST_SUPPORT_HPP

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace test {

inline void check(bool condition, const char* expression,
                  const char* file, int line) {
    if (!condition) {
        std::ostringstream message;
        message << file << ':' << line << ": CHECK failed: " << expression;
        throw std::runtime_error(message.str());
    }
}

template <typename Exception, typename Function>
void check_throws(Function&& action, const char* expression,
                  const char* file, int line) {
    try {
        action();
    } catch (const Exception&) {
        return;
    } catch (const std::exception& error) {
        std::ostringstream message;
        message << file << ':' << line << ": wrong exception for "
                << expression << ": " << error.what();
        throw std::runtime_error(message.str());
    } catch (...) {
        throw std::runtime_error("Unexpected non-standard exception");
    }
    check(false, expression, file, line);
}

class Runner {
public:
    template <typename Function>
    void run(const std::string& name, Function&& action) {
        ++total_;
        try {
            action();
            ++passed_;
            std::cout << "[PASS] " << name << '\n';
        } catch (const std::exception& error) {
            std::cout << "[FAIL] " << name << " -- " << error.what() << '\n';
        } catch (...) {
            std::cout << "[FAIL] " << name << " -- unknown exception\n";
        }
    }

    int finish() const {
        std::cout << '\n' << passed_ << '/' << total_ << " cases passed.\n";
        return passed_ == total_ ? 0 : 1;
    }

private:
    int total_ = 0;
    int passed_ = 0;
};

}  // namespace test

// Unlike assert(), these checks also run in Release builds with NDEBUG.
#define CHECK(expression) \
    ::test::check(static_cast<bool>(expression), #expression, __FILE__, __LINE__)
#define CHECK_THROWS_AS(expression, exception_type) \
    ::test::check_throws<exception_type>([&]() { (void)(expression); }, \
        #expression " throws " #exception_type, __FILE__, __LINE__)

#endif
