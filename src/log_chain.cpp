#include "log_chain.hpp"

#include <stdexcept>

namespace lab {

void LogChain::append(const std::string& record) {
    // EXTRA CREDIT TODO: Use crypto::hash_record, then store a LogEntry.
    crypto::Digest previous = final_digest();
    crypto::Digest digest=
        crypto::hash_record(previous,record);
    
    entries_.push_back(LogEntry{record,digest});
}

crypto::Digest LogChain::final_digest() const {
    // EXTRA CREDIT TODO: Handle both an empty and a nonempty chain.
    if (entries_.empty()){
        return initial_digest();
    }
    return entries_.back().digest;
}


    // EXTRA CREDIT TODO: Recompute from initial_digest(), compare each
    // stored digest, and finally compare with the independent trusted anchor.
bool LogChain::verify(
    const std::vector<LogEntry>& entries,
    const crypto::Digest& trusted_final) {

    crypto::Digest previous = initial_digest();

    for (const auto& entry : entries) {
        crypto::Digest computed =
            crypto::hash_record(previous, entry.record);

        if (computed != entry.digest) {
            return false;
        }

        previous = computed;
    }

    return previous == trusted_final;
}  

 
}  // namespace lab
