#include <bits/stdc++.h>
using namespace std;


constexpr int
	dx[5] = {-1, 0, 1, 0, 0},
	dy[5] = {0, 1, 0, -1, 0};

constexpr int
	ddx[5] = {1, 1, 1, 2, 1},
	ddy[5] = {1, 1, 1, 0, -1}, // 길이 맞춰주기
	ldx[5] = {-1, 0, 1, 1, 2},
	ldy[5] = {-1, -2, -1, -2, -1},
	rdx[5] = {-1, 0, 1, 1, 2},
	rdy[5] = {1, 2, 1, 2, 1};


typedef struct State {
	int x;
	int y;
	int dir;
	State(int _y, int _dir): x(1), y(_y), dir(_dir){};
	void downMove(){
		x ++;
	}
	void leftTurn(){
		x ++;   y --;   dir --;
		if (dir < 0) dir += 4;
	}
	void rightTurn(){
		x ++;   y ++;   dir ++;
		dir %= 4;
	}

	void fixPosition(vector<vector<int>>& maps, function<bool(int, int)>& isPointInBound, int queryIndex){
		for(int i = 0 ; i < 5 ; i ++ ){
            assert(isPointInBound(x + dx[i], y + dy[i])); // 골렘은 영역 밖으로 벗어난 상태에서 fix되려고 시도해서는 안 된다.
            int sign = (i == dir ? -1 : 1);
            maps[x + dx[i]][y + dy[i]] = queryIndex * sign;
		}
	}

	inline bool isAdjacent(int origin, int target){
		if (target == 0) return false;
		if (origin < 0) return true;
		return target == origin || target == -origin;
	}

	int getAnswer(vector<vector<int>>& maps, function<bool(int, int)>& isPointInBound){
        int answer = x - 2;
        queue<pair<int,int>> points;
        points.push(make_pair(x, y));
        vector<vector<bool>> visited(maps.size(), vector<bool>(maps[0].size(), false));
        visited[x][y] = true;
        pair<int,int> here, next;
        int value;
        while(!points.empty()){
            here = points.front();
            value = maps[here.first][here.second];
            points.pop();
            answer = max(answer, here.first - 2);
            for(int i = 0 ; i < 4 ; i ++ ){
                next.first = here.first + dx[i];
                next.second = here.second + dy[i];
                if (!isPointInBound(next.first, next.second))   continue;
                int nextValue = maps[next.first][next.second];
                if (!isAdjacent(value, nextValue)) continue;
                if (visited[next.first][next.second]) continue;
                visited[next.first][next.second] = true;
                points.push(next);
            }
        }
        assert(answer > 0);
		return answer;
	}
} State;


function<bool(int, int)> createIsInBoundFunction(int r, int c) {
	return [r, c](int i, int j) -> bool {
		return i >= 1 && j >= 1 && i < r + 2 && j < c - 1;
	};
}

// 유효한 범위 안인지 확인한다.
// row의 offset 3이 주어짐에 주의한다.
function<bool(int, int)> createIsPointInBoundFunction(int r, int c) {
	return [r, c](int i, int j) -> bool {
		return i >= 3 && j >= 0 && i < r + 3 && j < c;
	};
}


function<bool(vector<vector<int>>&, State)> turnAvailableCheckBuilder(
	function<bool(int, int)> isInBound,
	const int* dxOffset,
	const int* dyOffset,
	int yOffset
){
	return [isInBound, dxOffset, dyOffset, yOffset](vector<vector<int>>& maps, State state) -> bool {
		int x = state.x;
		int y = state.y;
		if (!isInBound(x + 1, y + yOffset)) return false; // !isInBound(x - 1, y)는 x - 1, y + 1 결과에 포함되기에 안 해도 됨.
		for(int i = 0 ; i < 5 ; i ++ ){
			if (maps[x + dxOffset[i]][y + dyOffset[i]] != 0) return false;
		}
		return true;
	};
}

void clearMaps(vector<vector<int>>& list){
	int n = list.size();
	int m = list[0].size();
	for(int i = 0 ; i < n ; i ++ ){
		for(int j = 0 ; j < m ; j ++ ){
			list[i][j] = 0;
		}
	}
	return;
}


int main() {
	int r, c, k;
	int answer = 0;
	cin >> r >> c >> k;
	auto isInBound = createIsInBoundFunction(r, c);
	auto isPointInBound = createIsPointInBoundFunction(r, c);
	auto downMoveAvailable = turnAvailableCheckBuilder(isInBound, ddx, ddy, 0);
	auto leftTurnAvailable = turnAvailableCheckBuilder(isInBound, ldx, ldy, -1);
	auto rightTurnAvailable = turnAvailableCheckBuilder(isInBound, rdx, rdy, 1);

    // offset 3을 주고 시작...
	vector<vector<int>> maps(r + 3, vector<int>(c, 0));

	for(int index = 1; index <= k ; index ++) {
		int pos, dir;
		cin >> pos >> dir;
		State state(pos - 1, dir);
		while(true){
			if (downMoveAvailable(maps, state))         state.downMove(); 
			else if (leftTurnAvailable(maps, state))    state.leftTurn();
			else if (rightTurnAvailable(maps, state))   state.rightTurn();
			else {
				if (state.x <= 3){
					clearMaps(maps);
					break;
				}
				state.fixPosition(maps, isPointInBound, index);
				int localAnswer = state.getAnswer(maps, isPointInBound);
				answer += localAnswer;
				break;
			}
		}
	}
	cout << answer;
	return 0;
}


/*
tag: 교육적인, 구현이 많은, 실수하기 쉬운

간단한 시뮬레이션 + BFS 문제.
사이즈가 늘면 언제든 graph dp로 풀어도 좋다.

연결 상황이 예상치 못하게 변경될 수 있으므로 BFS로 푸는 것이 더 편리하다.

Functional programming 연습용 문제로 좋다.
*/