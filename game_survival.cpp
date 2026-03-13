#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Global Constants
///////////////////////////////////////////////////////////////////////////

const int INITIAL_POINTS    = 30;
const int MOVE_COST         = 1;
const int ACTION_COST       = 1; // Extra cost for push, pull, or activate
const int TRAP_PENALTY      = 5;
const int TREASURE_REWARD   = 15;

// CellType is the type of cells that can be
// on the maze board
// Think of them like constant values for each type that
// you can use by name
enum class CellType { EMPTY, WALL, BLOCK, SWITCH, TREASURE, TRAP, EXIT };

// These are the cells in the maze board
// They have a type and a boolean to indicate if activated or not
// It is important for SWITCH cells to win the game 
// At least one SWITCH must be activated to exit
struct Cell {
    CellType type = CellType::EMPTY;
    bool activated = false; 
};

///////////////////////////////////////////////////////////////////////////
// Class Definitions
///////////////////////////////////////////////////////////////////////////

class Maze;

class Player {
public:
    Player(Maze* maze, int r, int c, int points = INITIAL_POINTS)
        : m_maze(maze), m_row(r), m_col(c), m_points(points) {}

    int row() const { return m_row; }
    int col() const { return m_col; }
    int points() const { return m_points; }
    bool isDead() const { return m_points <= 0; }

    void move(char dir);
    void push(char dir);
    void pull(char dir);
    void collect();
    void activate();

private:
    Maze* m_maze;
    int m_row, m_col;
    int m_points;
};

class Maze {
public:
    Maze(int rows, int cols);
    ~Maze();

    void display(const Player* player, const string& msg = "") const;
    bool isInBounds(int r, int c) const { return r >= 0 && r < m_rows && c >= 0 && c < m_cols; }
    Cell& at(int r, int c) { return m_grid[r][c]; }
    
    bool hasActiveSwitch() const;
    void generateRandom(int pR, int pC);

private:
    int m_rows, m_cols;
    // The grid is a 2D array of cells
    // That is why it is save as a pointer to a pointer
    Cell** m_grid; // Manual Dynamic 2D Array
};

class Game {
public:
    Game(int rows, int cols);
    ~Game();
    void play();

private:
    Maze* m_maze;
    Player* m_player;
};

///////////////////////////////////////////////////////////////////////////
// Maze Implementation
///////////////////////////////////////////////////////////////////////////

Maze::Maze(int rows, int cols) : m_rows(rows), m_cols(cols) {
    // TODO: 1. Dynamically allocate m_grid (array of Cell pointers)
    // TODO: 2. For each pointer, allocate an array of Cells
    // TODO: 3. Initialize the perimeter as WALL and the rest as EMPTY
}

Maze::~Maze() {
   // TODO: Delete each row, then delete the top-level array
}



bool Maze::hasActiveSwitch() const {
    //TODO check if any Switch cells are activated
    //Return true if so and false otherwise
    return false;
}

// Display function provided for you
// Please do not modify
void Maze::display(const Player* player, const string& msg) const {
    system("clear || cls");
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++) {
            if (player && player->row() == r && player->col() == c) cout << '@';
            else {
                switch (m_grid[r][c].type) {
                    case CellType::EMPTY:    cout << '.'; break;
                    case CellType::WALL:     cout << '*'; break;
                    case CellType::BLOCK:    cout << 'B'; break;
                    case CellType::SWITCH:   cout << (m_grid[r][c].activated ? 's' : 'S'); break;
                    case CellType::TREASURE: cout << 'T'; break;
                    case CellType::TRAP:     cout << '^'; break;
                    case CellType::EXIT:     cout << 'E'; break;
                }
            }
        }
        cout << endl;
    }
    cout << msg << endl;
}

///////////////////////////////////////////////////////////////////////////
// Player Implementation
///////////////////////////////////////////////////////////////////////////

void Player::move(char dir) {
	// TODO: Implement move
	char a = dir;
	return;
}



void Player::push(char dir) {
	//TODO: Implement push block
	char a = dir;
	return;
}

void Player::pull(char dir) {
	//TODO: Implement pull block
	char a = dir;
	return;
}

void Player::collect() {
	//TODO: Implement collect treasure
	return;
}

void Player::activate() {
	// TODO: Implement active switch
}

///////////////////////////////////////////////////////////////////////////
// Game Implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols) {
    m_maze = new Maze(rows, cols);
    int startR = 1, startC = 1;
    //TODO: Uncomment the following line
    //Once you have implemented the constructor for maze
    //m_maze->generateRandom(startR, startC);
    //TODO: Change this call to Player constructor to
    //use the default value
    //Right now negative score to end game
    m_player = new Player(m_maze, startR, startC, -10);
}

Game::~Game() {
    delete m_player;
    delete m_maze;
}

void Maze::generateRandom(int pR, int pC) {
    CellType types[] = { CellType::WALL, CellType::BLOCK, CellType::SWITCH, CellType::TREASURE, CellType::TRAP, CellType::EXIT };
    int counts[] = { 15, 4, 2, 3, 4, 1 };

    for (int i = 0; i < 6; i++) {
        int placed = 0;
        while (placed < counts[i]) {
            int r = rand() % m_rows;
            int c = rand() % m_cols;
            if (at(r, c).type == CellType::EMPTY && (r != pR || c != pC)) {
                at(r, c).type = types[i];
                placed++;
            }
        }
    }
}

void Game::play() {
    while (!m_player->isDead()) {
        string status = "Points: " + to_string(m_player->points()) + " | n/s/e/w | p[dir] | l[dir] | c | a";
        m_maze->display(m_player, status);
        string cmd; getline(cin, cmd);
        if (cmd.empty()) continue;
        char action = cmd[0];
        if (action == 'c') m_player->collect();
        else if (action == 'a') m_player->activate();
        else if (action == 'p' && cmd.size() > 1) m_player->push(cmd[1]);
        else if (action == 'l' && cmd.size() > 1) m_player->pull(cmd[1]);
        else m_player->move(action);
    }
    cout << "OUT OF ENERGY. GAME OVER." << endl;
}

int main() {
    srand(time(0));
    Game g(12, 25);
    g.play();
    return 0;
}
