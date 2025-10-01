#include <bits/stdc++.h>
using namespace std;

int kadane(vector<int>& arr) {
    int max_so_far = arr[0];
    int current_max = arr[0];

    for (int i = 1; i < arr.size(); i++) {
        current_max = max(arr[i], current_max + arr[i]); // extend or start new
        max_so_far = max(max_so_far, current_max);       // best so far
    }
    return max_so_far;
}
