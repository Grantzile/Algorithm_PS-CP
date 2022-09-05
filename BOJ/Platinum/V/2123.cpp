#include <bits/stdc++.h>
using namespace std;
bool compare(pair<long long int,long long int>& a, pair<long long int,long long int>& b){
    return (a.first + a.second) > (b.first + b.second);
}
int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    long long int n;
    cin >> n;
    long long int sum = 0;
    long long int answer = -1234567890123LL;
    vector<pair<long long int,long long int>> list(n);
    for(int i = 0 ; i < n; i ++ ){
        cin >> list[i].first >> list[i].second;
        sum += list[i].first;
    }
    sort(list.begin(), list.end(), compare);
    for(int i = 0 ; i < n ; i ++ ){
        sum -= list[i].first;
        answer = max(answer, sum - list[i].second);
    }
    cout << answer;
    return 0;
}

/*
Greedy, Sort
ㄴ Exchange Argument (Typical)
*/