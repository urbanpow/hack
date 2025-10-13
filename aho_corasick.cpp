#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
using namespace std;

class AhoCorasick {
    struct TrieNode {
        map<char, int> children;
        int fail_link = -1;
        int output_link = -1;
        vector<int> pattern_indices;
    };

    vector<TrieNode> trie;
    vector<string> patterns;

public:
    AhoCorasick() {
        // Initialize with root node
        trie.emplace_back();
    }

    void add_pattern(const string& pattern, int pattern_index) {
        int node = 0;
        for (char c : pattern) {
            if (trie[node].children.find(c) == trie[node].children.end()) {
                trie[node].children[c] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].children[c];
        }
        trie[node].pattern_indices.push_back(pattern_index);
    }

    void build_automaton() {
        queue<int> q;
        
        // Initialize fail links for root's children
        for (auto& [c, child] : trie[0].children) {
            trie[child].fail_link = 0;
            q.push(child);
        }

        // BFS to set fail links
        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (auto& [c, child] : trie[node].children) {
                int fail = trie[node].fail_link;
                
                while (fail != -1 && trie[fail].children.find(c) == trie[fail].children.end()) {
                    fail = trie[fail].fail_link;
                }
                
                if (fail == -1) {
                    trie[child].fail_link = 0;
                } else {
                    trie[child].fail_link = trie[fail].children[c];
                }
                
                // Set output link
                if (!trie[trie[child].fail_link].pattern_indices.empty()) {
                    trie[child].output_link = trie[child].fail_link;
                } else {
                    trie[child].output_link = trie[trie[child].fail_link].output_link;
                }
                
                q.push(child);
            }
        }
    }

    vector<vector<int>> search(const string& text) {
        vector<vector<int>> results(patterns.size());
        int current = 0;

        for (int i = 0; i < text.size(); ++i) {
            char c = text[i];
            
            while (current != 0 && trie[current].children.find(c) == trie[current].children.end()) {
                current = trie[current].fail_link;
            }
            
            if (trie[current].children.find(c) != trie[current].children.end()) {
                current = trie[current].children[c];
                
                // Check for matches at current node
                int temp = current;
                while (temp != -1) {
                    for (int pattern_idx : trie[temp].pattern_indices) {
                        results[pattern_idx].push_back(i - patterns[pattern_idx].length() + 1);
                    }
                    temp = trie[temp].output_link;
                }
            }
        }
        
        return results;
    }

    static AhoCorasick build(const vector<string>& patterns) {
        AhoCorasick ac;
        ac.patterns = patterns;
        
        for (int i = 0; i < patterns.size(); ++i) {
            ac.add_pattern(patterns[i], i);
        }
        
        ac.build_automaton();
        return ac;
    }
};

int main() {
    string text = "ushers";
    vector<string> patterns = {"he", "she", "his", "hers"};
    
    cout << "Text: " << text << endl;
    cout << "Patterns: ";
    for (const auto& p : patterns) {
        cout << '"' << p << '"' << " ";
    }
    cout << "\n\n";
    
    AhoCorasick ac = AhoCorasick::build(patterns);
    auto results = ac.search(text);
    
    for (int i = 0; i < patterns.size(); ++i) {
        cout << "Pattern '" << patterns[i] << "' found at positions: ";
        for (int pos : results[i]) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    return 0;
}
