// om_namah_shivay
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
#define out(a) for (auto &i : a) cout << i;
using namespace std;

class Node {
public:
    array<Node*,26> nxt;
    bool end;
    Node() {
        nxt.fill(NULL);
        end = false;
    }
};

class Trie {
    Node* root;
public:
    Trie() { root = new Node(); }
    void insert(string s) {
        Node* cur = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!cur->nxt[idx]) cur->nxt[idx] = new Node();
            cur = cur->nxt[idx];
        }
        cur->end = true;
    }
    bool search(string s) {
        Node* cur = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!cur->nxt[idx]) return 0;
            cur = cur->nxt[idx];
        }
        return cur->end;
    }
    bool startsWith(string s) {
        Node* cur = root;
        for (char c : s) {
            int idx = c - 'a';
            if (!cur->nxt[idx]) return 0;
            cur = cur->nxt[idx];
        }
        return 1;
    }
};

void solve() {
    int q; cin >> q;
    Trie t;
    while (q--) {
        int type; string s; cin >> type >> s;
        if (type == 1) t.insert(s);
        else if (type == 2) cout << t.search(s) << " ";
        else cout << t.startsWith(s) << " ";
    }
}

int main() {
    fastread();
    int t; cin >> t;
    while (t--) {
        solve();
        cout << endl;
    }
    return 0;
}
