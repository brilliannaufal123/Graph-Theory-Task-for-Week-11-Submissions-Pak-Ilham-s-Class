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
