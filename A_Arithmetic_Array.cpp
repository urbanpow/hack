#include <bits/stdc++.h>
#define pb push_back
#define in insert
#define fastread() (ios_base::sync_with_stdio(false), cin.tie(NULL));
#define vec(a) vector<a>
#define set(a) set<a>
#define all(a) a.begin(), a.end()
#define ll long long int
#define f(i, a, b, c) for (auto i = a; i < b; i = i + c)
using namespace std;
void solve()
{
    int n;cin>>n;
    int s=0;
    f(i,0,n,1){
        int t;cin>>t;
        s+=t;
    }
    if(s<0){
        cout<<abs((n)+(s));
    }
    else if(s==n){
        cout<<0;
    }else{
        cout<<abs(s-n);
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