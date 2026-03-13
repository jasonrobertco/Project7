#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/*
The game world consists of a rectangular grid of cells. Each cell may be empty, a wall, a movable block, a
switch, a treasure, a trap, or the exit
*/

///////////////////////////////////////////////////////////////////////////
// Global Constants
///////////////////////////////////////////////////////////////////////////

const int INITIAL_POINTS = 30;
const int MOVE_COST = 1;
const int ACTION_COST = 1; // Extra cost for push, pull, or activate
const int TRAP_PENALTY = 5;
const int TREASURE_REWARD = 15;

// CellType is the type of cells that can be
// on the maze board
// Think of them like constant values for each type that
// you can use by name
enum class CellType
{
    EMPTY,
    WALL,
    BLOCK,
    SWITCH,
    TREASURE,
    TRAP,
    EXIT
};

// These are the cells in the maze board
// They have a type and a boolean to indicate if activated or not
// It is important for SWITCH cells to win the game
// At least one SWITCH must be activated to exit
struct Cell
{
    CellType type = CellType::EMPTY;
    bool activated = false;
};

///////////////////////////////////////////////////////////////////////////
// Class Definitions
///////////////////////////////////////////////////////////////////////////
class Maze;

class Player
{
public:
    Player(Maze *maze, int r, int c, int points = INITIAL_POINTS)
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
    Maze *m_maze;
    int m_row, m_col;
    int m_points;
};

class Maze
{ // class maze
    // public
public:
    Maze(int rows, int cols); // constructor
    ~Maze();                  // destructor

    void display(const Player *player, const string &msg = "") const;
    bool isInBounds(int r, int c) const { return r >= 0 && r < m_rows && c >= 0 && c < m_cols; }
    Cell &at(int r, int c) { return m_grid[r][c]; }

    bool hasActiveSwitch() const;
    void generateRandom(int pR, int pC);

    // private
private:
    int m_rows, m_cols;
    // The grid is a 2D array of cells
    // That is why it is save as a pointer to a pointer
    Cell **m_grid; // Manual Dynamic 2D Array
    // m_grid points to dyn 2D cells
};

class Game
{
public:
    Game(int rows, int cols);
    ~Game();
    void play();

private:
    Maze *m_maze;
    Player *m_player;
};

///////////////////////////////////////////////////////////////////////////
// Maze Implementation
///////////////////////////////////////////////////////////////////////////

Maze::Maze(int rows, int cols) : m_rows(rows), m_cols(cols)
{
    // TODO: 1. Dynamically allocate m_grid (array of Cell pointers)
    m_grid = new Cell *[m_rows]; //ROW POINTERS
    /*
    m grid is a cell pointer

    new dyanmically allocate heap
    cell* pointer to cell

    array of mrows elements
    */

    // TODO: 2. For each pointer, allocate an array of Cells
    for (int i = 0; i < m_rows; i++)  // loop thru rows
        m_grid[i] = new Cell[m_cols]; // ROW HAS M COLS (HOZ LENGTH)
    // COLUMS VERITICAL
    // ROWS HOZ

    // TODO: 3. Initialize the perimeter as WALL and the rest as EMPTY
    for (int r = 0; r < m_rows; r++) //loop thru row
    {
        for (int c = 0; c < m_cols; c++) //loop thru col
        {
            /*
            r == 0 first
            r == mrows-1 last

            */
            if (r == 0 || r == m_rows - 1 || c == 0 || c == m_cols - 1)
                m_grid[r][c].type = CellType::WALL; //border
            else
                m_grid[r][c].type = CellType::EMPTY; //empty cell
        }
    }
}

Maze::~Maze()
{
    // TODO: Delete each row, then delete the top-level array
    for (int i = 0; i < m_rows; i++) //scan thru # rows
        delete[] m_grid[i]; //delete cell array in that row
    delete[] m_grid; //delete top row pointer
}

bool Maze::hasActiveSwitch() const
{
    // TODO check if any Switch cells are activated
    // Return true if so and false otherwise
    for (int row = 0; row < m_rows; row++){ //looop thru row
        for (int col = 0; col < m_cols; col++){ //loop thru col
            if (m_grid[row][col].type == CellType::SWITCH && m_grid[row][col].activated){
                return true;
            }
        }
    }
    return false;
}

// Display function provided for you
// Please do not modify
void Maze::display(const Player *player, const string &msg) const
{
    system("clear || cls");
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
        {
            if (player && player->row() == r && player->col() == c)
                cout << '@';
            else
            {
                switch (m_grid[r][c].type)
                {
                case CellType::EMPTY:
                    cout << '.';
                    break;
                case CellType::WALL:
                    cout << '*';
                    break;
                case CellType::BLOCK:
                    cout << 'B';
                    break;
                case CellType::SWITCH:
                    cout << (m_grid[r][c].activated ? 's' : 'S');
                    break;
                case CellType::TREASURE:
                    cout << 'T';
                    break;
                case CellType::TRAP:
                    cout << '^';
                    break;
                case CellType::EXIT:
                    cout << 'E';
                    break;
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

void Player::move(char dir)
{
    /*
    Calculate the target coordinates based on the input direction (’n’, ’s’, ’e’, ’w’). You must verify that
the move is within the maze bounds and that the destination is not a WALL or a BLOCK. Furthermore,
an EXIT cell acts as an impassable barrier unless hasActiveSwitch() returns true. If the move is
valid, update the player’s position and subtract the MOVE COST. Finally, handle cell-specific triggers:
if the player enters a TRAP, deduct the TRAP PENALTY; if the player reaches a powered EXIT, the game
ends in success*/
    // TODO: Implement move
    //char a = dir;
    //row and column direction
    int dr = 0, dc = 0;
    if (dir == 'n')
        dr = -1;
    else if (dir == 's')
        dr = 1;

    else if (dir == 'e')
        dc = 1;
    else if (dir == 'w')
        dc = -1;
    else
        return;

    //new position
    int newRow = m_row + dr;
    int newCol = m_col + dc;

    //bounds check
    if (!m_maze->isInBounds(newRow, newCol))
        return;

    //cell reference
    Cell& target = m_maze->at(newRow, newCol);

    //check wall or block
    if (target.type == CellType::WALL || target.type == CellType::BLOCK)
        return;

    //check locked exit
    if (target.type == CellType::EXIT && !m_maze->hasActiveSwitch())
        return;

    //if pass change to new position update movepoints by 1
    m_row = newRow;
    m_col = newCol;
    m_points -= MOVE_COST;

    //cell type = trap
    if (target.type == CellType::TRAP)
        m_points -= TRAP_PENALTY;

    //cell type is euqal to exit and 1 active switch is on
    if (target.type == CellType::EXIT && m_maze->hasActiveSwitch())
    {
        cout << "SUCCESS! Exit powered. Final Points: " << m_points << endl;
        exit(0);
    }
}

void Player::push(char dir)
{
    // TODO: Implement push block
    /*
    void push(char dir);
Check if a BLOCK is adjacent to the player in the given direction. If the cell immediately beyond that
block is also within bounds and EMPTY, move the block to that target cell and move the player into the
block’s former spot. This action is physically demanding and costs 1 point (ACTION COST) in addition
to the standard movement cost.
*/
    //direction code repeated from above
    int dr = 0, dc = 0;
    if (dir == 'n')
        dr = -1;
    else if (dir == 's')
        dr = 1;
    else if (dir == 'e')
        dc = 1;
    else if (dir == 'w')
        dc = -1;
    else
        return;

    //new block code repeated from above
    int blockRow = m_row + dr;
    int blockCol = m_col + dc;

    //find block behind current block
    int targetRow = blockRow + dr;
    int targetCol = blockCol + dc;

    //checkbounds if either are in bounds fo rmaze
    if (!m_maze->isInBounds(blockRow, blockCol) || !m_maze->isInBounds(targetRow, targetCol))
        return;

    //in front block has to be a block
    if (m_maze->at(blockRow, blockCol).type != CellType::BLOCK)
        return;

    //in front block cannot be empty
    if (m_maze->at(targetRow, targetCol).type != CellType::EMPTY)
        return;

    //make the new target (behind) a block make the old empty so that the block was pusedh
    m_maze->at(targetRow, targetCol).type = CellType::BLOCK;
    m_maze->at(blockRow, blockCol).type = CellType::EMPTY;

    //move palyer
    m_row = blockRow;
    m_col = blockCol;

    //calculate aciton cost
    m_points -= (MOVE_COST + ACTION_COST);
}

void Player::pull(char dir)
{
    // TODO: Implement pull block
    /*
    void pull(char dir);
Check if the cell in the given direction is EMPTY and if there is a BLOCK located directly behind the
player (the opposite direction). If both are true, the player moves forward and the block is ”dragged”
into the player’s previous coordinates. This action costs 1 point (ACTION COST) in addition to the
movement cost.
    */
   //same movmeent code
    int dr = 0, dc = 0;
    if (dir == 'n')
        dr = -1;
    else if (dir == 's')
        dr = 1;
    else if (dir == 'e')
        dc = 1;
    else if (dir == 'w')
        dc = -1;
    else
        return;
    //new postion
    int newRow = m_row + dr;
    int newCol = m_col + dc;
    //target pull direction
    int blockRow = m_row - dr;
    int blockCol = m_col - dc;
    //check bounds
    if (!m_maze->isInBounds(newRow, newCol) || !m_maze->isInBounds(blockRow, blockCol))
        return;
    //check if its not empty
    if (m_maze->at(newRow, newCol).type != CellType::EMPTY)
        return;
    //check if its ot a block
    if (m_maze->at(blockRow, blockCol).type != CellType::BLOCK)
        return;
    //if conditiosn satisfies update the new pulled position to block and remove the old block set to epty
    m_maze->at(m_row, m_col).type = CellType::BLOCK;
    m_maze->at(blockRow, blockCol).type = CellType::EMPTY;
    //update the players position

    m_row = newRow;
    m_col = newCol;
    //update aciton cost
    m_points -= (MOVE_COST + ACTION_COST);
}

void Player::collect()
{
    // TODO: Implement collect treasure

    /*
    Check the player’s current cell. If the cell type is TREASURE, change the cell type to EMPTY and increase
the player’s points by the TREASURE REWARD.
    */

    //call at on m_maze
    //if cell type is treasure
    if (m_maze->at(m_row, m_col).type == CellType::TREASURE)
    {
        //set cell type to empty
        m_maze->at(m_row, m_col).type = CellType::EMPTY;
        //change players points by treasure reward
        m_points += TREASURE_REWARD;
    }
    return;
}

void Player::activate()
{
    // TODO: Implement active switch

    /*
    void activate();
Check the player’s current cell. If the cell type is a SWITCH, toggle its activated boolean state (flipping
it from true to false or vice versa). This action costs the player 1 point (ACTION COST).
    */
   //if cell type = switch 
   if (m_maze->at(m_row, m_col).type == CellType::SWITCH)
   {
        //toggle to opposite state
        //.activated cell variable
        m_maze->at(m_row, m_col).activated = !m_maze->at(m_row, m_col).activated;
        //action point cost 1
        m_points -= ACTION_COST;
   }

}

///////////////////////////////////////////////////////////////////////////
// Game Implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols)
{
    m_maze = new Maze(rows, cols);
    int startR = 1, startC = 1;
    // TODO: Uncomment the following line
    // Once you have implemented the constructor for maze
    m_maze->generateRandom(startR, startC);
    //pointer mmaze to call generate rando funciton passing startR/C

    // TODO: Change this call to Player constructor to
    // use the default value
    // Right now negative score to end game
    //m_player = new Player(m_maze, startR, startC, -10);
    m_player = new Player(m_maze, startR, startC);
}

Game::~Game()
{
    delete m_player;
    delete m_maze;
}

void Maze::generateRandom(int pR, int pC)
{
    CellType types[] = {CellType::WALL, CellType::BLOCK, CellType::SWITCH, CellType::TREASURE, CellType::TRAP, CellType::EXIT};
    int counts[] = {15, 4, 2, 3, 4, 1};

    for (int i = 0; i < 6; i++)
    {
        int placed = 0;
        while (placed < counts[i])
        {
            int r = rand() % m_rows;
            int c = rand() % m_cols;
            if (at(r, c).type == CellType::EMPTY && (r != pR || c != pC))
            {
                at(r, c).type = types[i];
                placed++;
            }
        }
    }
}

void Game::play()
{
    while (!m_player->isDead())
    {
        string status = "Points: " + to_string(m_player->points()) + " | n/s/e/w | p[dir] | l[dir] | c | a";
        m_maze->display(m_player, status);
        string cmd;
        getline(cin, cmd);
        if (cmd.empty())
            continue;
        char action = cmd[0];
        if (action == 'c')
            m_player->collect();
        else if (action == 'a')
            m_player->activate();
        else if (action == 'p' && cmd.size() > 1)
            m_player->push(cmd[1]);
        else if (action == 'l' && cmd.size() > 1)
            m_player->pull(cmd[1]);
        else
            m_player->move(action);
    }
    cout << "OUT OF ENERGY. GAME OVER." << endl;
}

int main()
{
    srand(time(0));
    Game g(12, 25);
    g.play();
    return 0;
}
