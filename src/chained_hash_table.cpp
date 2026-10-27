#include "chained_hash_table.hpp"

#include <stdexcept>

namespace lab {

std::size_t ChainedHashTable::bucket_index(const std::string& key) const {
    // TODO: Apply the polynomial recurrence to each unsigned byte.
    (void)key;
    throw std::logic_error("TODO: ChainedHashTable::bucket_index");
}

void ChainedHashTable::put(const std::string& key, int value) {
    // TODO: Find the bucket. Update an equal key, or add a new entry.
    // Different keys may share a bucket; compare the actual strings.
    (void)key;
    (void)value;
    throw std::logic_error("TODO: ChainedHashTable::put");
}

std::optional<int> ChainedHashTable::find(const std::string& key) const {
    // TODO: Search only the selected chain. Return std::nullopt if absent.
    (void)key;
    throw std::logic_error("TODO: ChainedHashTable::find");
}

bool ChainedHashTable::erase(const std::string& key) {
    // TODO: Erase the matching list entry, if any, and maintain size_.
    // Do not use an iterator after erasing the entry it refers to.
    (void)key;
    throw std::logic_error("TODO: ChainedHashTable::erase");
}

}  // namespace lab
