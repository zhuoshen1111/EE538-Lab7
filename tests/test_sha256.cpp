#include "sha256.hpp"
#include "test_support.hpp"

#include <string>

int main() {
    test::Runner tests;
    tests.run("SHA-256: empty string", [] {
        CHECK(lab::crypto::to_hex(lab::crypto::sha256("")) ==
              "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    });
    tests.run("SHA-256: abc", [] {
        CHECK(lab::crypto::to_hex(lab::crypto::sha256("abc")) ==
              "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    });
    tests.run("SHA-256: a message that requires two padded blocks", [] {
        const std::string message =
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
        CHECK(lab::crypto::to_hex(lab::crypto::sha256(message)) ==
              "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    });
    tests.run("hash_record: raw previous digest followed by exact record bytes", [] {
        lab::crypto::Digest previous{};
        previous[0] = 0xA5;
        previous[31] = 0x7F;
        const std::string record("A\0B", 3);
        std::string expected_input;
        for (const auto byte : previous) {
            expected_input.push_back(static_cast<char>(byte));
        }
        expected_input += record;
        CHECK(lab::crypto::hash_record(previous, record) ==
              lab::crypto::sha256(expected_input));
    });
    return tests.finish();
}
