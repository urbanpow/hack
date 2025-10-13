#include <iostream>
using namespace std;

class Random {
private:
    unsigned int seed;

public:
    Random(unsigned int s = 1) : seed(s) {}

    // Generates a pseudo-random number between 0 and max-1
    int next(int max) {
        seed = (1664525 * seed + 1013904223); // LCG parameters
        return (seed >> 16) % max;
    }

    // Generates a pseudo-random number between min and max (inclusive)
    int next(int min, int max) {
        return min + next(max - min + 1);
    }
};

int main() {
    Random rng(12345); // You can change the seed

    for (int i = 0; i < 10; ++i) {
        cout << "Random number: " << rng.next(1, 100) << endl;
    }

    return 0;
}