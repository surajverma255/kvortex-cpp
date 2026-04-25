#include "kv_store.h"

void KVStore::set(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(mtx);
    store[key] = value;
}

std::string KVStore::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);

    auto it = store.find(key);
    if (it != store.end()) {
        return it->second;
    }
    return "NULL";
}

bool KVStore::del(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    return store.erase(key) > 0;
}