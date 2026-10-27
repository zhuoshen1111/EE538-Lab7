#include "log_chain.hpp"

#include <stdexcept>

namespace lab {

void LogChain::append(const std::string& record) {
    // EXTRA CREDIT TODO: Use crypto::hash_record, then store a LogEntry.
    (void)record;
    throw std::logic_error("TODO: LogChain::append (extra credit)");
}

crypto::Digest LogChain::final_digest() const {
    // EXTRA CREDIT TODO: Handle both an empty and a nonempty chain.
    throw std::logic_error("TODO: LogChain::final_digest (extra credit)");
}

bool LogChain::verify(const std::vector<LogEntry>& entries,
                      const crypto::Digest& trusted_final) {
    // EXTRA CREDIT TODO: Recompute from initial_digest(), compare each
    // stored digest, and finally compare with the independent trusted anchor.
    (void)entries;
    (void)trusted_final;
    throw std::logic_error("TODO: LogChain::verify (extra credit)");
}

}  // namespace lab
