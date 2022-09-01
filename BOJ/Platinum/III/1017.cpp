#include <bits/stdc++.h>
using namespace std;
class FlowEdge{
    private:
        bool visited;
        int origin;
        int destination;
        int currentFlow;
        int maxFlow;
    public:
        FlowEdge(int origin, int destination, int maxFlow): origin(origin), destination(destination), maxFlow(maxFlow){};
        int GetLeftFlow(){
            return maxFlow - currentFlow;
        }
        void addFlow(int flow){
            currentFlow += flow;
            return;
        }
        void checkVisit(){
            visited = true;
            return;
        }
        bool isVisited(){
            return visited;
        }
        void resetVisited(){
            visited = false;
            return;
        }
};
class NodeForBipartiteMatching{
    private:
        vector<pair<FlowEdge*, bool>> edgeList;
    public:
        void PushEdge(FlowEdge* newEdge, bool direction){
            edgeList.push_back(make_pair(newEdge, direction));
        }
        int GetEdgeListSize(){
            return edgeList.size();
        }
        int GetEdgeLeftFlow(int index){
            if (edgeList[index].second){
                return edgeList[index].first -> GetLeftFlow();
            }
            else{
                return - ( edgeList[index].first -> GetLeftFlow() );
            }
        }
        FlowEdge* GetEdge(int index){
            return edgeList[index].first;
        }
        void totalReset(){
            for(int i = 0 ; i < edgeList.size() ; i ++ ){
                edgeList[i].first -> resetVisited();
            }
            return;
        }
};

bool trySearch(vector<NodeForBipartiteMatching>& list, int root){
    int nextNode;
    for(int i = 0 ; i < list[root].GetEdgeListSize() ; i ++ ){
        
    }
}

int main(){
    int n;
    vector<bool> isPrime(1001,true);
    isPrime[0] = false;
    isPrime[1] = false;
    for(int i = 2 ; i < 1001; i ++ ){
        if (isPrime[i]){
            for(int j = 2 ; i * j < 1001 ; j ++ ){
                isPrime[i] = false;
            }
        }
    }
    cin >> n;
    FlowEdge* newEdge;
    vector<int> input(n);
    vector<NodeForBipartiteMatching> list(n + 2);

    for(int i = 1 ; i <= n ; i ++ ){
        cin >> input[i];
    }

    for(int i = 1 ; i < n ; i ++ ){
        for(int j = i + 1 ; j <= n; j ++ ){
            if ( isPrime[input[i] + input[j]] ){
                newEdge = new FlowEdge(i, j, 1);
                list[i].PushEdge(newEdge, true);
                list[j].PushEdge(newEdge, false);
            }
        }
    }
    
    for(int i = 1 ; i <= n ; i ++ ){
        newEdge = new FlowEdge(0, i, 1);
        list[0].PushEdge(newEdge, true);
        list[i].PushEdge(newEdge, false);
        newEdge = new FlowEdge(i, n + 1, 1);
        list[i].PushEdge(newEdge, true);
        list[n+1].PushEdge(newEdge, false);
    }
    while(trySearch(list, 0)){
        for(int i = 0 ; i < n + 2 ; i ++ ){
            list[i].totalReset();
        }
    }
    
    return 0;
}

// Introduction to Algorithm.

/*
도입 요약
: 느그 자료구조에서 Big-O notation 같은거 많이 배웠을거다.
  증명하는 법도 많이 배웠을거다.
  토론식 수업으로 가겠다.
  
  느그들 ㅈㄴ 잘풀더라 ㅋㅋ 과제는 어렵게 낼거다. 수고해라.


*/