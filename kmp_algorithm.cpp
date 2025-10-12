#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> compute_lps(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;  // Length of the previous longest prefix suffix
    
    lps[0] = 0;  // lps[0] is always 0
    
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    
    return lps;
}

vector<int> kmp_search(const string& text, const string& pattern) {
    vector<int> positions;
    int n = text.length();
    int m = pattern.length();
    
    if (m == 0 || n < m) return positions;
    
    vector<int> lps = compute_lps(pattern);
    
    int i = 0;  // Index for text
    int j = 0;  // Index for pattern
    
    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
            
            if (j == m) {
                // Pattern found at index i-j
                positions.push_back(i - j);
                j = lps[j - 1];
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    
    return positions;
}

int main() {
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    
    vector<int> result = kmp_search(text, pattern);
    
    if (result.empty()) {
        cout << "Pattern not found in the text" << endl;
    } else {
        cout << "Pattern found at positions: ";
        for (int pos : result) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    // Another example with multiple occurrences
    text = "AABAACAADAABAABA";
    pattern = "AABA";
    
    cout << "\nText: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    
    result = kmp_search(text, pattern);
    
    if (result.empty()) {
        cout << "Pattern not found in the text" << endl;
    } else {
        cout << "Pattern found at positions: ";
        for (int pos : result) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    return 0;
}
