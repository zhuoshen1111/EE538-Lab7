#ifndef EE538_LOG_CHAIN_HPP
#define EE538_LOG_CHAIN_HPP

#include "sha256.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace lab {

struct LogEntry {
    std::string record;
    crypto::Digest digest;
};

// Optional extra credit. The SHA-256 calculation is provided for you.
class LogChain {
public:
    // d0 is exactly 32 zero bytes, not the hash of an empty string.
    static crypto::Digest initial_digest() { return {}; }

    // Store the record and hash_record(previous_digest, record).
    // Use initial_digest() before the first record.
    void append(const std::string& record);

    // Return the last stored digest, or initial_digest() for an empty chain.
    crypto::Digest final_digest() const;

    // Recompute from d0, checking every stored digest and the trusted final
    // digest. An empty chain is valid only if trusted_final equals d0.
    static bool verify(const std::vector<LogEntry>& entries,
                       const crypto::Digest& trusted_final);

    const std::vector<LogEntry>& entries() const noexcept { return entries_; }
    std::size_t size() const noexcept { return entries_.size(); }
    bool empty() const noexcept { return entries_.empty(); }

private:
    std::vector<LogEntry> entries_;
};

}  // namespace lab

#endif
