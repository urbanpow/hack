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
#define out(a) for (auto &i : a) cout << i << " ";
using namespace std;

const int N = 1005;
vec(int) g[N];
int sz[N], depth[N];

void dfs(int u, int p) {
    sz[u] = 1;
    depth[u] = (p == -1 ? 0 : depth[p] + 1);
    for (int v : g[u]) {
        if (v == p) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}

void solve() {
    int n; cin >> n;
    f(i,0,n+1,1) g[i].clear();
    f(i,0,n-1,1) {
        int u,v; cin >> u >> v;
        g[u].pb(v); g[v].pb(u);
    }
    dfs(1,-1);
    out(sz+1, sz+1+n); cout << endl;
    out(depth+1, depth+1+n);
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
