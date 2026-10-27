#include "log_chain.hpp"
#include "test_support.hpp"

#include <algorithm>
#include <vector>

namespace {

lab::LogChain sample_chain() {
    lab::LogChain chain;
    chain.append("Alice deposited 10");
    chain.append("Bob deposited 20");
    chain.append("Alice withdrew 3");
    return chain;
}

}  // namespace

int main() {
    test::Runner tests;

    tests.run("log: empty chain and trusted initial digest", [] {
        const lab::LogChain chain;
        CHECK(chain.empty());
        CHECK(chain.size() == 0);
        CHECK(chain.final_digest() == lab::LogChain::initial_digest());
        CHECK(lab::LogChain::verify(chain.entries(), chain.final_digest()));
        auto wrong_anchor = lab::LogChain::initial_digest();
        wrong_anchor[0] = 1;
        CHECK(!lab::LogChain::verify(chain.entries(), wrong_anchor));
    });

    tests.run("log: append and verify an intact chain", [] {
        const auto chain = sample_chain();
        CHECK(chain.size() == 3);
        CHECK(chain.entries()[0].record == "Alice deposited 10");
        const auto first = lab::crypto::hash_record(
            lab::LogChain::initial_digest(), "Alice deposited 10");
        const auto second = lab::crypto::hash_record(first, "Bob deposited 20");
        const auto third = lab::crypto::hash_record(second, "Alice withdrew 3");
        CHECK(chain.entries()[0].digest == first);
        CHECK(chain.entries()[1].digest == second);
        CHECK(chain.entries()[2].digest == third);
        CHECK(chain.final_digest() == third);
        CHECK(lab::LogChain::verify(chain.entries(), third));
    });

    tests.run("log: empty and repeated records are allowed", [] {
        lab::LogChain chain;
        chain.append("");
        chain.append("same");
        chain.append("same");
        CHECK(chain.size() == 3);
        CHECK(chain.entries()[0].record.empty());
        CHECK(chain.entries()[1].digest != chain.entries()[2].digest);
        CHECK(lab::LogChain::verify(chain.entries(), chain.final_digest()));
        const auto previous = chain.final_digest();
        const std::string binary_record("a\0b", 3);
        chain.append(binary_record);
        CHECK(chain.entries().back().record == binary_record);
        CHECK(chain.final_digest() == lab::crypto::hash_record(previous, binary_record));
        CHECK(lab::LogChain::verify(chain.entries(), chain.final_digest()));
    });

    tests.run("log: changing a record or a stored digest is detected", [] {
        const auto chain = sample_chain();
        const auto anchor = chain.final_digest();
        auto changed = chain.entries();
        changed[1].record = "Bob deposited 2000";
        CHECK(!lab::LogChain::verify(changed, anchor));
        changed = chain.entries();
        changed[1].digest[0] ^= 1;
        CHECK(!lab::LogChain::verify(changed, anchor));
    });

    tests.run("log: deletion, reordering, and truncation are detected", [] {
        const auto chain = sample_chain();
        const auto anchor = chain.final_digest();
        auto changed = chain.entries();
        changed.erase(changed.begin() + 1);
        CHECK(!lab::LogChain::verify(changed, anchor));
        changed = chain.entries();
        std::swap(changed[0], changed[1]);
        CHECK(!lab::LogChain::verify(changed, anchor));
        changed = chain.entries();
        changed.pop_back();
        CHECK(!lab::LogChain::verify(changed, anchor));
    });

    tests.run("log: a fully recomputed forgery fails the original trusted anchor", [] {
        const auto chain = sample_chain();
        const auto original_anchor = chain.final_digest();
        auto changed = chain.entries();
        changed[0].record = "Alice deposited 10000";
        auto previous = lab::LogChain::initial_digest();
        for (auto& entry : changed) {
            entry.digest = lab::crypto::hash_record(previous, entry.record);
            previous = entry.digest;
        }
        // Internal consistency alone would accept the attacker's new history.
        CHECK(lab::LogChain::verify(changed, previous));
        CHECK(!lab::LogChain::verify(changed, original_anchor));
    });

    return tests.finish();
}
