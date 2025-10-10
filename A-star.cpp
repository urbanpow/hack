#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <functional>

using namespace std;

// Structure to represent a cell in the grid
struct Cell {
    int x, y;
    int f, g, h; // f = g + h (total cost, cost from start, heuristic to end)
    Cell* parent;
    
    Cell(int x, int y) : x(x), y(y), f(0), g(0), h(0), parent(nullptr) {}
    
    // Overload == operator to compare cells
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
    
    // Calculate the heuristic (Manhattan distance)
    int calculateH(const Cell& goal) const {
        return abs(x - goal.x) + abs(y - goal.y);
    }
};

// Hash function for Cell to use in unordered_map
namespace std {
    template<>
    struct hash<Cell> {
        size_t operator()(const Cell& cell) const {
            return hash<string>()(to_string(cell.x) + "," + to_string(cell.y));
        }
    };
}

class AStar {
private:
    vector<vector<int>> grid; // 0 = free, 1 = obstacle
    int rows, cols;
    
    // Check if a cell is valid (within grid and not an obstacle)
    bool isValid(int x, int y) const {
        return x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 0;
    }
    
    // Reconstruct the path from end to start
    vector<Cell> reconstructPath(Cell* current) {
        vector<Cell> path;
        while (current != nullptr) {
            path.push_back(*current);
            current = current->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }

public:
    AStar(const vector<vector<int>>& grid) : grid(grid) {
        rows = grid.size();
        cols = grid[0].size();
    }
    
    // Find path from start to goal using A* algorithm
    vector<Cell> findPath(Cell start, Cell goal) {
        // If start or goal is an obstacle, return empty path
        if (!isValid(start.x, start.y) || !isValid(goal.x, goal.y)) {
            return {};
        }
        
        // If start is the same as goal
        if (start.x == goal.x && start.y == goal.y) {
            return {start};
        }
        
        // Initialize open and closed lists
        vector<Cell> openList;
        vector<Cell> closedList;
        
        // Add start node to open list
        start.h = start.calculateH(goal);
        start.f = start.h;
        openList.push_back(start);
        
        while (!openList.empty()) {
            // Find the node with the least f in openList
            auto currentIt = min_element(openList.begin(), openList.end(),
                [](const Cell& a, const Cell& b) { return a.f < b.f; });
            
            // Get the current node and move it to closed list
            Cell current = *currentIt;
            closedList.push_back(current);
            openList.erase(currentIt);
            
            // Check all 8 possible movements (including diagonals)
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    // Skip the current cell
                    if (dx == 0 && dy == 0) continue;
                    
                    int x = current.x + dx;
                    int y = current.y + dy;
                    
                    // If the neighbor is valid
                    if (isValid(x, y)) {
                        // Create neighbor cell
                        Cell neighbor(x, y);
                        
                        // If this is the goal, we're done
                        if (x == goal.x && y == goal.y) {
                            neighbor.parent = new Cell(current.x, current.y);
                            return reconstructPath(&neighbor);
                        }
                        
                        // Skip if neighbor is in closed list
                        if (find(closedList.begin(), closedList.end(), neighbor) != closedList.end()) {
                            continue;
                        }
                        
                        // Calculate g, h, and f
                        int gNew = current.g + ((dx != 0 && dy != 0) ? 14 : 10); // 14 for diagonal, 10 for straight
                        int hNew = neighbor.calculateH(goal);
                        int fNew = gNew + hNew;
                        
                        // Check if neighbor is already in open list
                        auto it = find(openList.begin(), openList.end(), neighbor);
                        if (it != openList.end()) {
                            // If we found a better path to the neighbor
                            if (gNew < it->g) {
                                it->g = gNew;
                                it->f = fNew;
                                it->parent = new Cell(current.x, current.y);
                            }
                        } else {
                            // Add neighbor to open list
                            neighbor.g = gNew;
                            neighbor.h = hNew;
                            neighbor.f = fNew;
                            neighbor.parent = new Cell(current.x, current.y);
                            openList.push_back(neighbor);
                        }
                    }
                }
            }
        }
        
        // No path found
        return {};
    }
    
    // Print the grid with the path
    void printPath(const vector<Cell>& path) {
        vector<vector<char>> display(rows, vector<char>(cols, '.'));
        
        // Mark obstacles
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == 1) {
                    display[i][j] = '#';
                }
            }
        }
        
        // Mark path
        for (size_t i = 0; i < path.size(); ++i) {
            display[path[i].x][path[i].y] = (i == 0) ? 'S' : 
                                           (i == path.size() - 1) ? 'E' : '*';
        }
        
        // Print the grid
        for (const auto& row : display) {
            for (char c : row) {
                cout << c << ' ';
            }
            cout << '\n';
        }
    }
};

int main() {
    // Example grid (0 = free, 1 = obstacle)
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    // Create A* solver
    AStar aStar(grid);
    
    // Define start and goal
    Cell start(0, 0);
    Cell goal(9, 9);
    
    // Find path
    vector<Cell> path = aStar.findPath(start, goal);
    
    if (path.empty()) {
        cout << "No path found!" << endl;
    } else {
        cout << "Path found! Length: " << path.size() << endl;
        aStar.printPath(path);
    }
    
    return 0;
}