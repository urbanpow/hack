#include <iostream>
using namespace std;

class Random {
private:
    unsigned int seed;

public:
    Random(unsigned int s = 1) : seed(s) {}
    int next(int max) {
        seed = (1664525 * seed + 1013904223);
        return (seed >> 16) % max;
    }

    int next(int min, int max) {
        return min + next(max - min + 1);
    }
};

int main() {
    Random rng(12345);

    for (int i = 0; i < 10; ++i) {
        cout << "Random number: " << rng.next(1, 100) << endl;
    }

    return 0;
}
