// BOJ 1006 : 습격자 초라기 , Platinum 3
#include <bits/stdc++.h>
using namespace std;
constexpr int MAX = 987654321;
void FillDP(vector<vector<long long int>>& list, vector<vector<long long int>>& dp,
            long long int n, long long int platoon){
    long long int i;
    if (dp[0][0] == -1){
        dp[0][0] = 0;
        if (list[0][0] + list[0][1] <= platoon){
            dp[0][0] = 1;
        }
        else dp[0][0] = 2;
        dp[0][1] = 1;
        dp[0][2] = 1;
    }
    for(i=1;i<n;i++){
        dp[i][0] = dp[i-1][0] + 2;
        dp[i][1] = dp[i-1][0] + 1;
        dp[i][2] = dp[i-1][0] + 1;
        if (list[i][0] + list[i][1] <= platoon){
            dp[i][0] = min(dp[i][0],dp[i-1][0] + 1);
        }
        if (list[i][0] + list[i-1][0] <= platoon){
            dp[i][1] = min(dp[i][1],dp[i-1][2] + 1);
            dp[i][0] = min(dp[i][0],dp[i][1] + 1);
        }
        if (list[i][1] + list[i-1][1] <= platoon){
            dp[i][2] = min(dp[i][2],dp[i-1][1] + 1);
            dp[i][0] = min(dp[i][0],dp[i][2] + 1);
        }
        if ((list[i][0] + list[i-1][0] <= platoon) && (list[i][1] + list[i-1][1] <= platoon)){
            if (i > 1){
                dp[i][0] = min(dp[i][0],dp[i-2][0] + 2);
            }
            else if (dp[0][1] != MAX && dp[0][2] != MAX){
                dp[i][0] = 2;
            }
        }
    }
    return;
}
void solve(){
    long long int n,i,j,platoon,answer = MAX;
    cin >> n >> platoon;
    vector<vector<long long int>> list(n,vector<long long int>(2,0));
    for(i=0;i<2;i++){
        for(j=0;j<n;j++){
            cin >> list[j][i];
        }
    }
    vector<vector<long long int>> dp(n,vector<long long int>(3,0)),emptydp(n,vector<long long int>(3,0));
    if (list[0][0] + list[n-1][0] <= platoon){
        dp[0][0] = 1;
        dp[0][1] = 0;
        dp[0][2] = MAX;
        FillDP(list,dp,n,platoon);
        answer = min(answer,dp[n-1][2] + 1);
        dp = emptydp;
    }
    if (list[0][1] + list[n-1][1] <= platoon){
        dp[0][0] = 1;
        dp[0][1] = MAX;
        dp[0][2] = 0;
        FillDP(list,dp,n,platoon);
        answer = min(answer,dp[n-1][1] + 1);
        dp = emptydp;
    }
    if (list[0][1] + list[n-1][1] <= platoon && list[0][0] + list[n-1][0] <= platoon && n>1){
        dp[0][0] = 0;
        dp[0][1] = MAX;
        dp[0][2] = MAX;
        FillDP(list,dp,n,platoon);
        answer = min(answer,dp[n-2][0] + 2);
        dp = emptydp;
    }
    if (true){
        dp[0][0] = -1;
        FillDP(list,dp,n,platoon);
        answer = min(answer,dp[n-1][0]);
        dp = emptydp;
    }
    cout << answer << endl;
    return;
}
int main(){
    int testcase;
    cin >> testcase;
    while(testcase--){
        solve();
    }
    return 0;
}

/*
2차원 DP.
앞과 뒤가 연결된 2*n DP.
connection 정보를 미리 작성해도 좋고 DP 런타임에 실시간으로 점검해도 좋다.
Platinum III 급 문제는 아닌 것으로 보임.
*/