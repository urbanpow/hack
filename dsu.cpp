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

class DSU {
    vec(int) p, r;
public:
    DSU(int n) {
        p.resize(n+1);
        r.assign(n+1,0);
        iota(all(p),0);
    }
    int find(int x) {
        return p[x]==x?x:p[x]=find(p[x]);
    }
    bool unite(int a,int b) {
        a=find(a); b=find(b);
        if(a==b) return 0;
        if(r[a]<r[b]) swap(a,b);
        p[b]=a;
        if(r[a]==r[b]) r[a]++;
        return 1;
    }
};

void solve() {
    int n,m; cin >> n >> m;
    DSU d(n);
    while(m--){
        int t,u,v; cin >> t >> u >> v;
        if(t==1) d.unite(u,v);
        else cout << (d.find(u)==d.find(v)) << " ";
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
