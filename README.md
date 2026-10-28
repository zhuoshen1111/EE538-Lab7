# EE538 — Heaps and Hashing Lab

- Name: Zhuoshen Cheng
- Student ID: 7918905403
- Compiler and version: 16.2.0
- Extra credit attempted: Yes

Read [Assignment.html](Assignment.html) for all requirements and commands.
Complete this file with brief answers and your test results.

## Implementation

1. Heap: In what order does your bottom-up build repair the heap?
   Why is the total cost O(n), rather than O(n log n)?

   Answer: The bottom-up build first copies the input into data_. It then calls sift_down from the last non-leaf node, at index n / 2 - 1, backward to the root. Leaves require no repair, and each node is processed after its children’s subtrees are already heaps.

   The total cost is O(n) because most nodes are near the bottom and can move down only a few levels. About n/4 nodes can move down at most one level, n/8 at most two levels, and so on. The sum of this work is O(n), and copying the input also takes O(n).

2. Hash table: How does a collision differ from a duplicate key?
   How do `put` and `erase` maintain the correct `size_`?

   Answer: A collision occurs when different keys map to the same bucket. These keys remain separate entries in that bucket’s linked list. A duplicate key is a key that already exists in the table; put updates its value rather than creating another entry.

   `put` increments size_ only when inserting a new key. Updating an existing key leaves size_ unchanged. `erase` decrements size_ only when it finds and removes the requested key. If the key is absent, it returns false without changing size_.

## Testing

| Provided checks | Passed / total |
|---|---|
| Required heap and hash-table tests | 11 / 11 |
| Provided SHA-256 helper tests | 4 / 4 |
| Optional log-chain tests | 6 / 6 or not attempted |

Add one of your own cases for each required component. Include the commands
entered in its program, or the operations in a separate C++ API test.

| Your test | Commands / operations | Expected result | Actual result |
|---|---|---|---|
| Heap | `BUILD 5 3 -1 3 0 -1`; execute `POP` five times; `SIZE`; `EMPTY` | POP returns -1, -1, 0, 3, 3 in order; SIZE returns 0; EMPTY returns true. | Matched the expected result. |
| Hash table | Start a fresh session: `PUT "" -1`; `BUCKET ""`; `FIND ""`; `SIZE`; `PUT "" 99`; `FIND ""`; `SIZE`; `ERASE ""`; `FIND ""`; `SIZE` | Outputs: 0, -1, 1, 99, 1, true, NOT_FOUND, 0. | Matched the expected result. |

## Extra credit (if attempted)

Why must the trusted final digest be kept separately from editable history?
What happens if an attacker changes a record and recomputes later digests?

Answer: The trusted final digest must be kept separately from editable history so that an attacker who changes the records cannot also replace the reference used for verification.

If an attacker changes a record without updating its digest, verification detects a mismatch. If the attacker recomputes that digest and all later digests, the rewritten chain may be internally consistent, but its final digest will not match the original trusted final digest. If the attacker can replace the trusted anchor as well, this protection is lost. The chain detects changes against a trusted anchor; it does not prevent edits, encrypt records, or authenticate the writer.

## Known issues and references

List incomplete behavior and any sources used:
No incomplete behavior or known issues were observed in the tests performed. Both required components and the optional log chain were implemented. All 21 provided checks passed, and the additional heap and hash-table cases matched their expected results.