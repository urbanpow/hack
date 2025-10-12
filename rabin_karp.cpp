#include <iostream>
#include <string>
#include <vector>
using namespace std;

const int PRIME = 101;  // A prime number for hashing

vector<int> rabin_karp_search(const string& text, const string& pattern) {
    vector<int> positions;
    int n = text.length();
    int m = pattern.length();
    
    if (m == 0 || n < m) return positions;
    
    // Calculate hash for pattern and first window of text
    int pattern_hash = 0;
    int text_hash = 0;
    int h = 1;
    
    // The value of h would be "pow(PRIME, m-1) % INT_MAX"
    for (int i = 0; i < m - 1; i++)
        h = (h * 256) % PRIME;
    
    // Calculate hash value for pattern and first window of text
    for (int i = 0; i < m; i++) {
        pattern_hash = (256 * pattern_hash + pattern[i]) % PRIME;
        text_hash = (256 * text_hash + text[i]) % PRIME;
    }
    
    // Slide the pattern over text one by one
    for (int i = 0; i <= n - m; i++) {
        // Check the hash values of current window of text and pattern
        if (pattern_hash == text_hash) {
            // If hash values match, check characters one by one
            int j;
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j])
                    break;
            }
            
            if (j == m)  // Pattern found at index i
                positions.push_back(i);
        }
        
        // Calculate hash value for next window of text
        if (i < n - m) {
            text_hash = (256 * (text_hash - text[i] * h) + text[i + m]) % PRIME;
            
            // We might get negative value of text_hash, converting it to positive
            if (text_hash < 0)
                text_hash = (text_hash + PRIME);
        }
    }
    
    return positions;
}

int main() {
    string text = "ABABCABABDABABCABABCABAB";
    string pattern = "ABABCABAB";
    
    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl;
    
    vector<int> result = rabin_karp_search(text, pattern);
    
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
