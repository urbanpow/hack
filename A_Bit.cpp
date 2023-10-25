#include<bits/stdc++.h>
using namespace std;
void solve(string input,int &x){
    if(input=="X++"){
        x++;
    }
    else if(input=="X--"){
        x--;
    }
    else if(input=="++X"){
        ++x;
    }
    else if(input=="--X"){
        --x;
    }
}
int main()
{
    int n;cin>>n;
    int x=0;
    while(n--){
        string inputd;
        cin>>inputd;
        solve(inputd,x);
    }
    cout<<x<<endl;
return 0;
}