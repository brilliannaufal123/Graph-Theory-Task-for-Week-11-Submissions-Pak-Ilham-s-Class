| Name | NRP | Class |
|---|---|---|
| Naufal Bintang Brillian | 5025241168 | IUP |
| Bismantaka Revano Dirgantara | 5025241075 | IUP |
| Stephanie Gabriella Adiseputra | 5025241081 | IUP |

# Graph-Theory-Assignment-Welsh-Powell-and-Hungarian

In this repository, **we** have implemented two core graph theory algorithms in C++:
1.  **Welsh-Powell Algorithm** for the Graph Coloring problem.
2.  **Hungarian Algorithm** for the Minimum-Cost Assignment problem.

## Welsh-Powell Algorithm (Graph Coloring)

**We** implemented the **Welsh-Powell algorithm**, a heuristic for the **Graph Coloring problem**. Our goal here is to assign a "color" (represented by an integer) to each node in a graph such that no two adjacent nodes share the same color. **We** applied this algorithm to the real-world problem of **VM-to-Host placement**, where VMs with conflicts (edges) cannot be placed on the same physical host (color).

Our program first reads the number of VMs (nodes) and a list of conflicts (edges), which **we** store in an **adjacency matrix**. The Welsh-Powell algorithm itself works by first calculating the **degree** (number of connections) for every node. It then sorts all nodes in descending order of their degree. The algorithm iterates through the sorted list: it assigns the first available "color" (e.g., `Host 1`) to the highest-degree uncolored node, and then iterates through the rest of the sorted list, assigning the *same color* to any node that is **not** adjacent to any other node already given that color in this pass. This process repeats with a new color (`Host 2`, `Host 3`, ...) until all nodes are colored.



---

### The main components we built are:

* **struct NodeDegree**
    * A helper structure **we use** to store a node's `id` and `degree`.
    * This is used to sort all nodes in descending order of their degree.
* **runWelshPowell(adj, numNodes)**
    * This is where **we** implement the core coloring logic.
    * It sorts the `NodeDegree` list and iterates through it, assigning "colors" (Host IDs) to nodes based on their adjacency.
* **main()**
    * This function handles the user's scenario selection.
    * It prompts the user for the number of VMs and conflicts, builds the adjacency matrix, and then calls **our** `runWelshPowell()` function.

---

### Input Format

The input consists of:

1.  One integer `1` to select the Welsh-Powell scenario.
2.  Two integers for `numVMs` (nodes) and `numConflicts` (edges).
3.  `numConflicts` lines, each containing a pair of conflicting VMs: `vm1 vm2`

> Node coordinates are 0-based.

---

### Example Input

Here is an example of how **we** run the program for a 5-node cycle graph:

`1 5 5 0 1 1 2 2 3 3 4 4 0`


### Example Output
```
--- Running Welsh-Powell (VM Placement) ---
VM to Physical Host Placement Result:
Total Physical Hosts Required: 3
  Host ID 1: [ VM 0 VM 2 ]
  Host ID 2: [ VM 1 VM 3 ]
  Host ID 3: [ VM 4 ]
--------------------------------------------------
```
(Note: The exact groupings, e.g., [0, 3], [1, 4], [2], are also valid. The key is that the total count is 3.)

Explanation
- The 5 VMs form a 5-node cycle graph (C5), which is known to require 3 colors.

- Our algorithm correctly determines that 3 physical hosts are required.

- Host 1 is assigned to VM 0 and VM 2 (which are not adjacent).

- Host 2 is assigned to VM 1 and VM 3 (which are not adjacent).

- Host 3 is assigned to the remaining VM 4.

- No two conflicting VMs are placed on the same host.

Hungarian Algorithm (Assignment Problem)
For the second part, we implemented the Hungarian algorithm to solve the minimum-cost assignment problem. Our goal was to find a perfect matching in a weighted bipartite graph that has the minimum possible total cost. We applied this to the Job-to-Server assignment problem, where we want to find the one-to-one assignment that minimizes the total operational cost.

Our program reads the number of jobs and servers (`N`) and a list of all possible assignments with their costs. From this, we build an `N x N` cost matrix. The algorithm uses potentials (or labels, `lx` and `ly`) for each node in the two sets (Jobs and Servers). It iteratively searches for augmenting paths in an equality subgraph (a subgraph containing only edges where `cost[i][j] == lx[i] + ly[j]`) using a **Depth-First Search (DFS**). If no augmenting path is found, the algorithm updates the potentials (`lx` and `ly`) based on the minimum slack value. This update effectively adds new edges to the equality subgraph, guaranteeing that a path will eventually be found. This process repeats until all jobs are matched, guaranteeing an optimal solution with a complexity of `O(n³)`.

The main functions we created for this are:
- dfs_hungarian(...)

  - A recursive DFS helper function we use to search for an augmenting path in the current equality subgraph.

  - It uses visited_u, visited_v, and slack arrays to manage the search.

  - It returns true if a path is found and the matching can be augmented.

- runHungarian(cost, N)

  - This function initializes the potentials `lx` and `ly` for each job and server.

  - We loop through each job `u`, repeatedly calling `dfs_hungarian()` to find a match.

  - If no path is found, it updates the potentials based on the minimum `slack` value to expand the equality subgraph.

  - After all jobs are matched, we reconstruct the optimal assignments and print the total minimum cost.

**Input Format**
The input consists of:

1. One integer `2` to select the Hungarian scenario.

2. One integer `N` (the number of jobs, which assumes an equal number `N` of servers).

3. One integer `numEdges` (the number of possible assignments/edges in the bipartite graph).

4. `numEdges` lines of: `job_id server_id cost`

   All IDs are 0-based.

Example Input
Here is an example input we used for a 3x3 assignment problem:
```
2
3
9
0 0 5
0 1 1
0 2 3
1 0 6
1 1 4
1 2 2
2 0 2
2 1 5
2 2 7
```
Example Output
```
--- Running Hungarian (Job to Server Assignment) ---
Job to Server Assignment (Minimum Cost):
  Job 0 -> Server 1 (Cost: 1)
  Job 1 -> Server 2 (Cost: 2)
  Job 2 -> Server 0 (Cost: 2)
Total Minimum Cost: 5
--------------------------------------------------
```
**Explanation**
- Our program constructs a 3x3 cost matrix and finds the cheapest combination of one-to-one assignments.

- The optimal assignment for `Job 0` is `Server 1` (Cost: 1).

- The optimal assignment for `Job 1` is `Server 2` (Cost: 2).

- The optimal assignment for `Job 2` is `Server 0` (Cost: 2).

- The total minimum cost is the sum of these assignments: 1 + 2 + 2 = 5.


---------------------------------------------------

### The Code (using C++):
```
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
using namespace std;

// --- Welsh-Powell Algorithm Implementation ---
struct NodeDegree {
    int id;
    int degree;
    bool operator<(const NodeDegree& other) const {
        return degree > other.degree;
    }
};

void runWelshPowell(const vector<vector<int>>& adj, int numNodes) {
    cout << "\n--- Running Welsh-Powell (VM Placement) ---" << endl;
    vector<NodeDegree> nodeDegrees(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        nodeDegrees[i].id = i;
        nodeDegrees[i].degree = 0;
        for (int j = 0; j < numNodes; ++j) {
            if (adj[i][j]) {
                nodeDegrees[i].degree++;
            }
        }
    }
    sort(nodeDegrees.begin(), nodeDegrees.end());
    map<int, int> colors;
    int currentColor = 1;
    for (const auto& node : nodeDegrees) {
        int u = node.id;
        if (colors.count(u)) {
            continue;
        }
        colors[u] = currentColor;
        for (const auto& otherNode : nodeDegrees) {
            int v = otherNode.id;
            if (v != u && !colors.count(v)) {
                bool canColor = true;
                for (const auto& pair : colors) {
                    int w = pair.first;
                    int color = pair.second;
                    if (color == currentColor && adj[v][w]) {
                        canColor = false;
                        break;
                    }
                }
                if (canColor) {
                    colors[v] = currentColor;
                }
            }
        }
        currentColor++;
    }
    cout << "VM to Physical Host Placement Result:" << endl;
    cout << "Total Physical Hosts Required: " << (currentColor - 1) << endl;
    for (int host = 1; host < currentColor; ++host) {
        cout << "  Host ID " << host << ": [ ";
        for (const auto& pair : colors) {
            if (pair.second == host) {
                cout << "VM " << pair.first << " ";
            }
        }
        cout << "]" << endl;
    }
    cout << "--------------------------------------------------\n";
}

// --- Hungarian Algorithm Implementation ---

const long long INF = numeric_limits<long long>::max();

// DFS helper to find augmenting path (Min-Cost)
bool dfs_hungarian(int u, const vector<vector<long long>>& cost, int N,
                   vector<long long>& lx, vector<long long>& ly, 
                   vector<int>& match, vector<long long>& slack,
                   vector<bool>& visited_u, vector<bool>& visited_v) {
    
    visited_u[u] = true;
    
    for (int v = 0; v < N; ++v) {
        if (visited_v[v]) continue;
        
        // **FIX 2: Changed delta calculation for Min-Cost**
        long long delta = cost[u][v] - lx[u] - ly[v];
        
        if (delta == 0) { // Edge (u,v) is in the equality subgraph
            visited_v[v] = true;
            if (match[v] == -1 || dfs_hungarian(match[v], cost, N, lx, ly, match, slack, visited_u, visited_v)) {
                match[v] = u;
                return true;
            }
        } else {
            slack[v] = min(slack[v], delta);
        }
    }
    return false;
}

// Main Hungarian Algorithm function (Min-Cost)
void runHungarian(const vector<vector<long long>>& cost, int N) {
    cout << "\n--- Running Hungarian (Job to Server Assignment) ---" << endl;
    
    if (N == 0) {
        cout << "No jobs or servers to assign." << endl;
        return;
    }

    vector<long long> lx(N, INF), ly(N, 0);
    // match[v] = u (server v is paired with job u)
    vector<int> match(N, -1);
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (cost[i][j] != INF) {
                lx[i] = min(lx[i], cost[i][j]);
            }
        }
        // Handle rows with no edges (all INF)
        if (lx[i] == INF) lx[i] = 0; 
    }
    
    for (int u = 0; u < N; ++u)
        vector<long long> slack(N, INF);
        while (true) {
            vector<bool> visited_u(N, false);
            vector<bool> visited_v(N, false);
            
            if (dfs_hungarian(u, cost, N, lx, ly, match, slack, visited_u, visited_v)) {
                break;
            } else {
                // Update potentials (labels)
                long long delta = INF;
                for (int v = 0; v < N; ++v) {
                    if (!visited_v[v]) {
                        delta = min(delta, slack[v]);
                    }
                }
                
                for (int i = 0; i < N; ++i) {
                    if (visited_u[i]) lx[i] += delta;
                }
                for (int j = 0; j < N; ++j) {
                    if (visited_v[j]) ly[j] -= delta;
                }
                // Slack update remains the same logic (slack_new = slack_old - delta)
                for (int v = 0; v < N; v++) {
                    if (!visited_v[v]) {
                        slack[v] -= delta;
                    }
                }
            }
        }
    }
    
    // --- Hungarian Output ---
    cout << "Job to Server Assignment (Minimum Cost):" << endl;
    long long totalCost = 0;
    vector<pair<int, int>> assignments(N);
    
    for (int v = 0; v < N; ++v) {
        if (match[v] != -1) {
            int u = match[v];
            assignments[u] = {u, v}; // Store (job, server)
        }
    }

    for(int u = 0; u < N; ++u) {
        // Handle cases where a job might not be assigned (if N != M)
        // Although in this N=M code, it should always be assigned
        if (assignments[u].first == u) {
             int v = assignments[u].second;
             if (cost[u][v] == INF) {
                  cout << "  Job " << u << " -> CANNOT BE ASSIGNED" << endl;
             } else {
                  cout << "  Job " << u << " -> Server " << v << " (Cost: " << cost[u][v] << ")" << endl;
                  totalCost += cost[u][v];
             }
        } else {
            // This case shouldn't happen in a square matrix, but good to have
             cout << "  Job " << u << " -> NOT ASSIGNED" << endl;
        }
    }
    
    cout << "Total Minimum Cost: " << totalCost << endl;
    cout << "--------------------------------------------------\n";
}

int main() {
    int choice;
    cout << "===========================================" << endl;
    cout << "     Graph Theory - Implementation" << endl;
    cout << "         Welsh-Powell & Hungarian" << endl;
    cout << "===========================================" << endl;
    cout << "Select Scenario (1 or 2):" << endl;
    cout << "1. Welsh-Powell (VM Placement)" << endl;
    cout << "2. Hungarian (Job to Server Assignment)" << endl;
    cout << "Your choice: ";
    cin >> choice;
    
    if (choice == 1) {
        int numVMs, numConflicts;
        cout << "Enter number of VMs (nodes): ";
        cin >> numVMs;
        cout << "Enter number of Conflicts (edges): ";
        cin >> numConflicts;
        vector<vector<int>> adjMatrix(numVMs, vector<int>(numVMs, 0));
        cout << "Enter " << numConflicts << " conflicts (format: vm1 vm2):" << endl;
        for (int i = 0; i < numConflicts; ++i) {
            int u, v;
            cin >> u >> v;
            if (u >= 0 && u < numVMs && v >= 0 && v < numVMs) {
                adjMatrix[u][v] = 1;
                adjMatrix[v][u] = 1;
            }
        }
        runWelshPowell(adjMatrix, numVMs);
        
    } else if (choice == 2) {
        int N;
        int numEdges;
        cout << "Enter number of Jobs (e.g., N): ";
        cin >> N;
        cout << "Assuming: " << N << " Servers exist." << endl;
        cout << "         Jobs = 0..." << (N-1) << endl;
        cout << "         Servers = 0..." << (N-1) << endl;
        cout << "Enter number of possible assignments (edges): ";
        cin >> numEdges;
        vector<vector<long long>> costMatrix(N, vector<long long>(N, INF));
        cout << "Enter " << numEdges << " assignments (format: job_id server_id cost):" << endl;
        for (int i = 0; i < numEdges; ++i) {
            int u, v;
            long long cost;
            cin >> u >> v >> cost;
            if (u >= 0 && u < N && v >= 0 && v < N) {
                costMatrix[u][v] = cost;
            }
        }
        runHungarian(costMatrix, N);

    } else {
        cout << "Invalid choice." << endl;
    }
    
    return 0;
}
```
