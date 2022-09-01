#include <bits/stdc++.h>
using namespace std;
class FlowEdge{
    private:
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
};
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
    vector<NodeForBipartiteMatching> list(n);

    for(int i = 0 ; i < n ; i ++ ){
        cin >> input[i];
    }

    for(int i = 0 ; i < n - 1 ; i ++ ){
        for(int j = i + 1 ; j < n; j ++ ){
            if ( isPrime[input[i] + input[j]] ){
                newEdge = new FlowEdge(i, j, 1);
                list[i].PushEdge(newEdge, true);
                list[j].PushEdge(newEdge, false);
            }
        }
    }
    
    return 0;
}