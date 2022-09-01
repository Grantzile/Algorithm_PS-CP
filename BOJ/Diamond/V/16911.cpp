#include <bits/stdc++.h>
using namespace std;

typedef struct Node{
    int parentNode;
    int size;
    Node(): parentNode(-1), size(1){};
};

long long int packer(int x, int y){
    return (long long int)x * 200000 + y;
}

void update(vector<vector<pair<int,int>>>& list, pair<int,int> value, int queryLeft, int queryRight, int left, int right, int root = 1){
    int mid = (left + right) / 2;
    if (queryLeft <= left && right <= queryRight)
    {
        list[root].push_back(value);
        return;
    }
    else if (queryRight < left || right < queryLeft)
    {
        return;
    }
    else
    {
        update(list, value, queryLeft, queryRight, left, mid, root * 2);
        update(list, value, queryLeft, queryRight, mid + 1, right, root * 2 + 1);
        return;
    }
}

int find(vector<Node>& nodes, int root){
    while(nodes[root].parentNode != -1)
    {
        root = nodes[root].parentNode;
    }
    return root;
}

bool Union(vector<Node>& nodes, int left, int right){
    if (nodes[left].size > nodes[right].size)
    {
        nodes[right].parentNode = left;
        nodes[left].size += nodes[right].size;
        return true;
    }
    else
    {
        nodes[left].parentNode = right;
        nodes[right].size += nodes[left].size;
        return false;
    }
}

void Cut(vector<Node>& nodes, int left, int right){
    nodes[left].size -= nodes[right].size;
    nodes[right].parentNode = -1;
    return;
}

void PrintAnswer(vector<vector<pair<int,int>>>& list, vector<Node>& nodes, vector<pair<int,int>>& printQuery, int left, int right, int root = 1){
    /*
	cout << left << " " << right << " !\\" << endl;
    	for(int i = 0 ; i < list[root].size() ; i ++ ){
    		cout << list[root][i].first << " ";
    	}cout << endl;
    	for(int i = 0 ; i < list[root].size() ; i ++ ){
    		cout << list[root][i].second << " ";
    	}cout << endl;
    */
    int mid = (left + right) / 2;
    int rootLeft, rootRight;
    vector<pair<int,int>> getUnion;
    getUnion.reserve(list[root].size());
    for(int i = 0 ; i < list[root].size() ; i ++ ){
        //cout << i << " " << list[root][i].first << " " << list[root][i].second << endl;
        rootLeft = find(nodes, list[root][i].first);
        rootRight = find(nodes, list[root][i].second);
        if ( rootLeft == rootRight )
        {
            continue;
        }
        else
        {
            if (Union(nodes, rootLeft, rootRight))
            {
                getUnion.push_back(make_pair(rootLeft, rootRight));
            }
            else
            {
                getUnion.push_back(make_pair(rootRight, rootLeft));
            }
        }
    }

    if (left == right)
    {
        //cout << printQuery[left - 1].first << " " << printQuery[right - 1].second << endl;
        if ( find(nodes, printQuery[left - 1].first) == find(nodes, printQuery[right - 1].second))
        {
            cout << "1\n";
        }
        else
        {
            cout << "0\n";
        }
    }
    else
    {
        PrintAnswer(list, nodes, printQuery, left, mid, root * 2);
        PrintAnswer(list, nodes, printQuery, mid + 1, right, root * 2 + 1);
    }

    for(int i = getUnion.size() - 1 ; i >= 0 ; i -- ){
        Cut( nodes, getUnion[i].first, getUnion[i].second );
    }
    /*
	cout << left << " " << right << " !\\" << endl;
    	for(int i = 0 ; i < nodes.size() ; i ++ ){
    		cout << nodes[i].parentNode << " ";
    	}cout << endl;
    */
    return;
}

int main(){
    cin.tie(NULL);
    cout.tie(NULL);
    ios_base::sync_with_stdio(false);
    int n, queryCount;
    int queryCode;
    cin >> n >> queryCount;
    vector<pair<int,int>> printQuery;
    vector<bool> isQueryEnded;
    vector<pair<int,int>> connectQuery;
    unordered_map<long long int, pair<int,int>> dupliCheck;
    int left, right;
    int timeStamp = 1;
    int printCount = 0;
    vector<pair<int,pair<int,int>>> queryList(queryCount);


    for(int i = 0 ; i < queryCount; i ++ )
    {
        cin >> queryList[i].first >> queryList[i].second.first >> queryList[i].second.second;
        if (queryList[i].first == 3)
        {
            printCount++;
        }
    }

    vector<vector<pair<int,int>>> list(4*(printCount + 1));

    for(int i = 0 ; i < queryCount; i ++ )
    {
        queryCode = queryList[i].first;
        left = queryList[i].second.first;
        right = queryList[i].second.second;

        if (left > right){
            left ^= right;
            right ^= left;
            left ^= right;
        }
        if (queryCode == 1)
        {
            dupliCheck[packer(left, right)] = make_pair(timeStamp, (int)connectQuery.size());
            isQueryEnded.push_back(false);
            connectQuery.push_back(make_pair(left,right));
        }
        else if (queryCode == 2)
        {
            if ( dupliCheck[packer(left,right)].first > timeStamp - 1){
                continue;
            }
            update(list, make_pair(left, right), dupliCheck[packer(left,right)].first, timeStamp - 1, 1, printCount);
            isQueryEnded[dupliCheck[packer(left, right)].second] = true;
        }
        else
        {
            timeStamp++;
            printQuery.push_back(make_pair(left,right));
        }
    }
    for(int i = 0 ; i < connectQuery.size(); i ++ )
    {
        if (!isQueryEnded[i])
        {
            update(list, connectQuery[i], dupliCheck[packer(connectQuery[i].first, connectQuery[i].second)].first, timeStamp, 1, printCount);
        }
    }

    vector<Node> nodes(n + 1, Node());
    
    PrintAnswer(list, nodes, printQuery, 1, printCount);

    return 0;
}