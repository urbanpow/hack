#include <iostream>
#include <list>
#include <vector>
#include <string>

class HashTable {
private:
    static const int TABLE_SIZE = 10; // Adjust size
    std::vector<std::list<std::pair<int, std::string>>> table;

    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

public:
    HashTable() {
        table.resize(TABLE_SIZE);
    }

    // Key-value pair
    void insert(int key, const std::string& value) {
        int index = hashFunction(key);
        for (auto& pair : table[index]) {
            if (pair.first == key) {
                pair.second = value; // Update existing key
                return;
            }
        }
        table[index].emplace_back(key, value); //New key insert
    }

    // Key search
    std::string search(int key) {
        int index = hashFunction(key);
        for (const auto& pair : table[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        return "Key not found";
    }

    // Key remove
    void remove(int key) {
        int index = hashFunction(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                return;
            }
        }
    }

    // Hash table show
    void display() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& pair : table[i]) {
                std::cout << "[" << pair.first << ": " << pair.second << "] ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    HashTable ht;
    ht.insert(1, "Apple");
    ht.insert(11, "Banana"); // Collision with key 1
    ht.insert(21, "Cherry"); // Collision with key 1
    ht.insert(2, "Date");

    ht.display();

    std::cout << "Search key 11: " << ht.search(11) << "\n";

    ht.remove(11);
    std::cout << "After removing key 11:\n";
    ht.display();

    return 0;
}