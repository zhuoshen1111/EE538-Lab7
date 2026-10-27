#include "chained_hash_table.hpp"
#include "min_heap.hpp"
#include "test_support.hpp"

#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void check_heap(const lab::MinHeap& heap) {
    const auto& values = heap.data();
    CHECK(heap.size() == values.size());
    CHECK(heap.empty() == values.empty());
    for (std::size_t child = 1; child < values.size(); ++child) {
        CHECK(values[(child - 1) / 2] <= values[child]);
    }
}

std::vector<int> drain(lab::MinHeap& heap) {
    std::vector<int> result;
    while (!heap.empty()) {
        const std::size_t old_size = heap.size();
        const int expected = heap.top();
        CHECK(heap.size() == old_size);  // top must not remove an element.
        const int removed = heap.pop();
        CHECK(removed == expected);
        CHECK(heap.size() == old_size - 1);
        result.push_back(removed);
        check_heap(heap);
    }
    return result;
}

}  // namespace

int main() {
    test::Runner tests;

    tests.run("heap: empty top and pop throw out_of_range", [] {
        lab::MinHeap heap;
        CHECK(heap.empty());
        CHECK(heap.size() == 0);
        CHECK_THROWS_AS(heap.top(), std::out_of_range);
        CHECK_THROWS_AS(heap.pop(), std::out_of_range);
    });

    tests.run("heap: build, top, and sorted removal", [] {
        lab::MinHeap heap;
        heap.build({9, 4, 7, 1, 0, 3, 2});
        CHECK(heap.size() == 7);
        CHECK(heap.top() == 0);
        check_heap(heap);
        CHECK(drain(heap) == std::vector<int>({0, 1, 2, 3, 4, 7, 9}));
    });

    tests.run("heap: push and pop restore heap order", [] {
        lab::MinHeap heap;
        for (const int value : {8, 5, 12, 2, 7, 10, 3}) {
            const auto before = heap.size();
            heap.push(value);
            CHECK(heap.size() == before + 1);
            check_heap(heap);
        }
        CHECK(heap.pop() == 2);
        check_heap(heap);
        heap.push(1);
        check_heap(heap);
        CHECK(drain(heap) == std::vector<int>({1, 3, 5, 7, 8, 10, 12}));
    });

    tests.run("heap: duplicates, negative values, and one child", [] {
        lab::MinHeap heap;
        heap.build({3, -2, 3, 0, -2, 1});
        check_heap(heap);
        CHECK(drain(heap) == std::vector<int>({-2, -2, 0, 1, 3, 3}));
        heap.build({2, 1});
        CHECK(drain(heap) == std::vector<int>({1, 2}));
    });

    tests.run("heap: build replaces contents; singleton and empty build", [] {
        lab::MinHeap heap;
        heap.build({4, 2, 6});
        heap.build({-5});
        CHECK(heap.size() == 1);
        CHECK(heap.top() == -5);
        CHECK(heap.pop() == -5);
        CHECK(heap.empty());
        heap.push(8);
        heap.build({});
        CHECK(heap.empty());
        CHECK_THROWS_AS(heap.top(), std::out_of_range);
        CHECK_THROWS_AS(heap.pop(), std::out_of_range);
    });

    tests.run("hash: polynomial bucket index", [] {
        const lab::ChainedHashTable table;
        CHECK(table.bucket_count() == 7);
        CHECK(table.bucket_index("") == 0);
        CHECK(table.bucket_index("a") == 6);
        CHECK(table.bucket_index("h") == 6);
        CHECK(table.bucket_index("o") == 6);
        CHECK(table.bucket_index("ab") == 4);
        const std::string byte(1, static_cast<char>(0xFF));
        CHECK(table.bucket_index(byte) == 3);  // 255 % 7
    });

    tests.run("hash: an empty table and absent keys", [] {
        lab::ChainedHashTable table;
        CHECK(table.empty());
        CHECK(table.size() == 0);
        CHECK(!table.find("missing").has_value());
        CHECK(!table.erase("missing"));
        CHECK(table.size() == 0);
    });

    tests.run("hash: insert and lookup across buckets", [] {
        lab::ChainedHashTable table;
        table.put("alice", 10);
        table.put("bob", -1);
        table.put("carol", 0);
        CHECK(table.size() == 3);
        CHECK(!table.empty());
        CHECK(table.find("alice") == std::optional<int>(10));
        CHECK(table.find("bob") == std::optional<int>(-1));
        CHECK(table.find("carol") == std::optional<int>(0));
        CHECK(!table.find("dave").has_value());
    });

    tests.run("hash: colliding keys remain distinct; duplicate key updates", [] {
        lab::ChainedHashTable table;
        table.put("a", 10);
        table.put("h", 20);
        table.put("o", 30);
        CHECK(table.size() == 3);
        CHECK(table.find("a") == std::optional<int>(10));
        CHECK(table.find("h") == std::optional<int>(20));
        CHECK(table.find("o") == std::optional<int>(30));
        table.put("h", 99);
        CHECK(table.size() == 3);
        CHECK(table.find("h") == std::optional<int>(99));
        CHECK(table.find("a") == std::optional<int>(10));
        CHECK(table.find("o") == std::optional<int>(30));
    });

    tests.run("hash: erase from a collision chain and reuse the key", [] {
        lab::ChainedHashTable table;
        table.put("a", 10);
        table.put("h", 20);
        table.put("o", 30);
        CHECK(table.erase("h"));
        CHECK(table.size() == 2);
        CHECK(!table.find("h").has_value());
        CHECK(table.find("a") == std::optional<int>(10));
        CHECK(table.find("o") == std::optional<int>(30));
        CHECK(!table.erase("h"));
        CHECK(table.size() == 2);
        table.put("h", 40);
        CHECK(table.size() == 3);
        CHECK(table.find("h") == std::optional<int>(40));
        CHECK(table.erase("a"));
        CHECK(table.erase("h"));  // Remove the tail from the remaining [o, h].
        CHECK(table.find("o") == std::optional<int>(30));
        CHECK(table.erase("o"));
        CHECK(table.empty());
    });

    tests.run("hash: an empty string is a valid key", [] {
        lab::ChainedHashTable table;
        table.put("", 7);
        CHECK(table.size() == 1);
        CHECK(table.find("") == std::optional<int>(7));
        table.put("", 8);
        CHECK(table.size() == 1);
        CHECK(table.find("") == std::optional<int>(8));
        CHECK(table.erase(""));
        CHECK(table.empty());
    });

    return tests.finish();
}
