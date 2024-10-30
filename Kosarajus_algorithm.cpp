#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list for original graph
    vector<vector<int>> revAdj; // Adjacency list for the reversed graph

    void dfs(int v, vector<bool>& visited, stack<int>& Stack) {
        visited[v] = true;
        for (int u : adj[v])
            if (!visited[u])
                dfs(u, visited, Stack);
        Stack.push(v); // Push the vertex onto the stack after its DFS finishes
    }

    void reverseDFS(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " "; // Print or store the component as needed
        for (int u : revAdj[v])
            if (!visited[u])
                reverseDFS(u, visited);
    }

public:
    Graph(int V) : V(V), adj(V), revAdj(V) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);    // Add edge to the original graph
        revAdj[w].push_back(v); // Add reverse edge for the transposed graph
    }

    void findSCCs() {
        stack<int> Stack;
        vector<bool> visited(V, false);

        // Step 1: Fill vertices in stack according to their finishing times
        for (int i = 0; i < V; i++)
            if (!visited[i])
                dfs(i, visited, Stack);

        // Step 2: Process all vertices in reverse DFS order using reversed graph
        fill(visited.begin(), visited.end(), false); // Reset visited array
        while (!Stack.empty()) {
            int v = Stack.top();
            Stack.pop();

            if (!visited[v]) {
                // Print or store the SCC
                cout << "SCC: ";
                reverseDFS(v, visited);
                cout << endl;
            }
        }
    }
};

int main() {
    Graph g(5);
    g.addEdge(1, 0);
    g.addEdge(0, 2);
    g.addEdge(2, 1);
    g.addEdge(0, 3);
    g.addEdge(3, 4);

    cout << "Strongly Connected Components:\n";
    g.findSCCs();

    return 0;
}
