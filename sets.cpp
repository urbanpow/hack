#include <iostream>
using namespace std;

class Set {
private:
    int* elements;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        int* newElements = new int[capacity];
        for (int i = 0; i < size; ++i)
            newElements[i] = elements[i];
        delete[] elements;
        elements = newElements;
    }

    bool contains(int value) const {
        for (int i = 0; i < size; ++i)
            if (elements[i] == value)
                return true;
        return false;
    }

public:
    Set(int initialCapacity = 10) : size(0), capacity(initialCapacity) {
        elements = new int[capacity];
    }

    ~Set() {
        delete[] elements;
    }

    void insert(int value) {
        if (contains(value)) return;
        if (size == capacity) resize();
        elements[size++] = value;
    }

    void remove(int value) {
        for (int i = 0; i < size; ++i) {
            if (elements[i] == value) {
                elements[i] = elements[size - 1];
                --size;
                return;
            }
        }
    }

    void display() const {
        cout << "{ ";
        for (int i = 0; i < size; ++i)
            cout << elements[i] << " ";
        cout << "}" << endl;
    }

    bool exists(int value) const {
        return contains(value);
    }
};

int main() {
    Set mySet;
    mySet.insert(5);
    mySet.insert(10);
    mySet.insert(5); // Duplicate, won't be added
    mySet.display(); // Output: { 5 10 }
    mySet.remove(10);
    mySet.display(); // Output: { 5 }
    cout << "Contains 5? " << (mySet.exists(5) ? "Yes" : "No") << endl;
}