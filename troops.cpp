#include <bits/stdc++.h>
using namespace std;

class Move {
 public:
  pair<int, int> ori, tar, shoot;

  Move() {}

  Move(pair<int, int> ori, pair<int, int> tar, pair<int, int> shoot): ori(ori), tar(tar), shoot(shoot) {}

};

int nodes;
Move bestmove;

class Board {
 public:
  int arr[8][8];
  int currentPlayer = -1;

  static vector<Move> allMoves;

  static bool outOfRange(pair<int, int> loc) {
  	return loc.first < 0 || loc.first >= 8 || loc.second < 0 || loc.second >= 8;
  }

  static bool isBuilding(pair<int, int> loc) {
  	return loc == make_pair(0, 0) || loc == make_pair(0, 7) || loc == make_pair(7, 0) || loc == make_pair(7, 7);
  }

  static void precomputeMoves() {
    vector<Move> tmp;
  	for (int i = 0; i < 8; ++i) {
  		for (int j = 0; j < 8; ++j) {
  			pair<int, int> ori = {i, j};
  			if (Board::isBuilding(ori)) continue;
  			for (int dr = -1; dr <= 1; ++dr) {
  				for (int dc = -1; dc <= 1; ++dc) {
  					if (dr == 0 && dc == 0) continue;
  					pair<int, int> tar = {i + dr, j + dc};
  					if (Board::outOfRange(tar)) continue;
  					if (Board::isBuilding(tar)) continue;
  					for (int sr = -1; sr <= 1; ++sr) {
  						for (int sc = -1; sc <= 1; ++sc) {
  							pair<int ,int> shoot = {tar.first + sr, tar.second + sc};
  							if (Board::outOfRange(shoot)) continue;
                if (sr == 0 && sc == 0) tmp.push_back(Move(ori, tar, shoot));
  							else Board::allMoves.push_back(Move(ori, tar, shoot));
  						}
  					}
  				}
  			}
  		}
  	}
    for (auto i : tmp) Board::allMoves.push_back(i);
  	printf("%d\n", (int)Board::allMoves.size());
  }

  Board() {
    memset(arr, 0, sizeof(arr));
    if (allMoves.empty()) Board::precomputeMoves();
  }

  Board(Board* board) {
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        arr[i][j] = board -> arr[i][j];
      }
    }
    currentPlayer = board -> currentPlayer;
  }

  static Board* startpos() {
    Board* res = new Board();
    for (int i = 0; i < 8; ++i) res -> arr[0][i] = res -> arr[1][i] = -1, res -> arr[6][i] = res -> arr[7][i] = 1;
    return res;
  }

  void move(Move move) {
    arr[move.shoot.first][move.shoot.second] = 0;
    arr[move.tar.first][move.tar.second] = arr[move.ori.first][move.ori.second];
    arr[move.ori.first][move.ori.second] = 0;
    currentPlayer *= -1;
  }

  void print() {
    printf("\n");
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j)
        printf("%c ", arr[i][j] == -1 ? 'w' : arr[i][j] == 1 ? 'b' : '.');
      printf("\n");
    }
    printf("\n");
  }

  vector<Move> getValidMoves() {
  	vector<Move> res;
  	for (auto move : Board::allMoves) {
  		if (arr[move.ori.first][move.ori.second] != currentPlayer) continue;
  		if (arr[move.tar.first][move.tar.second] != 0) continue;
  		if (move.shoot != move.tar && arr[move.shoot.first][move.shoot.second] != -currentPlayer) continue;
  		if (move.shoot == move.tar) {
  			bool valid = true;
  			for (int i = -1; i <= 1; ++i) {
  				for (int j = -1; j <= 1; ++j) {
  					pair<int, int> loc = {move.tar.first + i, move.tar.second + j};
  					if (Board::outOfRange(loc)) continue;
  					if (arr[loc.first][loc.second] == -currentPlayer) valid = false;
  				}
  			}
  			if (!valid) continue;
  		}
  		if (arr[move.shoot.first][move.shoot.second] == currentPlayer) continue;
  		res.push_back(move);
  	}
  	return res;
  }

  void randomMove() {
    vector<Move> validMoves = getValidMoves();
    int rnd = rand() % validMoves.size();
    move(validMoves[rnd]);
  }

  void greedyMove() {
    vector<Move> validMoves = getValidMoves();

    // check ending game
    for (auto move : validMoves) {
      Board* nextBoard = new Board(this);
      nextBoard -> move(move);
      if (nextBoard -> getGameEnded() == currentPlayer) {
        this -> move(move);
        return;
      }
    }
    randomMove();
  }

  int getGameEnded() {
    if (arr[0][0] != -1 || arr[0][7] != -1) return 1;
    if (arr[7][0] != 1 || arr[7][7] != 1) return -1;
    int cntA = 0, cntB = 0;
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) if (arr[i][j] == -1) cntA++; else if (arr[i][j] == 1) cntB++;
    if (cntA == 2) return 1;
    if (cntB == 2) return -1;
    return 2;
  }

  int getDiff() {
    int res = 0;
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) res += arr[i][j];
    return res;
  }

  int getHeuristics() {
    int sum = 0;
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) {
      if (arr[i][j] == 1) sum += 7 - i;
      else if (arr[i][j] == -1) sum -= i;
    }
    return sum;
  }

  int getValue() {
    return getDiff() * 1000000 + getHeuristics();
  }

  static int alphabeta(Board* board, int depth, int alpha, int beta, int player) {
    ++nodes;
    int gameEnded = board -> getGameEnded();
    if (gameEnded != 2) return gameEnded * 1000000000;
    if (depth == 0) return board -> getValue();
    Move which;
    bool ada = false;
    assert((board -> getValidMoves()).size());
    if (player == -1) {
      int res = 1000000006;
      for (auto move : board -> getValidMoves()) {
        Board* child = new Board(board);
        child -> move(move);
        res = min(res, alphabeta(child, depth - 1, alpha, beta, 1));
        if (res < beta) {
          beta = res;
          which = move;
          ada = true;
        }
        if (alpha >= beta) break;
      }
      if (ada) board -> move(which);
      bestmove = which;
      return res;
    } else {
      int res = -1000000006;
      for (auto move : board -> getValidMoves()) {
        Board* child = new Board(board);
        child -> move(move);
        res = max(res, alphabeta(child, depth - 1, alpha, beta, -1));
        if (res > alpha) {
          alpha = res;
          which = move;
          ada = true;
        }
        if (alpha >= beta) break;
      }
      if (ada) board -> move(which);
      bestmove = which;
      return res;
    }
  }

  void alphabeta(int depth) {
    nodes = 0;
    Board::alphabeta(this, depth, -1000000006, 1000000006, currentPlayer);
    printf("nodes: %d\n", nodes);
  }

};

vector<Move> Board::allMoves = vector<Move>();

Board* currentBoard = Board::startpos();

vector<string> split(string s) {
  vector<string> res;
  istringstream is(s);
  while (!is.eof()) {
    is >> s;
    res.push_back(s);
  }
  return res;
}

void startpos() {
  currentBoard = Board::startpos();
}

void move(vector<string> inputs) {
  assert(inputs.size() == 4);
  currentBoard -> move(Move({inputs[1][0] - '0', inputs[1][1] - '0'}, {inputs[2][0] - '0', inputs[2][1] - '0'}, {inputs[3][0] - '0', inputs[3][1] - '0'}));
}

void go(vector<string> inputs) {
  if (inputs[1] == "depth") {
    currentBoard -> alphabeta(stoi(inputs[2]));
    printf("bestmove %d%d %d%d %d%d\n", bestmove.ori.first, bestmove.ori.second, bestmove.tar.first, bestmove.tar.second, bestmove.shoot.first, bestmove.shoot.second);
    fflush(stdout);
  } else if (inputs[1] == "random") {
    currentBoard -> randomMove();
  } else if (inputs[1] == "greedy") {
    currentBoard -> greedyMove();
  }
}

void position(vector<string> inputs) {
  currentBoard = new Board();
  currentBoard -> currentPlayer = 1;
  int cur = 1;
  for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) {
    currentBoard -> arr[i][j] = stoi(inputs[cur]);
    cur++;
  }
  if (currentBoard -> arr[0][0] == 1 || currentBoard -> arr[0][7] == 1) {
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) currentBoard -> arr[i][j] *= -1;
    currentBoard -> currentPlayer = -1;
  }

  currentBoard -> print();
}

int main() {
  string input;
  while (1) {
    if (!getline(cin, input)) return 0;
    cerr << "inp " << input << endl;
    auto inputs = split(input);
    if (input == "quit") return 0;
    else if (input == "newgame") startpos();
    else if (inputs[0] == "move") move(inputs);
    else if (input == "board") currentBoard -> print();
    else if (inputs[0] == "go") go(inputs);
    else if (input == "selfplay random") {
      while (currentBoard -> getGameEnded() == 2) {
        currentBoard -> randomMove();
        currentBoard -> print();
        this_thread::sleep_for(chrono::nanoseconds(100000000));
      }
    } else if (input == "selfplay greedy") {
      while (currentBoard -> getGameEnded() == 2) {
        currentBoard -> greedyMove();
        currentBoard -> print();
        this_thread::sleep_for(chrono::nanoseconds(100000000));
      }
    } else if (input == "selfplay alphabeta") {
      while (currentBoard -> getGameEnded() == 2) {
        currentBoard -> alphabeta(6);
        currentBoard -> print();
      }
    } else if (inputs[0] == "position") {
      position(inputs);
    }
  }
  return 0;
}
