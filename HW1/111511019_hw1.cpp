#include <iostream>
#include <queue>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::cerr;
using std::vector; 
using std::queue; 
using std::pair;

enum Type {
	WALKABLE = 0,
	WALL = 1,
	START = 2,
	END = 3
};

struct Position {
	int row;
	int col;

	// C++ doesn't automatically know how to compare custom structs. 
	// Define the comparison operator on myself.
	bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

struct Result {
	int distance;
	long long pathCount;
};

class MazeBFS {

	private: 
		const vector<vector<int>>& maze;
		const int row, col;
		Position start, end;

		bool isValid(Position p) const {
			return p.row >= 0 && p.col >= 0 && p.row < row && p.col < col && maze[p.row][p.col] != WALL;
		}

	public:
		MazeBFS(const vector<vector<int>>& m, Position s, Position e)
			: maze(m), row(m.size()), col(m[0].size()), start(s), end(e) {}
			
		//BFS whole process
		Result solve() {
			vector<vector<int>> dist(row, vector<int>(col, -1));
			vector<vector<long long>> count(row, vector<long long>(col, 0));

			queue<Position> q;
			q.push(start);
			dist[start.row][start.col] = 0;
			count[start.row][start.col] = 1;

			const int DIRECTIONS[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

			while (!q.empty()) {
				Position curr = q.front();
				q.pop();

				// Reach the end point
				if (curr == end) break;

				for (const auto& dir: DIRECTIONS) {
					Position next{curr.row + dir[0], curr.col + dir[1]};

					if(!isValid(next)) continue;

					int newDist = dist[curr.row][curr.col] + 1;

					if (dist[next.row][next.col] == -1) {
						dist[next.row][next.col] = newDist;
						count[next.row][next.col] = count[curr.row][curr.col];
						q.push(next);
					}
					else if (dist[next.row][next.col] == newDist) 
						count[next.row][next.col] += count[curr.row][curr.col];
				}

			}

			return {dist[end.row][end.col], count[end.row][end.col]};
		}
};

static constexpr int DIRECTIONS[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int main() {
	
	// row, column
	int row, col;
	cin >> row >> col;
	
	// initilize the maze
	vector<vector<int>> maze(row, vector<int>(col));

	// start/end points
	Position start = {-1, -1}, end = {-1, -1};

	// read and store the maze and find start/end positions
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cin >> maze[i][j];
			if (maze[i][j] == START) start = {i, j};
			if (maze[i][j] == END) end = {i, j};
		}
	}

	// Check there are start and end point in the maze
	if (start.row == -1 || end.row == -1) {
		cerr << "Error: Start or end point not found\n";
		return 1;
	}

	MazeBFS solver(maze, start, end);
	Result result = solver.solve();

	if (result.distance == -1) cout << "-1 0\n";
	else cout << result.distance << " " << result.pathCount << "\n";

	return 0;
}
