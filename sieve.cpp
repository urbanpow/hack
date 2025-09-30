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

void solve()
{
    int n = 1000;
    vector<bool> prime(n + 1, true);
    prime[0] = prime[1] = false;

    for (int i = 2; i * i <= n; i++)
    {
        if (prime[i])
        {
            for (int j = i * i; j <= n; j += i)
                prime[j] = false;
        }
    }


    for (int i = 2; i <= n; i++)
    {
        if (prime[i])
            cout << i << " ";
    }

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
