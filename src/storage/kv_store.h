#ifndef KV_STORE_H
#define KV_STORE_H

#include <string>
#include <unordered_map>
#include <mutex>

class KVStore {
private:
    std::unordered_map<std::string, std::string> store;
    std::mutex mtx;

public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    bool del(const std::string& key);
};

#endif