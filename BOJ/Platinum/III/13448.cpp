#include <bits/stdc++.h>
using namespace std;
typedef struct Node{
    long long int score;
    long long int solveTime;
    long long int reduce;
};
bool compare(Node a, Node b){
    return a.solveTime * b.reduce < a.reduce * b.solveTime;
}
int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    long long int n, t;
    cin >> n >> t;
    long long int answer = 0;
    vector<vector<long long int>> knapsack(n+1, vector<long long int>(t+1,0));
    vector<Node> list(n);
    for(int i = 0 ; i < n; i ++ ){
        cin >> list[i].score;
    }
    for(int i = 0 ; i < n; i ++ ){
        cin >> list[i].reduce;
    }
    for(int i = 0 ; i < n; i ++ ){
        cin >> list[i].solveTime;
    }
    sort(list.begin(), list.end(), compare);
    long long int nextValue;
    
    for(long long int index = 0 ; index < n ; index ++ ){
        for(long long int i = 0 ; i < t + 1; i ++ ){
            knapsack[index + 1][i] = knapsack[index][i];
        }
        for(long long int i = 0 ; i + list[index].solveTime < t + 1 ; i ++ ){
        	nextValue = list[index].score - (i + list[index].solveTime) * list[index].reduce;
            knapsack[index + 1][i + list[index].solveTime] = max(knapsack[index + 1][i + list[index].solveTime], knapsack[index][i] + nextValue);
        }
    }
    for(long long int i = 0 ; i < t + 1 ; i ++ ){
    	answer = max(answer, knapsack[n][i]);
    }
    cout << answer;
    return 0;
}

/*
Greedy, Sort, DP(Knapsack)
Exchange Argument with knapsack problem

Little bit harder than BOJ 2123 or BOJ 25190
*/