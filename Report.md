# Enterprise Knowledge Documentation Core - Final Report

## 1. Project Explanation
The **Enterprise Knowledge Documentation Core** is a robust simulation of a backend system designed to manage large-scale corporate knowledge bases. The system tackles common data management problems such as fast retrieval, duplicate detection, action tracking, document relevance ranking, and optimizing data storage. By modeling these features through highly specific Data Structures and Algorithms (DSA), the project demonstrates how foundational computer science concepts scale up to solve complex enterprise problems. 

The entire project has been synthesized into a monolithic, Object-Oriented `main.cpp` file for easy compilation, featuring an interactive menu system and 20 sample documents pre-loaded for immediate testing.

---

## 2. Feature to DSA Mapping

| Feature | Data Structure / Algorithm | Justification |
| :--- | :--- | :--- |
| **Naming Registry** | **Trie** | Organizes document names character-by-character to allow rapid prefix matching (e.g. typing "auth" matches "Authentication" and "Authorization") without iterating over every string. |
| **Correction Trail** | **Stack** | Uses LIFO (Last-In-First-Out) ordering. The most recent configuration change is at the top, perfectly mimicking the behavior of an 'Undo' button. |
| **Compute Line** | **Queue** | Enforces FIFO (First-In-First-Out) processing, guaranteeing that documents are indexed strictly in the order they were uploaded. |
| **Checksum Lookup** | **Hash Table** | Provides near-instant (O(1)) lookup times to verify if a given file fingerprint (checksum) already exists, preventing duplicate uploads. |
| **Hit Sorter** | **Max Heap (Priority Queue)** | Always keeps the document with the highest relevance score at the root, allowing immediate retrieval of top search hits without sorting the entire array. |
| **Citation Map** | **Graph (Adjacency List)** | Models documents as vertices and citations as directed edges. An adjacency list is extremely memory efficient for this type of sparse network. |
| **Validation Path** | **Breadth-First Search (BFS)** | Traverses the unweighted citation graph level-by-level, mathematically guaranteeing that the first path found between two documents is the shortest one. |
| **Compression Balancer** | **Greedy Algorithm** | Sorts compression blocks by their "savings-to-cost" ratio and greedily selects the highest ratios until the budget runs out, approximating the optimal Knapsack solution efficiently. |

---

## 3. Time and Space Complexity Table

| Component | Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Trie** | Insert | $O(L)$ where $L$ is word length | $O(C \times M)$ where $C$ is alphabet size, $M$ is total chars |
| **Trie** | Prefix Search | $O(P + N)$ where $P$ is prefix length, $N$ is subtrie nodes | - |
| **Stack** | Push / Pop | $O(1)$ | $O(A)$ where $A$ is number of stored actions |
| **Queue** | Enqueue / Dequeue | $O(1)$ | $O(D)$ where $D$ is documents in queue |
| **Hash Table** | Insert / Lookup | $O(1)$ average, $O(D)$ worst | $O(U)$ where $U$ is unique checksums |
| **Max Heap** | Insert / Extract Max | $O(\log D)$ | $O(D)$ where $D$ is total documents |
| **Graph** | Add Edge | $O(1)$ average | $O(V + E)$ where $V$=vertices, $E$=edges |
| **BFS** | Shortest Path | $O(V + E)$ | $O(V)$ for queue and visited sets |
| **Greedy Algo** | Balancer Selection | $O(D \log D)$ | $O(D)$ for building the queue |

---

## 4. 20 Viva Questions with Answers

**Q1: What is the primary advantage of using a Trie for the Naming Registry over a Hash Map?**
A: A Hash Map provides $O(1)$ exact matches but cannot efficiently return strings that share a common prefix. A Trie stores strings by prefix naturally, making autocomplete incredibly fast.

**Q2: Why use `std::unordered_set` instead of `std::set` for Checksum Lookup?**
A: `std::unordered_set` is implemented using a Hash Table (average $O(1)$ time), while `std::set` uses a balanced BST ($O(\log N)$ time). Hash Tables are faster for pure existence checks.

**Q3: How does the Hit Sorter maintain the top relevance scores?**
A: It utilizes a Priority Queue acting as a Max Heap. When an item is added, it "bubbles up" to maintain the heap property in $O(\log N)$ time. The highest score is always accessible at the root in $O(1)$ time.

**Q4: Can we detect duplicate documents using an Array instead of a Hash Table?**
A: Yes, but it's inefficient. Searching an unsorted array takes $O(N)$ time. A Hash Table accomplishes this in $O(1)$ average time.

**Q5: Why is an Adjacency List better than an Adjacency Matrix for the Citation Map?**
A: Document citations form a "sparse" graph (most documents only cite a few others). A matrix requires $O(V^2)$ space, while an adjacency list only requires $O(V + E)$ space, saving massive amounts of memory.

**Q6: Explain how Breadth-First Search (BFS) finds the shortest validation path.**
A: BFS explores all neighbors at the current depth before moving deeper. Because our citation graph is unweighted, the first time BFS encounters the target node, it is guaranteed to have taken the minimum number of edges (hops).

**Q7: Why didn't you use Depth-First Search (DFS) for the Validation Path?**
A: DFS dives deep into a path before backtracking. It might find a path to the target, but there is no guarantee it is the *shortest* path unless we check all paths, which is very inefficient.

**Q8: Explain the logic behind your Greedy Algorithm for the Compression Balancer.**
A: It treats compression like a Fractional Knapsack problem. It calculates the ratio of "savings / cost" for each block, sorts them in a Max Heap, and greedily selects blocks with the highest ratio until the processing budget is exhausted.

**Q9: Why cross-multiply instead of dividing when calculating the savings/cost ratio?**
A: Division can lead to floating-point inaccuracies or division-by-zero errors if cost is 0. Cross-multiplying (`savingsA * costB < savingsB * costA`) achieves the exact same comparison using safe integer arithmetic.

**Q10: What does the Correction Trail (Stack) achieve?**
A: It implements an "Undo" feature. By pushing actions onto a stack, the last action pushed is the first one popped, which perfectly mirrors user undo expectations.

**Q11: Why is the Compute Line managed by a Queue?**
A: Queue enforces FIFO (First-In-First-Out) behavior. In systems, files must be indexed in the order they arrive to maintain sequential consistency.

**Q12: In `TrieManager`, how does `collectAllWords` work?**
A: It's a recursive DFS traversal. Once it reaches the node representing the end of a prefix, it traverses all child branches, appending the characters and collecting any node marked as `isEndOfWord`.

**Q13: What happens if two documents have the same checksum?**
A: The Hash Table (`unordered_set`) detects that the checksum already exists in $O(1)$ time. The `uploadDocument` method intercepts this and halts the upload, flagging it as a duplicate.

**Q14: Are classes inside `main.cpp` good practice for production?**
A: No. In production, classes should be separated into `.h` headers and `.cpp` implementation files for modularity, reusability, and faster compilation. However, for this specific assignment requirement, they were consolidated.

**Q15: How does `std::priority_queue` know to act as a Max Heap for custom objects?**
A: We provided a custom comparator `struct DocumentCompare` that overloads the `operator()`. By returning `a.relevance < b.relevance`, we instruct the queue to bubble larger relevance scores to the top.

**Q16: What is the time complexity of building the Citation Map?**
A: Adding a document or edge to the unordered_map-based Adjacency List takes average $O(1)$ time. Thus, adding $E$ citations takes $O(E)$ time.

**Q17: Why do we convert strings to lowercase in the Trie?**
A: To ensure the Naming Registry is case-insensitive. "Auth" and "auth" will map to the exact same nodes, improving the user search experience.

**Q18: What is the space complexity of the StackManager?**
A: $O(A \times S)$ where $A$ is the number of stored string actions, and $S$ is the average length of the strings.

**Q19: How are the 20 sample documents processed on startup?**
A: They are placed into the `uploadDocument` method, added to the Compute Line (Queue), and then `processComputeLine()` is called. The queue dequeues them one by one and inserts them into the Trie, Hash Table, Heap, and Graph.

**Q20: Can you change the priority of the Hit Sorter?**
A: Yes. If we wanted an ascending sort (Min Heap), we would simply change the comparator to return `a.relevance > b.relevance`.

---

## 5. Step-by-Step Explanation of Program Flow

1. **Initialization:**
   * The program starts in `main()`, which instantiates the `EnterpriseKnowledgeSystem` facade.
   * `populateSampleData()` is called. 20 `Document` objects are created.
2. **Uploading (Queueing & Duplicate Checking):**
   * Each document is passed to `uploadDocument()`.
   * The system hashes the document and checks the `HashIndex` (Hash Table). If it's unique, it goes into the `QueueManager` (Compute Line).
3. **Indexing (Dequeuing & Routing):**
   * `processComputeLine()` is invoked. The queue empties sequentially.
   * Each document is mapped into the `TrieManager` (for names), `RankingSystem` (for scores), `GraphManager` (for graph nodes), and `CompressionOptimizer` (for future compression).
4. **Graph Edges (Citations):**
   * The initialization explicitly calls `addCitation()` to map links between related documents (e.g., Doc 13 cites Doc 1).
5. **Interactive Menu:**
   * A `do-while` loop starts, displaying the 14-option menu.
   * Based on the user's input (`switch` statement), the system routes the request to the specific subsystem (e.g., searching prefix calls the Trie, requesting Validation Path calls BFS on the Graph).
   * Actions like `changeSearchConfiguration` are pushed to the `StackManager`. If the user hits Undo, it is popped.
6. **Termination:**
   * The system runs until the user selects option 14 (Exit). The `while(choice != 14)` loop breaks, destructors are called (e.g., cleaning up Trie nodes), and the program ends.
