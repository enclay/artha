# Artha
> **High-performance, decentralized cryptocurrency implementation built with modern C++20.**

Artha (Sanskrit for *wealth/prosperity*) is a proof-of-concept blockchain engineered to explore deterministic state transitions, cryptographic integrity, and real-time peer-to-peer (P2P) synchronization. Unlike high-level implementations, Artha manages its own cryptographic primitives and networking stack from the ground up.

## Table of Contents
1. [Technical Specifications](#technical-specifications)
2. [System Architecture](#system-architecture)
3. [Installation & Build](#installation--build)
4. [Roadmap](#roadmap)


## Technical Specifications

### 1. Core Engineering & State Management
* **Modern C++20 Stack:** Utilizes standard-compliant C++20 features for robust memory management and efficient data structures.
* **State Persistence:** Implements JSON-based filesystem persistence for both the ledger (`chain.json`) and user wallet (`wallet.json`).
* **Modular Block Structure:** Blocks encapsulate transactions, height, nonce, and timestamps, linked via SHA-256 hashes to ensure chain immutability.

### 2. Hardened Cryptographic Layer
* **secp256k1 Integration:** Uses the industry-standard `secp256k1` elliptic curve for asymmetric key generation and ECDSA digital signatures.
* **Transaction Verification:** Every transaction input is cryptographically verified against the sender's public key before being accepted into the mempool.
* **EVP Hashing:** Leverages OpenSSL’s `EVP` interface for high-speed, secure SHA-256 message digesting.



### 3. Asynchronous P2P Networking
* **WebSocket Protocol:** Built on **Boost.Beast** and **Boost.Asio**, providing low-latency, full-duplex communication between nodes.
* **Multi-threaded Server:** Employs an asynchronous socket handler with `std::thread` to manage concurrent peer requests without blocking the mining process.
* **CLI Orchestration:** Features a command-line interface via `boost::program_options` for launching nodes and interacting with the wallet.

---

## System Architecture

The project follows a modular design to isolate core logic from infrastructure:

* **`artha::Blockchain`:** The source of truth; manages the chain and the unconfirmed transaction pool.
* **`artha::Miner`:** Orchestrates the Proof-of-Work (PoW) process with adjustable difficulty and a leading-zero hash validation strategy.
* **`artha::Wallet`:** Manages secret/public keypairs and provides the API for crafting signed transactions.
* **`artha::net`:** A networking namespace handling peer discovery, broadcasting, and WebSocket lifecycle management.

---

## Installation & Build

### 1. Dependencies
Ensure your system has a C++20 compatible compiler, CMake, and the following libraries installed:
* **Boost** (program_options, system, asio)
* **OpenSSL**

### 2. Compilation
```bash
# Clone with recursive submodules (secp256k1 & termcolor)
git clone --recursive https://github.com/enclay/artha.git
cd artha/

# Build project
mkdir build && cd build
cmake ..
make -j$(nproc)

# Start a miner node
./artha mine --port 5001
```
## Roadmap
- [ ] **Persistent Database Migration:** Replace the current JSON-based state persistence with a high-performance key-value store like **LevelDB** or **RocksDB** for $O(1)$ transaction lookups.
- [ ] **Consensus Logic Expansion:** Transition from the current basic Proof-of-Work implementation to a more scalable Byzantine Fault Tolerance (BFT) or Proof-of-Stake (PoS) mechanism.
- [ ] **Dockerization:** Containerize the node environment to facilitate automated multi-node cluster deployment and network stress testing.
- [ ] **Enhanced API:** Implement a RESTful interface to allow external wallet applications to query balances and broadcast transactions.
