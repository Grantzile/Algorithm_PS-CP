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
    long long int lazyVal;
    long long int value;
}

long long int diffSum(long long int queryValue, long long int queryDiff, long long int left, long long int right){
    return ((right - left) * (2 * queryValue + (right - left) * queryDiff)) / 2;
}

int TreeInitialize(vector<TreeNode>& list, int root = 1, int parent = -1, int depth = 0){
    list[root].depth = depth;
    list[root].size = 1;
    list[root].parent = parent;
    long long int parentIndex;
    long long int maxWeight = 0;
    long long int subWeight = 0;
    for(int i = 0; i < list[root].edge.size() ; i ++ ){
        if (list[root].edge[i] == parent){
            parentIndex = i;
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

long long int update(vector<Node>& list, int queryValue, int queryDiff, int queryLeft, int queryRight,
                    int left, int right, int root = 1){
    int mid = (left + right) / 2;
    long long int answer = 0;
    if (queryLeft <= left && right <= queryRight){
        list[root].lazyVal += queryValue;
        list[root].lazyDiff += queryDiff;
        answer += diffSum(queryValue, queryDiff, left, right);
        return answer;
    }
    else if ( right < queryLeft || queryRight < left ){
        return 0;
    }
    else{
        answer += update(list, queryValue, queryDiff, queryLeft, queryRight, left, mid, root * 2);
        answer += update(list, queryValue + queryDiff * (mid + 1 - left), queryDiff, queryLeft, queryRight, mid + 1, right , root * 2 + 1);
        list[root].value += answer;
        return answer;
    }
}

long long int search(vector<Node>& list, int index, int left, int right, int root = 1){
    int mid = (left + right) / 2;
    long long int answer = 0;
    list[root].value += diffSum(list[root].lazyValue, list[root].lazyDiff, left, right);
    list[root * 2].lazyValue += list[root].lazyValue;
    list[root * 2 + 1].lazyValue += list[root].lazyValue + (mid + 1 - left) * lazyDiff;
    list[root * 2].lazyDiff += list[root].lazyDiff;
    list[root * 2 + 1].lazyDiff += list[root].lazyDiff;
    list[root].lazyDiff = 0;
    list[root].lazyValue = 0;
    if (left <= index && index <= right){
        answer += list[root].value;
    }
    else {
        return 0;
    }
    if (left != right){
        answer += search(list, index, left, mid, root * 2);
        answer += search(list, index, mid + 1 , right, root * 2 + 1);
    }
    return answer;
}

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    long long int n, origin, destination;
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
    
    
    return 0;
}