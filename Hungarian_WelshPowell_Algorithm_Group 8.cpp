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
        
        long long delta = cost[u][v] - lx[u] - ly[v];
        
        if (delta == 0) { 
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
    vector<int> match(N, -1);
    
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (cost[i][j] != INF) {
                lx[i] = min(lx[i], cost[i][j]);
            }
        }
       
        if (lx[i] == INF) lx[i] = 0; 
    }
    
    for (int u = 0; u < N; ++u) { 
        vector<long long> slack(N, INF);
        while (true) {
            vector<bool> visited_u(N, false);
            vector<bool> visited_v(N, false);
            
            if (dfs_hungarian(u, cost, N, lx, ly, match, slack, visited_u, visited_v)) {
                break;
            } else {
                
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
            assignments[u] = {u, v}; 
        }
    }

    for(int u = 0; u < N; ++u) {
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