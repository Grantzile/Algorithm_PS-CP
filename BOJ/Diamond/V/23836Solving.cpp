#include <bits/stdc++.h>
using namespace std;
typedef struct TreeNode{
    vector<int> edge;
    int parent;
    int depth;
    int pathId;
    int size;
    int heavyEdge;
    int segmentIndex;
    TreeNode():depth(-1), pathId(-1), size(1), parent(-1), heavyEdge(-1), segmentIndex(-1), edge(vector<int>(0)){};
};

typedef struct Node{
    int lazyDiff;
    int lazyValue;
    int value;
};

typedef struct UpdateQuery{
    int index;
    int origin;
    int destination;
    bool direction;
    UpdateQuery():index(0), origin(0), destination(0), direction(false){};
    UpdateQuery(int index, int origin, int destination, bool direction): index(index), origin(origin), destination(destination), direction(direction){};
};

int diffSum(int queryValue, int queryDiff, int left, int right){
    int queryLeft = queryValue;
    int queryRight = queryValue + queryDiff * (right - left);
    if (queryLeft < 0 && queryRight < 0){
        return 0;
    }
    else if ( queryRight < 0 ){
        return (queryLeft * (queryLeft - 1)) / 2;
    }
    else if ( queryLeft < 0 ){
        return (queryRight * (queryRight - 1)) / 2;
    }
    else {
        return ((right - left + 1) * (2 * queryValue + (right - left) * queryDiff)) / 2;
    }
}

int TreeInitialize(vector<TreeNode>& list, int root = 1, int parent = -1, int depth = 0){
    list[root].depth = depth;
    list[root].size = 1;
    list[root].parent = parent;
    int maxWeight = 0;
    int subWeight = 0;
    for(int i = 0; i < list[root].edge.size() ; i ++ ){
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

void MappingTree(vector<vector<int>>& treeMap, vector<TreeNode>& list, int root = 1, int pathId = 0){
    treeMap[pathId].push_back(root);
    list[root].pathId = pathId;
    list[root].segmentIndex = treeMap[pathId].size();
    for(int i = 0 ; i < list[root].edge.size() ; i ++ ){
        if (list[root].edge[i] == list[root].parent){
            continue;
        }
        else if (i == list[root].heavyEdge){
            MappingTree(treeMap, list, list[root].edge[i], pathId);
        }
        else{
            treeMap.push_back(vector<int>(0));
            MappingTree(treeMap, list, list[root].edge[i], treeMap.size() - 1);
        }
    }
    return;
}

void SegtreeInitialize(vector<vector<Node>>& pathSegment, vector<vector<int>>& treeMap){
    for(int i = 0 ; i < treeMap.size() ; i ++ ){
        pathSegment.push_back(vector<Node>(4 * treeMap[i].size(), Node()));
    }
    return;
}

int update(vector<Node>& list, int queryValue, int queryDiff, int queryLeft, int queryRight,
                    int left, int right, int root = 1){
//cout << root << " !\n";
    int mid = (left + right) / 2;
    int answer = 0;
    if (queryLeft <= left && right <= queryRight){
        list[root].lazyValue += queryValue + queryDiff * (left - queryLeft);
        list[root].lazyDiff += queryDiff;
        answer += diffSum(queryValue + queryDiff * (left - queryLeft), queryDiff, left, right);
//cout << diffSum(queryValue + queryDiff * (left - queryLeft), queryDiff, left, right) << " ?\n";
//cout << "queryValue + queryDiff * (left - queryLeft) : " <<queryValue + queryDiff * (left - queryLeft) << '\n';  
//cout << "queryDiff : " << queryDiff << '\n';  
//cout << "answer is : " << answer << " on left/right : " << left << " " << right << '\n';
        return answer;
    }
    else if ( right < queryLeft || queryRight < left ){
        return 0;
    }
    else{
        answer += update(list, queryValue, queryDiff, queryLeft, queryRight, left, mid, root * 2);
         answer += update(list, queryValue + queryDiff, queryDiff, queryLeft, queryRight, mid + 1, right , root * 2 + 1); // sibal
        list[root].value += answer;
//cout << "answer is : " << answer << " on left/right : " << left << " " << right << '\n';
        return answer;
    }
}

int search(vector<Node>& list, int index, int left, int right, int root = 1){
    int mid = (left + right) / 2;
    int answer = 0;
    if ( right < index || index < left ){
        return 0;
    }
    list[root].value += diffSum(list[root].lazyValue, list[root].lazyDiff, left, right);
    list[root * 2].lazyValue += list[root].lazyValue;
    list[root * 2 + 1].lazyValue += list[root].lazyValue + (mid + 1 - left) * list[root].lazyDiff; // 여기가 문제
    list[root * 2].lazyDiff += list[root].lazyDiff;
    list[root * 2 + 1].lazyDiff += list[root].lazyDiff;
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

void setDeliveryUpdate(vector<TreeNode>& list, vector<vector<int>>& treeMap,
                       vector<vector<Node>>& pathSegment, int origin, int destination){
    int originProbe = origin, destinationProbe = destination;
    int parentNode;
    int count = 0;
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
        //cout << boolalpha << thisQuery.direction << " " << thisQuery.origin << " " << thisQuery.destination << " " << thisQuery.index << endl;
        if (thisQuery.direction){
            update(pathSegment[thisQuery.index], count, 1, thisQuery.origin, thisQuery.destination, 1, (int)treeMap[thisQuery.index].size());
//cout << count << " on : " << thisQuery.origin << " " << thisQuery.destination << '\n';
            count += thisQuery.destination - thisQuery.origin + 1;
        }
        else{
            count += thisQuery.destination - thisQuery.origin + 1;
//cout << count -1 << " on : " << thisQuery.origin << " " << thisQuery.destination << '\n';
            update(pathSegment[thisQuery.index], count - 1, -1, thisQuery.origin, thisQuery.destination, 1, (int)treeMap[thisQuery.index].size());
        }
    }
    return;
}

void printDeliveryValue(vector<TreeNode>& list, vector<vector<int>>& treeMap, vector<vector<Node>>& pathSegment, int queryIndex){
    int index = list[queryIndex].pathId;
    int segmentIndex = list[queryIndex].segmentIndex;
    cout << search(pathSegment[list[queryIndex].pathId], list[queryIndex].segmentIndex, 1, (int)treeMap[list[queryIndex].pathId].size()) << '\n';
    return;
}

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int n, origin, destination;
    cin >> n;
    vector<TreeNode> list(n + 1,TreeNode());
    vector<vector<int>> treeMap(1,vector<int>(0));
    for(int i = 0 ; i < n - 1; i ++ ){
        cin >> origin >> destination;
        list[origin].edge.push_back(destination);
        list[destination].edge.push_back(origin);
    }
    TreeInitialize(list);
    MappingTree(treeMap, list);
    vector<vector<Node>> pathSegment;
    SegtreeInitialize(pathSegment, treeMap);
    int queryNumber, queryType, queryIndex;
    cin >> queryNumber;
    for(int i = 0 ; i < queryNumber ; i ++ ){
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