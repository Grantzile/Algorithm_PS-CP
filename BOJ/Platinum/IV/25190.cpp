#include <bits/stdc++.h>
using namespace std;
bool compare(pair<int,int>& a, pair<int,int>& b){
    if ((a.first + a.second) == (b.first + b.second)){
        return a < b;
    }
    return (a.first + a.second) < (b.first + b.second);
}
int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int n;
    int corruption = 1234567890;
    int manCount = 0;
    cin >> n;
    vector<pair<int,int>> list(n);
    for(int i = 0 ; i < n; i ++ ){
        cin >> list[i].first >> list[i].second;
    }
    sort(list.begin(), list.end(), compare);
    vector<vector<int>> knapsack(n + 1,vector<int>(4101,-1));
    for(int i = 0 ; i < n + 1; i ++){
        knapsack[i][0] = 0;
    }
    for(int i = 0 ; i < n; i ++ ){
        for(int j = 0 ; j < 4001 ; j ++ ){
            knapsack[i + 1][j] = knapsack[i][j];
        }
        for(int j = 0 ; j < list[i].second ; j ++ ){
            if (knapsack[i][j] < 0) continue;
            knapsack[i + 1][list[i].first + j] = max(knapsack[i + 1][list[i].first + j], knapsack[i][j] + 1);
        }
    }
    
    for(int i = 0; i < 4001 ; i ++ ){
        if (manCount < knapsack[n][i]){
            manCount = knapsack[n][i];
            corruption = i;
        }
    }
    cout << manCount << " " << corruption;
    return 0;
}


/*
Greedy, Sort, DP(Knapsack)
Exchange Argument with knapsack problem
*/