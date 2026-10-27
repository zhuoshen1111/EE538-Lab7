#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

// Provided helper. Students do not need to implement or edit SHA-256.
namespace lab::crypto {

using Digest = std::array<std::uint8_t, 32>;

// Hash the exact input bytes, including any embedded zero bytes.
Digest sha256(std::string_view bytes);

// Return the digest as 64 lowercase hexadecimal characters.
std::string to_hex(const Digest& digest);

// SHA256(previous raw 32 bytes || record bytes).
// No delimiter, newline, terminator, or hexadecimal conversion is added.
Digest hash_record(const Digest& previous, std::string_view record);

}  // namespace lab::crypto
