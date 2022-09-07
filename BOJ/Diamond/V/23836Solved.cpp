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
    long long int lazyDiff;
    long long int lazyValue;
    long long int value;
};

typedef struct UpdateQuery{
    long long int index;
    long long int origin;
    long long int destination;
    bool direction;
    UpdateQuery():index(0), origin(0), destination(0), direction(false){};
    UpdateQuery(long long int index, long long int origin, long long int destination, bool direction): index(index), origin(origin), destination(destination), direction(direction){};
};

long long int diffSum(long long int queryValue, long long int queryDiff, long long int left, long long int right){
    long long int queryLeft = queryValue;
    long long int queryRight = queryValue + queryDiff * (right - left);
    if (queryLeft < 0 || queryRight < 0){
        assert(0);
        return 0;
    }
    else {
        return ((right - left + 1) * (2 * queryValue + (right - left) * queryDiff)) / 2;
    }
}

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

long long int update(vector<Node>& list, long long int queryValue, long long int queryDiff, long long int queryLeft, long long int queryRight,
                    long long int left, long long int right, long long int root = 1){
    long long int mid = (left + right) / 2;
    long long int answer = 0;
    if (queryLeft <= left && right <= queryRight){
        list[root].lazyValue += queryValue + queryDiff * (left - queryLeft);
        list[root].lazyDiff += queryDiff;
        answer += diffSum(queryValue + queryDiff * (left - queryLeft), queryDiff, left, right);
        return answer;
    }
    else if ( right < queryLeft || queryRight < left ){
        return 0;
    }
    else{
        answer += update(list, queryValue, queryDiff, queryLeft, queryRight, left, mid, root * 2);
        answer += update(list, queryValue, queryDiff, queryLeft, queryRight, mid + 1, right , root * 2 + 1); // query 어떻게 들어가지??
        list[root].value += answer;
//cout << "answer is : " << answer << " on left/right : " << left << " " << right << '\n';
        return answer;
    }
}

long long int search(vector<Node>& list, long long int index, long long int left, long long int right, long long int root = 1){
    long long int mid = (left + right) / 2;
    long long int answer = 0;
    if ( right < index || index < left ){
        return 0;
    }
    list[root].value += diffSum(list[root].lazyValue, list[root].lazyDiff, left, right);
    if (left != right){
        list[root * 2].lazyValue += list[root].lazyValue;
        list[root * 2 + 1].lazyValue += list[root].lazyValue + (mid + 1 - left) * list[root].lazyDiff; // 여기가 문제
        list[root * 2].lazyDiff += list[root].lazyDiff;
        list[root * 2 + 1].lazyDiff += list[root].lazyDiff;
    }
    list[root].lazyDiff = 0;
    list[root].lazyValue = 0;
    if (left == right){
        return list[root].value;
    }
    else{
        answer += search(list, index, left, mid, root * 2);
        answer += search(list, index, mid + 1 , right, root * 2 + 1);
    }
    return answer;
}

void setDeliveryUpdate(vector<TreeNode>& list, vector<vector<long long int>>& treeMap,
                       vector<vector<Node>>& pathSegment, long long int origin, long long int destination){
    long long int originProbe = origin, destinationProbe = destination;
    long long int parentNode;
    long long int count = 0;
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
        if (thisQuery.direction){
            update(pathSegment[thisQuery.index], count, 1, thisQuery.origin, thisQuery.destination, 1, (long long int)treeMap[thisQuery.index].size());
            count += thisQuery.destination - thisQuery.origin + 1;
        }
        else{
            count += thisQuery.destination - thisQuery.origin + 1;
            update(pathSegment[thisQuery.index], count - 1, -1, thisQuery.origin, thisQuery.destination, 1, (long long int)treeMap[thisQuery.index].size());
        }
    }
    return;
}

void printDeliveryValue(vector<TreeNode>& list, vector<vector<long long int>>& treeMap, vector<vector<Node>>& pathSegment, long long int queryIndex){
    long long int index = list[queryIndex].pathId;
    long long int segmentIndex = list[queryIndex].segmentIndex;
    cout << search(pathSegment[list[queryIndex].pathId], list[queryIndex].segmentIndex, 1, (long long int)treeMap[list[queryIndex].pathId].size()) << '\n';
    return;
}

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    long long int n, origin, destination;
    cin >> n;
    vector<TreeNode> list(n + 1,TreeNode());
    vector<vector<long long int>> treeMap(1,vector<long long int>(0));
    for(long long int i = 0 ; i < n - 1; i ++ ){
        cin >> origin >> destination;
        list[origin].edge.push_back(destination);
        list[destination].edge.push_back(origin);
    }
    TreeInitialize(list);
    MappingTree(treeMap, list);
    vector<vector<Node>> pathSegment;
    SegtreeInitialize(pathSegment, treeMap);
    long long int queryNumber, queryType, queryIndex;
    cin >> queryNumber;
    for(long long int i = 0 ; i < queryNumber ; i ++ ){
        cin >> queryType;
        if (queryType == 1){
            cin >> origin >> destination;
            setDeliveryUpdate(list, treeMap, pathSegment, origin, destination);
        }
        else if (queryType == 2){
            cin >> queryIndex;
            printDeliveryValue(list, treeMap, pathSegment, queryIndex);
        }
    }
    
    return 0;
}

/*
Comment:
HLD + BOJ 17353
다이아 V 치고는 HLD만 안다면 쉬운 날먹 문제이나,
Heavy - light decomposition 연습에 매우 좋음. 교육적인 문제
*/