// 남극 탐험
#include <bits/stdc++.h>
using namespace std;
typedef struct TreeNode{
    vector<long long int> edge;
    long long int parent;
    long long int depth;
    long long int pathId;
    long long int size;
    long long int heavyEdge;
    long long int segmentIndex;
    TreeNode():depth(-1), pathId(-1), size(1), parent(-1), heavyEdge(-1), segmentIndex(-1), edge(vector<long long int>(0)){};
};

typedef struct Node{
    long long int value;
};

typedef struct UnionFindNode{
    long long int parent;
    long long int size;
    UnionFindNode():parent(-1), size() {};
};

typedef struct UpdateQuery{
    long long int index;
    long long int origin;
    long long int destination;
    bool direction;
    UpdateQuery():index(0), origin(0), destination(0), direction(false){};
    UpdateQuery(long long int index, long long int origin, long long int destination, bool direction): index(index), origin(origin), destination(destination), direction(direction){};
};

typedef struct ExternalQuery{
    long long int queryType;
    long long int first;
    long long int second;
};

long long int TreeInitialize(vector<TreeNode>& list, long long int root = 1, long long int parent = -1, long long int depth = 0){
    list[root].depth = depth;
    list[root].size = 1;
    list[root].parent = parent;
    long long int maxWeight = 0;
    long long int subWeight = 0;
    for(long long int i = 0; i < list[root].edge.size() ; i ++ ){
        if (list[root].edge[i] == parent){
            continue;
        }
        subWeight = TreeInitialize(list, list[root].edge[i], root, depth + 1);
        list[root].size += subWeight;
        if (subWeight > maxWeight){
            maxWeight = subWeight;
            list[root].heavyEdge = i;
        }
    }
    return list[root].size;
}

void MappingTree(vector<vector<long long int>>& treeMap, vector<TreeNode>& list, long long int root = 1, long long int pathId = 0){
//cout << "mapping tree, root : " << root << '\n';
    treeMap[pathId].push_back(root);
    list[root].pathId = pathId;
    list[root].segmentIndex = treeMap[pathId].size();
    for(long long int i = 0 ; i < list[root].edge.size() ; i ++ ){
        if (list[root].edge[i] == list[root].parent){
            continue;
        }
        else if (i == list[root].heavyEdge){
            MappingTree(treeMap, list, list[root].edge[i], pathId);
        }
        else{
            treeMap.push_back(vector<long long int>(0));
            MappingTree(treeMap, list, list[root].edge[i], treeMap.size() - 1);
        }
    }
    return;
}

void SegtreeInitialize(vector<vector<Node>>& pathSegment, vector<vector<long long int>>& treeMap){
    for(long long int i = 0 ; i < treeMap.size() ; i ++ ){
        pathSegment.push_back(vector<Node>(4 * treeMap[i].size(), Node()));
    }
    return;
}

void update(vector<Node>& list, long long int index, long long int value, long long int left, long long int right, long long int root = 1){
    long long int mid = (left + right) / 2;
    long long int answer = 0;
//cout << '\n' << left << " " << right << " & " << list[root].value << '\n';
    if (left <= index && index <= right){
        list[root].value += value;
        if (left != right){
            update(list, index, value, left, mid, root * 2);
            update(list, index, value, mid + 1, right, root * 2 + 1);
        }
        return;
    }
    else{
        return;
    }
    assert(0); // never happens
    return;
}

long long int search(vector<Node>& list, long long int queryLeft, long long int queryRight, long long int left, long long int right, long long int root = 1){
    long long int mid = (left + right) / 2;
    long long int answer = 0;
    if ( queryLeft <= left && right <= queryRight){
        answer += list[root].value;
        return answer;
    }
    else if ( right < queryLeft || queryRight < left){
        return 0;
    }
    else{
        answer += search(list, queryLeft, queryRight, left, mid, root * 2);
        answer += search(list, queryLeft, queryRight, mid + 1, right , root * 2 + 1);
        return answer;
    }
    assert(0); // never happens
    return 0;
}

void setDeliveryUpdate(vector<TreeNode>& list, vector<vector<long long int>>& treeMap,
                       vector<vector<Node>>& pathSegment, long long int queryIndex, long long int value){
    long long int index = list[queryIndex].pathId;
    long long int segmentIndex = list[queryIndex].segmentIndex;
    update(pathSegment[index], segmentIndex, value, 1, (long long int)treeMap[index].size());
    return;
}

void printDeliveryValue(vector<TreeNode>& list, vector<vector<long long int>>& treeMap, vector<vector<Node>>& pathSegment, long long int origin, long long int destination){
    long long int originProbe = origin, destinationProbe = destination;
    long long int parentNode;
    long long int answer = 0;
    UpdateQuery thisQuery;
    queue<UpdateQuery> originSide;
    stack<UpdateQuery> destinationSide;
    while (list[originProbe].pathId != list[destinationProbe].pathId){
        if (list[originProbe].depth - list[originProbe].segmentIndex >= list[destinationProbe].depth - list[destinationProbe].segmentIndex){
            originSide.push(UpdateQuery(list[originProbe].pathId, 1, list[originProbe].segmentIndex, false));
            originProbe = list[treeMap[list[originProbe].pathId][0]].parent;
        }
        else {
            destinationSide.push(UpdateQuery(list[destinationProbe].pathId, 1, list[destinationProbe].segmentIndex, true));
            destinationProbe = list[treeMap[list[destinationProbe].pathId][0]].parent;
        }
    }
    if (list[originProbe].segmentIndex > list[destinationProbe].segmentIndex){
        originSide.push(UpdateQuery(list[originProbe].pathId, list[destinationProbe].segmentIndex, list[originProbe].segmentIndex, false));
    }
    else{
        originSide.push(UpdateQuery(list[originProbe].pathId, list[originProbe].segmentIndex, list[destinationProbe].segmentIndex, true));
    }
    while(!destinationSide.empty()){
        originSide.push(destinationSide.top());
        destinationSide.pop();
    }
    while(!originSide.empty()){
        thisQuery = originSide.front();
        originSide.pop();
        answer += search(pathSegment[thisQuery.index], thisQuery.origin, thisQuery.destination, 1, (long long int)treeMap[thisQuery.index].size());
    }
    cout << answer << '\n';
    return;
}

void Union(vector<UnionFindNode>& unionFindList, long long int left, long long int right){
    while(unionFindList[left].parent != -1){
        left = unionFindList[left].parent;
    }
    while(unionFindList[right].parent != -1){
        right = unionFindList[right].parent;
    }
    if (unionFindList[left].size >= unionFindList[right].size){
        unionFindList[left].size += unionFindList[right].size;
        unionFindList[right].parent = left;
    }
    else{
        unionFindList[right].size += unionFindList[left].size;
        unionFindList[left].parent = right;
    }
    return;
}

long long int find(vector<UnionFindNode>& unionFindList, long long int index){
    long long int prevIndex = index;
    while(unionFindList[index].parent != -1){
        index = unionFindList[index].parent;
    }
    if (prevIndex != index){
        unionFindList[prevIndex].parent = index;
    }
    return index;
}

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    long long int n, origin, destination;
    cin >> n;
    vector<long long int> penguinValue(n + 1, 0);
    for(long long int i = 1 ; i <= n ; i ++ ){
        cin >> penguinValue[i];
    }
    vector<UnionFindNode> unionFindList(n + 1 , UnionFindNode());
    vector<TreeNode> list(n + 1,TreeNode());
    vector<vector<vector<long long int>>> treeMap(1, vector<vector<long long int>>(1,vector<long long int>(0)));
    unordered_map<long long int, long long int> setIndex;
    
    long long int queryNumber, queryType, queryIndex;
    long long int first, second;
    long long int setNumber;
    string queryTypeInput;
    cin >> queryNumber;
    vector<ExternalQuery> queryList(queryNumber);
    vector<vector<vector<Node>>> pathSegment(1);
    for(long long int i = 0 ; i < queryNumber; i ++ ){
        cin >> queryTypeInput >> first >> second;
        if (queryTypeInput == "bridge"){
            queryType = 1;
            if (find(unionFindList, first) != find(unionFindList, second)){
                list[first].edge.push_back(second);
                list[second].edge.push_back(first);
                Union(unionFindList, first, second);
            }
        }
        else if (queryTypeInput == "penguins"){
            queryType = 2;
        }
        else {
            queryType = 3;
        }
        queryList[i].queryType = queryType;
        queryList[i].first = first;
        queryList[i].second = second;
    }
    for(int i = 1 ; i <= n; i ++ ){
        setNumber = find(unionFindList, i);
        if (!setIndex.count(setNumber)){
            setIndex[setNumber] = treeMap.size();
            treeMap.push_back(vector<vector<long long int>>(1,vector<long long int>(0)));
            pathSegment.push_back(vector<vector<Node>>(0));
            TreeInitialize(list, i);
            MappingTree(treeMap.back(), list, i);
            SegtreeInitialize(pathSegment.back(), treeMap.back());
        }
    }
    for(long long int i = 1 ; i <= n ; i ++ ){
        setNumber = find(unionFindList, i);
        setDeliveryUpdate(list, treeMap[setIndex[setNumber]], pathSegment[setIndex[setNumber]], i, penguinValue[i]);
    }
    vector<UnionFindNode> unionFindListRuntime(n + 1 , UnionFindNode());
    for(long long int i = 0 ; i < queryNumber ; i ++ ){ // 바로 Union-find 갈겨도 됨. 근데 연결은 미리 해둬야 함. preprocess 후 확인.
        queryType = queryList[i].queryType;
        first = queryList[i].first;
        second = queryList[i].second;
        if (queryType == 1){
            if (find(unionFindListRuntime, first) != find(unionFindListRuntime, second)){
                Union(unionFindListRuntime, first, second);
                cout << "yes\n";
            }
            else{
                cout << "no\n";
            }
        }
        else if (queryType == 2){
            setNumber = find(unionFindList, first);
            setDeliveryUpdate(list, treeMap[setIndex[setNumber]], pathSegment[setIndex[setNumber]], first, second - penguinValue[first]);
            penguinValue[first] = second;
        }
        else if (queryType == 3){
            if (find(unionFindListRuntime, first) == find(unionFindListRuntime, second)){
                setNumber = find(unionFindList, first);
                printDeliveryValue(list, treeMap[setIndex[setNumber]], pathSegment[setIndex[setNumber]], first, second);
            }
            else{
                cout << "impossible\n";
            }
        }
        
    }
    return 0;
}

/*
Comment:
HLD + Union-find / offline Query / Disjoint set
구현이 빡세다. 그냥 HLD는 구현 태그 붙이는거 의무화하면 안되나?
*/