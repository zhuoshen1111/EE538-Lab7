#include "chained_hash_table.hpp"

#include <stdexcept>

namespace lab {

std::size_t ChainedHashTable::bucket_index(const std::string& key) const {
    // TODO: Apply the polynomial recurrence to each unsigned byte.
    
    std::size_t h=0;
    for (unsigned char c : key){
        h=(31*h+c)%kBucketCount;

    }
    return h;

}

void ChainedHashTable::put(const std::string& key, int value) {
    // TODO: Find the bucket. Update an equal key, or add a new entry.
    // Different keys may share a bucket; compare the actual strings.
    std::size_t index = bucket_index(key);
    auto& chain = buckets_[index];

    for (auto& entry : chain){
        if (entry.key == key){
            entry.value = value;
            return;
        }
    }
    chain.push_back(Entry{key,value});
    ++size_;
}

std::optional<int> ChainedHashTable::find(const std::string& key) const {
    // TODO: Search only the selected chain. Return std::nullopt if absent.
    std::size_t index = bucket_index(key);
    const auto& chain = buckets_[index];

    for (const auto& entry : chain){
        if (entry.key ==key){
            return entry.value;
        }
    }
    return std::nullopt;
}

bool ChainedHashTable::erase(const std::string& key) {
    // TODO: Erase the matching list entry, if any, and maintain size_.
    // Do not use an iterator after erasing the entry it refers to.
    std::size_t index = bucket_index(key);
    auto& chain = buckets_[index];
    for (auto it = chain.begin(); it !=chain.end();++it){
        if (it->key == key){
            chain.erase(it);
            --size_;
            return true;
        }
    }
    return false;
}

}  // namespace lab
