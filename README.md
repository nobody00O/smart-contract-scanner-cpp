Smart Contract Scanner (C++)

A small C++ tool that scans Solidity contracts for a couple of common security bugs — reentrancy and unsafe `tx.origin` checks.

I built this to get better at C++ and learn how smart contract vulnerabilities actually look in real code, not just in theory. Not a production tool, just a learning project — but it works.

What it checks for

- Reentrancy — flags `.call{value: ...}`, the pattern behind the DAO hack. Sending money before updating balances is bad news.
- tx.origin — flags contracts using `tx.origin` instead of `msg.sender` for auth checks. Can be exploited through phishing.
- assembly blocks — just flags these for manual review, not necessarily a bug.

Each match adds to a risk score, and you get a LOW / MEDIUM / HIGH rating at the end.

It's basically pattern matching on text, line by line — not a real Solidity parser. So it won't catch everything, and it can flag safe code too. More on that below.

## Running it

```bash
git clone https://github.com/nobody00O/smart-contract-scanner-cpp.git
cd smart-contract-scanner-cpp
make
./scanner contracts/vulnerable_sample.sol
```

Just compile directly:


g++ -std=c++17 -Wall src/main.cpp -o scanner


 Requirements

- C++17 compiler (g++, clang++, or MSVC)

Limitations
*It's just checking for text patterns, so sometimes it flags code that's actually fine. Like if .call{value: ...} comes after the balance is already updated (which is the safe way), it still gets flagged as HIGH risk — the tool has no idea about order, it just sees the pattern and reacts.
It skips // and /* */ comments so those don't get counted as real code, but that's pretty much it. It's not actually reading/understanding Solidity, just scanning text.
*Only looks for these 3 patterns right now. Not even close to a real audit tool, more of a first-pass check.

Why I made this

Wanted something that combined learning C++ with an actual security use case, instead of another generic exercise. Picking real vulnerabilities instead of made-up ones made it feel like something worth building.
