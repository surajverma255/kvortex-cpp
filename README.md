# KVortex (C++)

A simple multithreaded in-memory key-value store built using C++ and TCP sockets.

The goal of this project was to understand how systems like Redis work internally — especially around networking, concurrency, and shared memory.

---

## What it does

* Stores key-value pairs in memory
* Supports basic commands:

  * `SET key value`
  * `GET key`
  * `DEL key`
* Handles multiple clients over TCP
* Uses threads to serve concurrent connections
* Ensures thread safety using a mutex

---

## How it works

At a high level:

* A TCP server listens on a port (default: 8080)
* Each client connection is handled in a separate thread
* Commands are read line-by-line from the socket
* Input is parsed using `stringstream`
* Data is stored in a shared `unordered_map`
* A mutex protects concurrent access

Flow:

Client (nc / telnet / python)
        ↓
TCP Socket (port 8080)
        ↓
TCPServer (accept + thread spawn)
        ↓
Thread (per client)
        ↓
Command Parser (stringstream)
        ↓
KVStore (shared, mutex protected)
        ↓
In-Memory HashMap (unordered_map)

---

## Running the project

### Build

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### Run

```bash
./kv_store.exe
```

### Test (using netcat)

```bash
nc localhost 8080
```

Example:

```
SET name Suraj
GET name
DEL name
```

---

## Design choices

* `unordered_map` for O(1) average access
* `std::thread` (thread-per-connection model)
* `std::mutex` with `lock_guard` for safety
* Line-based protocol (`\n` terminated commands)

I kept the protocol intentionally simple to focus on system behavior rather than parsing complexity.

---

## Limitations

This is not production-ready yet. Known limitations:

* Thread-per-client model does not scale well
* Blocking I/O (each thread waits on `recv`)
* Single global mutex (coarse-grained locking)
* No persistence (data is lost on restart)
* No memory limits or eviction

---

## What I learned

* How TCP sockets behave (especially partial reads)
* Why line-based parsing is needed over raw `recv`
* How shared state works across threads
* The importance of passing references vs copies
* Practical issues like file locks, client behavior, and debugging concurrency

---

## Next improvements

If I continue this:

* Replace thread-per-client with a thread pool
* Introduce finer-grained locking or lock-free structures
* Add persistence (append-only log or snapshot)
* Support better protocol (multi-word values, framing)
* Add benchmarks for throughput/latency

---

## Author

Suraj Verma


<img width="1916" height="1005" alt="image" src="https://github.com/user-attachments/assets/58215108-b3bf-4fd1-9152-a61d17c9ac9a" />
