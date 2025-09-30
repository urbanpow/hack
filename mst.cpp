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
#define inp(a)        \
    for (auto &i : a) \
        cin >> i;
#define out(a)        \
    for (auto &i : a) \
        cout << i;
using namespace std;

struct DSU
{
    vector<int> p, r;
    DSU(int n)
    {
        p.resize(n);
        r.resize(n, 0);
        iota(all(p), 0);
    }
    int find(int x) { return p[x] == x ? x : p = find(p[x]); }
    bool unite(int a, int b)
    {
        a = find(a);
        b = find(b);
        if (a == b)
            return false;
        if (r[a] < r[b])
            swap(a, b);
        p[b] = a;
        if (r[a] == r[b])
            r[a]++;
        return true;
    }
};

void solve()
{
    int n, m;
    cin >> n >> m;
    vector<array<int, 3>> edges;
    f(i, 0, m, 1)
    {
        int u, v, w;
        cin >> u >> v >> w;
        edges.pb({w, u, v});
    }
    sort(all(edges));
    DSU dsu(n + 1);
    ll mst = 0;
    for (auto &e : edges)
    {
        if (dsu.unite(e[1], e[2]))
            mst += e[0];
    }
    cout << mst;
}

int main()
{
    fastread();
    int t;
    cin >> t;
    while (t--)
    {
        solve();
        cout << endl;
    }
    return 0;
}
