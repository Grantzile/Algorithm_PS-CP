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
        pair<int,int> GetEdgeEndPoint(){
            return make_pair(origin, destination);
        }
};
class NodeForBipartiteMatching{
    private:
        vector<pair<FlowEdge*, bool>> edgeList;
        bool visited;
    public:
        int fillEdgeIndex; // addon
        NodeForBipartiteMatching(): fillEdgeIndex(-1), visited(false){};
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
        pair<FlowEdge*, bool> GetEdge(int index){
            return edgeList[index];
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

bool trySearch(vector<NodeForBipartiteMatching>& list, int root, int index = 0){
	cout << index << " " << root << endl;
    int nextNode;
    bool flowType;
    pair<FlowEdge*, bool> edgeData;
    pair<int,int> edgeEndPoint;
    if (root == list.size() - 1){
    	cout << "fin!!" << endl;
        return true;
    }
    if (list[root].isVisited()){
        return false;
    }
    list[root].checkVisit();
    for(int i = 0 ; i < list[root].GetEdgeListSize() ; i ++ ){
        edgeData = list[root].GetEdge(i);
        if (!edgeData.first -> GetLeftFlow()){
            continue;
        }
        edgeEndPoint = edgeData.first -> GetEdgeEndPoint();
        if (edgeData.second){
            flowType = true;
            edgeData.first -> addFlow(1);
            nextNode = edgeEndPoint.second;
            list[root].fillEdgeIndex = i;
        }
        else{
            flowType = false;
            edgeData.first -> addFlow(-1);
            nextNode = edgeEndPoint.first;
            list[nextNode].fillEdgeIndex = -1;
        }
        if (nextNode == list.size() - 1 && index % 2 != 0){
            cout << "error" << endl;
            continue;
        }
        if (trySearch(list, nextNode, index + 1)){
            return true;
        }
        if (flowType){
            edgeData.first -> addFlow(-1);
            nextNode = -1;
        }
        else{

        }

        
    }
    return false;
}
void deleteTrace(vector<NodeForBipartiteMatching>& list, int root){
    if (root == list.size() - 1){
        return;
    }
    int fillEdgeIndex = list[root].fillEdgeIndex;
    list[root].GetEdge(fillEdgeIndex).first -> addFlow(-1);
    list[root].fillEdgeIndex = -1;
    deleteTrace(list, list[root].GetEdge(fillEdgeIndex).first -> GetEdgeEndPoint().second );
    return;
}

int getFirstMatching(vector<NodeForBipartiteMatching>& list){
    list[0].GetEdge(0).first -> addFlow(-1);
    int edgeIndexToAnotherNode = list[1].fillEdgeIndex;
    auto edgeToNextNode = list[1].GetEdge(edgeIndexToAnotherNode);
    int answer = edgeToNextNode.first -> GetEdgeEndPoint().second;
    return answer;
}

int main(){
	cin.tie(NULL);
	cout.tie(NULL);
	ios_base::sync_with_stdio(false);
    int n;
    int count = 0;
    vector<bool> isPrime(2001,true);
    vector<int> successTry;
    isPrime[0] = false;
    isPrime[1] = false;
    for(int i = 2 ; i < 2001; i ++ ){
        if (isPrime[i]){
            for(int j = 2 ; i * j < 2001 ; j ++ ){
                isPrime[i*j] = false;
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
cout << "sibal" << endl;
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
        count++;
        for(int i = 0 ; i < n + 2 ; i ++ ){
            list[i].resetVisited();
        }
    }
    if (count != n){
        cout << "-1";
        return 0;
    }

    do{
        successTry.push_back(getFirstMatching(list));
        for(int i = 0 ; i < n + 2 ; i ++ ){
            list[i].resetVisited();
        }
    }while(trySearch(list,0));

    sort(successTry.begin(), successTry.end());
    for(int i = 0 ; i < successTry.size() ; i ++ ){
        cout << successTry[i] << " ";
    }
    return 0;
}
