#include <bits/stdc++.h>
using namespace std;

// Abstract Cache class
class Cache {
protected:
    int currentSize;
    int memoryLimit;
public:
    Cache(int limit) : currentSize(0), memoryLimit(limit) {}

    void put(const string& element) {
        int elementSize = element.size();
        if (elementSize > memoryLimit) {
            cout << "Element '" << element << "' is too large for the cache. Skipped.\n";
            return;
        }
        if (has(element)) {
            updateRecency(element);
            cout << "Element '" << element << "' already in cache. Updated recency.\n";
            return;
        }
        while (currentSize + elementSize > memoryLimit && !isEmpty()) {
            evict();
        }
        add(element);
    }

    virtual bool has(const string& element) = 0;
    virtual void updateRecency(const string& element) = 0;
    virtual void add(const string& element) = 0;
    virtual void evict() = 0;
    virtual string get(const string& element) = 0;
    virtual void display() = 0;
    virtual bool isEmpty() = 0;
    virtual ~Cache() = default;

    int getCurrentSize() const { return currentSize; }
};

class RecencyBasedCache : public Cache {
protected:
    list<string> cache;
    unordered_map<string, list<string>::iterator> cacheMap;

    // Template pattern: derived class provides which element to evict
    virtual list<string>::iterator getEvictionIterator() = 0;
    virtual string label() const = 0;

public:
    RecencyBasedCache(int limit) : Cache(limit) {}

    bool has(const string& element) override {
        return cacheMap.find(element) != cacheMap.end();
    }

    void updateRecency(const string& element) override {
        auto it = cacheMap.find(element);
        if (it != cacheMap.end()) {
            string val = *it->second;
            cache.erase(it->second);
            cache.push_front(val);
            cacheMap[element] = cache.begin();
        }
    }

    void add(const string& element) override {
        cache.push_front(element);
        cacheMap[element] = cache.begin();
        currentSize += element.size();
    }

    string get(const string& element) override {
        auto it = cacheMap.find(element);
        if (it != cacheMap.end()) {
            string val = *it->second;
            cache.erase(it->second);
            cache.push_front(val);
            cacheMap[element] = cache.begin();
            return val;
        }
        return "";
    }

    void evict() override {
        if (cache.empty()) return;
        auto it = getEvictionIterator();
        string evicted = *it;
        cacheMap.erase(evicted);
        currentSize -= evicted.size();
        cache.erase(it);
        cout << "Evicted: " << evicted << endl;
    }

    void display() override {
        cout << "Cache (" << label() << "): ";
        for (const auto& item : cache)
            cout << item << " ";
        cout << endl;
    }

    bool isEmpty() override { return cache.empty(); }
};

class LRUCache : public RecencyBasedCache {
public:
    LRUCache(int limit) : RecencyBasedCache(limit) {}
protected:
    list<string>::iterator getEvictionIterator() override {
        return --cache.end(); // Least recently used
    }
    string label() const override { return "LRU"; }
};

class MRUCache : public RecencyBasedCache {
public:
    MRUCache(int limit) : RecencyBasedCache(limit) {}
protected:
    list<string>::iterator getEvictionIterator() override {
        return cache.begin(); // Most recently used
    }
    string label() const override { return "MRU"; }
};

class FIFOCache : public Cache {
private:
    queue<string> cache;
    unordered_map<string, bool> cacheMap;
public:
    FIFOCache(int limit) : Cache(limit) {}

    bool has(const string& element) override {
        return cacheMap.find(element) != cacheMap.end();
    }
    void updateRecency(const string&) override {}
    void add(const string& element) override {
        cache.push(element);
        cacheMap[element] = true;
        currentSize += element.size();
    }
    string get(const string& element) override {
        if (cacheMap.find(element) != cacheMap.end())
            return element;
        return "";
    }
    void evict() override {
        if (cache.empty()) return;
        string evicted = cache.front();
        cache.pop();
        cacheMap.erase(evicted);
        currentSize -= evicted.size();
        cout << "Evicted: " << evicted << endl;
    }
    void display() override {
        cout << "Cache (FIFO): ";
        queue<string> temp = cache;
        while (!temp.empty()) {
            cout << temp.front() << " ";
            temp.pop();
        }
        cout << endl;
    }
    bool isEmpty() override { return cache.empty(); }
};

class LFUCache : public Cache {
private:
    unordered_map<int, list<string>> freqListMap;      // Frequency → List of keys in order of use (oldest at front)
    unordered_map<string, pair<int, list<string>::iterator>> keyMap;     // Key → (frequency, iterator to place in list)

    int minFreq;       // Tracks minimum frequency among all keys for fast access

    // Removes expired or non-existing frequency buckets
    void cleanFreqList(int freq) {
        if (freqListMap[freq].empty())
            freqListMap.erase(freq);
    }

public:
    LFUCache(int limit) : Cache(limit), minFreq(0) {}

    bool has(const string& element) override {
        return keyMap.find(element) != keyMap.end();
    }

    void updateRecency(const string& element) override {
        auto it = keyMap.find(element);
        if (it == keyMap.end()) return;
        int freq = it->second.first;  // Remove from current frequency list
        freqListMap[freq].erase(it->second.second);
        int newFreq = freq + 1;           
        freqListMap[newFreq].push_back(element);  // Add to higher frequency list at the back (most recent at back)
        keyMap[element] = { newFreq, --freqListMap[newFreq].end() };   // Update keyMap with new frequency and iterator
        cleanFreqList(freq);
        if (freqListMap[minFreq].empty()) minFreq++;
    }

    void add(const string& element) override {
        // Insert with freq 1 at back of list (oldest at front)
        freqListMap[1].push_back(element);
        keyMap[element] = { 1, --freqListMap[1].end() };
        currentSize += element.size();
        minFreq = 1;
    }

    string get(const string& element) override {
        if (!has(element)) return "";
        updateRecency(element);
        return element;
    }

    void evict() override {
        if (keyMap.empty()) return;
        // Find key in freqListMap[minFreq] at front (least recently used among min freq)
        auto& candidates = freqListMap[minFreq];
        string evicted = candidates.front();
        candidates.pop_front();
        currentSize -= evicted.size();
        keyMap.erase(evicted);
        cleanFreqList(minFreq);

        // while (minFreq <= INT_MAX && freqListMap[minFreq].empty()) {
        // minFreq++;
        // }


        cout << "Evicted: " << evicted << endl;
        // No need to update minFreq; will be updated in add/updateRecency.
    }

    void display() override {
        cout << "Cache (LFU): ";
        for (auto& pair : freqListMap) {
            int freq = pair.first;
            auto& lst = pair.second;
            for (const auto& key : lst)
                cout << key << "(" << freq << ") ";
        }
    }

    bool isEmpty() override { return keyMap.empty(); }
};

class TTLCache : public Cache {
private:
    unordered_map<string, time_t> cacheMap; // key -> insertion timestamp
    int ttlSeconds;

    // Removes expired keys and updates currentSize accordingly
    void cleanupExpired() {
        time_t now = time(nullptr);
        for (auto it = cacheMap.begin(); it != cacheMap.end();) {
            if ((now - it->second) > ttlSeconds) {
                currentSize -= it->first.size();
                it = cacheMap.erase(it);
            } else {
                ++it;
            }
        }
    }

public:
    TTLCache(int limit, int ttl) : Cache(limit), ttlSeconds(ttl) {}

    bool has(const string& element) override {
        cleanupExpired(); // Ensure up-to-date state
        return cacheMap.find(element) != cacheMap.end();
    }

    void updateRecency(const string&) override {}

    void add(const string& element) override {
        cleanupExpired(); // Remove expired before adding
        cacheMap[element] = time(nullptr);
        currentSize += element.size();
    }

    string get(const string& element) override {
        cleanupExpired();
        if (cacheMap.find(element) != cacheMap.end())
            return element;
        return "";
    }

    void evict() override {
        cleanupExpired();
        if (!cacheMap.empty()) {
            // Find and erase oldest (minimum timestamp)
            auto oldest = cacheMap.begin();
            for (auto it = cacheMap.begin(); it != cacheMap.end(); ++it)
                if (it->second < oldest->second)
                    oldest = it;
            cout << "Evicted: " << oldest->first << endl;
            currentSize -= oldest->first.size();
            cacheMap.erase(oldest);
        }
    }

    void display() override {
        cleanupExpired();
        cout << "Cache (TTL): ";
        for (const auto& p : cacheMap)
            cout << p.first << " ";
        cout << endl;
    }

    bool isEmpty() override {
        cleanupExpired();
        return cacheMap.empty();
    }
};

class RandomCache : public Cache {
private:
    vector<string> elements;
    unordered_map<string, int> idxMap;
public:
    RandomCache(int limit) : Cache(limit) {}

    bool has(const string& element) override {
        return idxMap.find(element) != idxMap.end();
    }
    void updateRecency(const string&) override {}
    void add(const string& element) override {
        elements.push_back(element);
        idxMap[element] = elements.size() - 1;
        currentSize += element.size();
    }
    string get(const string& element) override {
        if (has(element)) return element;
        return "";
    }
    void evict() override {
        if (elements.empty()) return;
        int idx = rand() % elements.size();
        string evicted = elements[idx];
        cout << "Evicted: " << evicted << endl;
        currentSize -= evicted.size();
        int lastIdx = elements.size() - 1;
        if (idx != lastIdx) {
            swap(elements[idx], elements[lastIdx]);
            idxMap[elements[idx]] = idx;
        }
        elements.pop_back();
        idxMap.erase(evicted);
    }
    void display() override {
        cout << "Cache (Random): ";
        for (auto& e : elements)
            cout << e << " ";
        cout << endl;
    }
    bool isEmpty() override { return elements.empty(); }
};

int main() {
    int memoryLimit;
    cout << "Enter the memory limit for the cache in bytes (e.g., 102400 for 100KB): ";
    cin >> memoryLimit;
    if (memoryLimit <= 0) {
        cout << "Memory limit must be positive!" << endl;
        return 1;
    }

    string policy;
    cout << "Enter eviction policy (LRU/FIFO/LFU/MRU/TTL/RANDOM): ";
    cin >> policy;
    Cache* cache = nullptr;
    int ttlValue = 0;
    if (policy == "LRU")
        cache = new LRUCache(memoryLimit);
    else if (policy == "FIFO")
        cache = new FIFOCache(memoryLimit);
    else if (policy == "LFU")
        cache = new LFUCache(memoryLimit);
    else if (policy == "MRU")
        cache = new MRUCache(memoryLimit);
    else if (policy == "TTL") {
        cout << "Enter TTL (seconds) for TTL cache: ";
        cin >> ttlValue;
        cache = new TTLCache(memoryLimit, ttlValue);
    }
    else if (policy == "RANDOM")
        cache = new RandomCache(memoryLimit);
    else {
        cout << "Invalid eviction policy. Exiting." << endl;
        return 1;
    }
    string command, element;
    while (true) {
        cout << "Enter command (put/get/exit): ";
        cin >> command;
        if (command == "exit") break;
        if (command == "put") {
            cin >> element;
            cache->put(element);
        }
        else if (command == "get") {
            cin >> element;
            string result = cache->get(element);
            if (!result.empty()) {
                cout << "Retrieved: " << result << endl;
            } else {
                cout << "Element not found in cache." << endl;
            }
        }
        else {
            cout << "Unknown command." << endl;
        }
        cout << "Current cache size: " << cache->getCurrentSize() << " bytes" << endl;
        cache->display();
    }
    delete cache;
    return 0;
}
