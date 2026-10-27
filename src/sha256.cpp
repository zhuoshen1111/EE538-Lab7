#include "sha256.hpp"

#include <limits>
#include <stdexcept>
#include <vector>

// Provided teaching helper, written directly from the SHA-256 specification.
// Algorithm and constants: NIST FIPS 180-4, sections 4.1.2, 4.2.2, 5, and 6.2.
// https://csrc.nist.gov/pubs/fips/180-4/upd1/final
// This is a compact, one-shot implementation for the lab, not a streaming API.

namespace lab::crypto {
namespace {

constexpr std::array<std::uint32_t, 64> round_constants{
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
    0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
    0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
    0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
    0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
    0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
    0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
    0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u
};

std::uint32_t rotate_right(std::uint32_t value, unsigned bits) {
    return (value >> bits) | (value << (32u - bits));
}

void compress(const std::uint8_t* block,
              std::array<std::uint32_t, 8>& state) {
    std::array<std::uint32_t, 64> words{};
    for (std::size_t i = 0; i < 16; ++i) {
        const auto j = i * 4;
        words[i] = (std::uint32_t{block[j]} << 24u)
                 | (std::uint32_t{block[j + 1]} << 16u)
                 | (std::uint32_t{block[j + 2]} << 8u)
                 | std::uint32_t{block[j + 3]};
    }
    for (std::size_t i = 16; i < words.size(); ++i) {
        const auto x = words[i - 15];
        const auto y = words[i - 2];
        const auto s0 = rotate_right(x, 7) ^ rotate_right(x, 18) ^ (x >> 3u);
        const auto s1 = rotate_right(y, 17) ^ rotate_right(y, 19) ^ (y >> 10u);
        words[i] = words[i - 16] + s0 + words[i - 7] + s1;
    }

    auto a = state[0];
    auto b = state[1];
    auto c = state[2];
    auto d = state[3];
    auto e = state[4];
    auto f = state[5];
    auto g = state[6];
    auto h = state[7];

    for (std::size_t i = 0; i < words.size(); ++i) {
        const auto sum1 = rotate_right(e, 6) ^ rotate_right(e, 11)
                        ^ rotate_right(e, 25);
        const auto choose = (e & f) ^ (~e & g);
        const auto t1 = h + sum1 + choose + round_constants[i] + words[i];
        const auto sum0 = rotate_right(a, 2) ^ rotate_right(a, 13)
                        ^ rotate_right(a, 22);
        const auto majority = (a & b) ^ (a & c) ^ (b & c);
        const auto t2 = sum0 + majority;
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

}  // namespace

Digest sha256(std::string_view bytes) {
    if (bytes.size() > std::numeric_limits<std::uint64_t>::max() / 8u) {
        throw std::length_error("SHA-256 input is too long");
    }
    const auto bit_length = static_cast<std::uint64_t>(bytes.size()) * 8u;
    std::vector<std::uint8_t> padded;
    padded.reserve(bytes.size());
    for (const unsigned char byte : bytes) {
        padded.push_back(byte);
    }
    padded.push_back(0x80u);
    while (padded.size() % 64 != 56) {
        padded.push_back(0);
    }
    for (int shift = 56; shift >= 0; shift -= 8) {
        padded.push_back(static_cast<std::uint8_t>(bit_length >> shift));
    }

    std::array<std::uint32_t, 8> state{
        0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
        0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u
    };
    for (std::size_t offset = 0; offset < padded.size(); offset += 64) {
        compress(padded.data() + offset, state);
    }

    Digest digest{};
    for (std::size_t i = 0; i < state.size(); ++i) {
        digest[i * 4]     = static_cast<std::uint8_t>(state[i] >> 24u);
        digest[i * 4 + 1] = static_cast<std::uint8_t>(state[i] >> 16u);
        digest[i * 4 + 2] = static_cast<std::uint8_t>(state[i] >> 8u);
        digest[i * 4 + 3] = static_cast<std::uint8_t>(state[i]);
    }
    return digest;
}

std::string to_hex(const Digest& digest) {
    constexpr char digits[] = "0123456789abcdef";
    std::string result;
    result.reserve(digest.size() * 2);
    for (const auto byte : digest) {
        result.push_back(digits[byte >> 4u]);
        result.push_back(digits[byte & 0x0fu]);
    }
    return result;
}

Digest hash_record(const Digest& previous, std::string_view record) {
    std::string bytes(reinterpret_cast<const char*>(previous.data()),
                      previous.size());
    if (!record.empty()) {
        bytes.append(record.data(), record.size());
    }
    return sha256(bytes);
}

}  // namespace lab::crypto
