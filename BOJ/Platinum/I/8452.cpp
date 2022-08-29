// BOJ 8452
/*
문제
방향성 있는 그래프 G가 주어진다. 모든 간선의 길이는 1일 때, 당신은 두 가지 쿼리를 처리해야 한다.

간선 하나를 제거한다.
정점 1에서 정점 i 까지의 최단 경로를 출력한다. 경로가 없으면 -1을 출력한다.
입력
첫 번째 줄에 그래프의 정점, 간선의 수와 질의의 수를 나타내는 n, m, q 가 주어진다. (1 ≤ n ≤ 1, 000, 1 ≤ m ≤ 100, 000, 1 ≤ q ≤ 200, 000) 정점들은 순서대로 1부터 n까지 번호가 매겨져 있고, 간선들은 순서대로 1부터 m까지 번호가 매겨져 있다.

이후 m개의 줄로 간선의 정보가 주어진다. i 번째 줄은 간선 i를 나타내며, 두 정수 u, v (1 ≤ u, v ≤ n, u ≠ v) 로 주어진다. 정점 u에서 정점 v로 가는 간선을 의미한다.

이후 q개의 줄에 질의가 순서대로 주어진다. 각각의 질의는 문자 t 와 정수 p 로 주어진다. (t ∈ {U, E})

만약 t = U 일 경우, p 번 간선이 제거된다. 이미 제거된 간선이 다시 제거되는 일은 없다. (1 ≤ p ≤ m)

만약 t = E 일 경우, 1번 정점에서 p번 정점으로 가는 최단 경로의 길이를 출력한다. 간선 하나당 길이가 1이라고 가정한다. 만약 경로가 없으면 -1을 출력한다. (2 ≤ p ≤ n) t = E 인 쿼리가 적어도 1개 있음이 보장된다.

출력
t = E인 질의 마다 1번 정점에서 p번 정점으로 가는 최단 경로의 길이를 한 줄씩 출력한다. 간선 하나당 길이가 1이라고 가정한다. 만약 경로가 없으면 -1을 출력한다. 질의가 주어진 순서대로 출력하라.
*/

#include <bits/stdc++.h>
using namespace std;

constexpr int INF = 987654321;

typedef struct Query{
    char queryType;
    int target;
    int answer;
    Query():answer(INF){};
};

typedef struct Edge{
    pair<int,int> edgeData;
    bool isActive;
    Edge():isActive(true){};
};

typedef struct Node{
    vector<pair<int,int>> edge;
    pair<int,int> distanceValue;
};

void update(){

}

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int nodeNumber, edgeNumber, queryNumber;
    cin >> nodeNumber >> edgeNumber >> queryNumber;
    vector<Edge> edgeList(edgeNumber, Edge());
    vector<Query> queryList(queryNumber, Query());

    for(int i = 0 ; i < edgeNumber ; i ++ ){
        cin >> edgeList[i].edgeData.first >> edgeList[i].edgeData.second;
    } 

    for(int i = 0 ; i < queryNumber ; i ++ ){
        cin >> queryList[i].queryType >> queryList[i].target;
        if (queryList[i].queryType == 'U'){
            edgeList[queryList[i].target - 1].isActive = false;
        }
    }

    

    return 0;
}

/*
Ex input 1 :

7 8 8
1 2
1 3
1 5
2 4
3 1
3 5
4 5
4 6
E 7
E 5
U 7
E 6
E 5
U 2
E 5
E 4

Ex output 1 :

-1
1
3
1
1
2
*/