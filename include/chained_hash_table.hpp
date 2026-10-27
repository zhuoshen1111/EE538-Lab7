#ifndef EE538_CHAINED_HASH_TABLE_HPP
#define EE538_CHAINED_HASH_TABLE_HPP

#include <cstddef>
#include <list>
#include <optional>
#include <string>
#include <vector>

namespace lab {

class ChainedHashTable {
public:
    struct Entry {
        std::string key;
        int value;
    };

    static constexpr std::size_t kBucketCount = 7;
    ChainedHashTable() : buckets_(kBucketCount) {}

    // Insert a new key or replace the value of an existing key.
    // Updating a key does not change size(). Empty keys are valid.
    void put(const std::string& key, int value);

    // Return the matching value, or std::nullopt when the key is absent.
    std::optional<int> find(const std::string& key) const;

    // Remove the matching key. Return true if an entry was removed.
    bool erase(const std::string& key);

    // Start h at 0. For each unsigned byte c: h = (31 * h + c) % 7.
    // This function must not depend on whether plain char is signed.
    std::size_t bucket_index(const std::string& key) const;

    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    std::size_t bucket_count() const noexcept { return kBucketCount; }

private:
    std::vector<std::list<Entry>> buckets_;
    std::size_t size_ = 0;
};

}  // namespace lab

#endif
