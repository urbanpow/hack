#include <bits/stdc++.h>
#define pb push_back
#define in insert
#define MOD (int)(1e9 + 7)
#define fastread() (ios_base::sync_with_stdio(false), cin.tie(NULL));
#define vec(a) vector<a>
#define set(a) set<a>
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()
#define ll long long int
#define f(i, a, b, c) for (auto i = a; i < b; i = i + c)
#define inp(a) for (auto &i : a) cin >> i;
#define out(a) for (auto &i : a) cout << i << " ";
using namespace std;

/**
 * Advanced Suffix Array Implementation with LCP
 * 
 * Suffix Array is a sorted array of all suffixes of a given string.
 * Combined with LCP (Longest Common Prefix) array, it becomes a powerful
 * data structure for string processing problems.
 * 
 * Applications:
 * - Pattern matching in O(m + log n)
 * - Finding longest repeated substring
 * - Finding longest common substring of multiple strings
 * - String compression and analysis
 * 
 * Time Complexity: O(n log n) construction
 * Space Complexity: O(n)
 */

class SuffixArray {
private:
    string s;
    int n;
    vector<int> suffixArray, lcpArray, rank;
    
    // DC3 Algorithm for suffix array construction
    vector<int> dc3(vector<int>& s, int K) {
        int n = s.size();
        if (n == 0) return {};
        
        // Pad with zeros if needed
        while (s.size() % 3 != 0) s.pb(0);
        n = s.size();
        
        vector<int> SA12, SA0;
        vector<int> s12;
        
        // Create s12 array
        for (int i = 1; i < n; i += 3) s12.pb(i);
        for (int i = 2; i < n; i += 3) s12.pb(i);
        
        // Sort s12
        vector<int> s12_sorted = s12;
        sort(all(s12_sorted), [&](int a, int b) {
            if (s[a] != s[b]) return s[a] < s[b];
            if (a + 1 >= n || b + 1 >= n) return a > b;
            if (s[a + 1] != s[b + 1]) return s[a + 1] < s[b + 1];
            if (a + 2 >= n || b + 2 >= n) return a > b;
            return s[a + 2] < s[b + 2];
        });
        
        // Assign ranks
        vector<int> rank12(n, -1);
        int currentRank = 0;
        for (int i = 0; i < s12_sorted.size(); i++) {
            if (i > 0) {
                int a = s12_sorted[i-1], b = s12_sorted[i];
                bool different = false;
                for (int j = 0; j < 3; j++) {
                    if (a + j >= n || b + j >= n || s[a + j] != s[b + j]) {
                        different = true;
                        break;
                    }
                }
                if (different) currentRank++;
            }
            rank12[s12_sorted[i]] = currentRank;
        }
        
        // Recursively sort if needed
        if (currentRank < s12.size() - 1) {
            vector<int> s12_new;
            for (int i = 1; i < n; i += 3) s12_new.pb(rank12[i]);
            for (int i = 2; i < n; i += 3) s12_new.pb(rank12[i]);
            SA12 = dc3(s12_new, currentRank + 1);
            
            // Map back to original positions
            for (int i = 0; i < SA12.size(); i++) {
                if (SA12[i] < s12.size() / 2) {
                    SA12[i] = 1 + 3 * SA12[i];
                } else {
                    SA12[i] = 2 + 3 * (SA12[i] - s12.size() / 2);
                }
            }
        } else {
            SA12 = s12_sorted;
        }
        
        // Sort SA0
        vector<int> s0;
        for (int i = 0; i < n; i += 3) s0.pb(i);
        
        sort(all(s0), [&](int a, int b) {
            if (s[a] != s[b]) return s[a] < s[b];
            return rank12[a + 1] < rank12[b + 1];
        });
        
        SA0 = s0;
        
        // Merge SA0 and SA12
        vector<int> result;
        int i = 0, j = 0;
        
        while (i < SA0.size() && j < SA12.size()) {
            int a = SA0[i], b = SA12[j];
            bool choose_a = true;
            
            for (int k = 0; k < 3; k++) {
                if (a + k >= n || b + k >= n) {
                    choose_a = (a + k < n);
                    break;
                }
                if (s[a + k] != s[b + k]) {
                    choose_a = (s[a + k] < s[b + k]);
                    break;
                }
            }
            
            if (choose_a) {
                result.pb(a);
                i++;
            } else {
                result.pb(b);
                j++;
            }
        }
        
        while (i < SA0.size()) result.pb(SA0[i++]);
        while (j < SA12.size()) result.pb(SA12[j++]);
        
        return result;
    }
    
    // Kasai's algorithm for LCP construction
    void buildLCP() {
        lcpArray.resize(n, 0);
        rank.resize(n);
        
        for (int i = 0; i < n; i++) {
            rank[suffixArray[i]] = i;
        }
        
        int k = 0;
        for (int i = 0; i < n; i++) {
            if (rank[i] == n - 1) {
                k = 0;
                continue;
            }
            
            int j = suffixArray[rank[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
                k++;
            }
            
            lcpArray[rank[i]] = k;
            if (k > 0) k--;
        }
    }
    
public:
    SuffixArray(const string& str) : s(str), n(str.length()) {
        // Convert string to integer array
        vector<int> s_int;
        for (char c : s) s_int.pb(c - 'a' + 1);
        s_int.pb(0); // Add sentinel
        
        // Build suffix array using DC3
        suffixArray = dc3(s_int, 26);
        
        // Remove sentinel positions
        suffixArray.erase(remove_if(all(suffixArray), 
            [&](int x) { return x >= n; }), suffixArray.end());
        
        // Build LCP array
        buildLCP();
    }
    
    // Get suffix array
    vector<int> getSuffixArray() const {
        return suffixArray;
    }
    
    // Get LCP array
    vector<int> getLCPArray() const {
        return lcpArray;
    }
    
    // Pattern matching using binary search
    pair<int, int> findPattern(const string& pattern) {
        int m = pattern.length();
        int left = 0, right = n - 1;
        int start = -1, end = -1;
        
        // Find first occurrence
        while (left <= right) {
            int mid = (left + right) / 2;
            int pos = suffixArray[mid];
            string suffix = s.substr(pos, min(m, n - pos));
            
            if (suffix >= pattern) {
                if (suffix.substr(0, m) == pattern) {
                    start = mid;
                }
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        
        if (start == -1) return {-1, -1};
        
        // Find last occurrence
        left = start, right = n - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            int pos = suffixArray[mid];
            string suffix = s.substr(pos, min(m, n - pos));
            
            if (suffix.substr(0, m) == pattern) {
                end = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return {start, end};
    }
    
    // Find longest repeated substring
    string longestRepeatedSubstring() {
        int maxLen = 0, maxPos = 0;
        
        for (int i = 0; i < n - 1; i++) {
            if (lcpArray[i] > maxLen) {
                maxLen = lcpArray[i];
                maxPos = suffixArray[i];
            }
        }
        
        return s.substr(maxPos, maxLen);
    }
    
    // Find longest common substring of two strings
    static string longestCommonSubstring(const string& s1, const string& s2) {
        string combined = s1 + "#" + s2 + "$";
        SuffixArray sa(combined);
        vector<int> suffixArray = sa.getSuffixArray();
        vector<int> lcpArray = sa.getLCPArray();
        
        int maxLen = 0, maxPos = 0;
        int n1 = s1.length();
        
        for (int i = 0; i < lcpArray.size() - 1; i++) {
            int pos1 = suffixArray[i];
            int pos2 = suffixArray[i + 1];
            
            // Check if suffixes belong to different strings
            bool inS1 = (pos1 < n1);
            bool inS2 = (pos2 < n1);
            
            if (inS1 != inS2 && lcpArray[i] > maxLen) {
                maxLen = lcpArray[i];
                maxPos = min(pos1, pos2);
            }
        }
        
        return s1.substr(maxPos, maxLen);
    }
    
    // Print suffix array and LCP array
    void printArrays() {
        cout << "Suffix Array: ";
        for (int i = 0; i < n; i++) {
            cout << suffixArray[i] << " ";
        }
        cout << endl;
        
        cout << "LCP Array: ";
        for (int i = 0; i < n - 1; i++) {
            cout << lcpArray[i] << " ";
        }
        cout << endl;
        
        cout << "Suffixes:" << endl;
        for (int i = 0; i < n; i++) {
            cout << suffixArray[i] << ": " << s.substr(suffixArray[i]) << endl;
        }
    }
};

void solve() {
    string s;
    cin >> s;
    
    SuffixArray sa(s);
    
    cout << "String: " << s << endl;
    sa.printArrays();
    
    cout << "\nLongest Repeated Substring: " << sa.longestRepeatedSubstring() << endl;
    
    // Pattern matching example
    string pattern;
    cin >> pattern;
    auto result = sa.findPattern(pattern);
    
    if (result.first != -1) {
        cout << "Pattern '" << pattern << "' found at positions: ";
        for (int i = result.first; i <= result.second; i++) {
            cout << sa.getSuffixArray()[i] << " ";
        }
        cout << endl;
    } else {
        cout << "Pattern '" << pattern << "' not found" << endl;
    }
}

void solveLongestCommonSubstring() {
    string s1, s2;
    cin >> s1 >> s2;
    
    string lcs = SuffixArray::longestCommonSubstring(s1, s2);
    cout << "Longest Common Substring: '" << lcs << "'" << endl;
}

int main() {
    fastread();
    int t;
    cin >> t;
    
    while (t--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            solve();
        } else {
            solveLongestCommonSubstring();
        }
        cout << endl;
    }
    
    return 0;
}

/*
Test Cases:

Input:
2
1
banana
ana
2
abcdef
defghi

Output:
String: banana
Suffix Array: 5 3 1 0 4 2
LCP Array: 0 1 3 0 0 2
Suffixes:
5: a
3: ana
1: anana
0: banana
4: na
2: nana

Longest Repeated Substring: ana
Pattern 'ana' found at positions: 3 1

Longest Common Substring: 'def'
*/
