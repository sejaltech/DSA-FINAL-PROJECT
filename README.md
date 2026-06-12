# DSA-FINAL-PROJECT

# Enterprise Knowledge Documentation Core

## 2.1 Project Title

**Enterprise Knowledge Documentation Core Using Data Structures and Algorithms**

---

## 2.2 Problem Statement

Large IT organizations generate massive amounts of documentation including technical specifications, project reports, HR policies, meeting notes, and design documents. Over time, this information becomes scattered across millions of files, making it difficult for employees to locate relevant knowledge quickly.

The objective of this project is to design and implement an **Enterprise Knowledge Documentation Core** that enables efficient document storage, indexing, searching, ranking, validation, and optimization using fundamental Data Structures and Algorithms (DSA).

The system supports instant document search, duplicate detection, document relationship mapping, validation path discovery, ranking of search results, and storage optimization.

---

## 2.3 Objectives

The primary objectives of this project are:

* Provide fast document name search using prefix matching.
* Maintain configuration history and support undo operations.
* Process uploaded documents in FIFO order.
* Detect duplicate documents efficiently using checksums.
* Rank search results based on relevance scores.
* Model document references through a citation network.
* Find the shortest validation path between documents.
* Optimize storage compression using a greedy strategy.
* Demonstrate practical applications of multiple DSA concepts in a real-world enterprise system.

---

## 2.4 System Overview / Architecture

### System Workflow

1. Documents are uploaded into the system.
2. Duplicate documents are checked using Checksum Lookup.
3. Valid documents enter the Compute Line Queue.
4. Documents are processed and indexed.
5. File names are stored in a Trie for fast searching.
6. Relevance scores are stored in a Priority Queue for ranking.
7. Citations are stored in a Graph structure.
8. Compression candidates are evaluated using a Greedy Algorithm.
9. Users can search, rank, validate, and optimize documents through the menu-driven interface.

### Architecture Diagram

```text
                +----------------+
                | Document Upload|
                +--------+-------+
                         |
                         v
               +------------------+
               | Checksum Lookup  |
               | (Hash Table)     |
               +--------+---------+
                        |
                        v
               +------------------+
               | Compute Line     |
               | (Queue)          |
               +--------+---------+
                        |
          +-------------+-------------+
          |             |             |
          v             v             v

     +---------+   +---------+   +---------+
     | Trie    |   | Heap    |   | Graph   |
     | Search  |   | Ranking |   | Citation|
     +---------+   +---------+   +---------+

                        |
                        v

               +------------------+
               | Compression      |
               | Optimizer        |
               +------------------+
```

---

## 2.5 Data Structures and Algorithms Used

| Feature              | Data Structure / Algorithm        | Purpose                       |
| -------------------- | --------------------------------- | ----------------------------- |
| Naming Registry      | Trie                              | Prefix-based document search  |
| Correction Trail     | Stack                             | Undo configuration changes    |
| Compute Line         | Queue                             | FIFO document processing      |
| Checksum Lookup      | Hash Table (unordered_set)        | Duplicate detection           |
| Hit Sorter           | Priority Queue (Max Heap)         | Rank documents by relevance   |
| Citation Map         | Graph (Adjacency List)            | Represent document references |
| Validation Path      | Breadth First Search (BFS)        | Find shortest citation path   |
| Compression Balancer | Greedy Algorithm + Priority Queue | Maximize storage savings      |

---

## 2.6 Implementation Approach

### 1. Naming Registry (Trie)

* Document names are inserted into a Trie.
* Prefix searches return all matching documents.
* Example:

  * Input: "auth"
  * Output:

    * Authentication Guide
    * Authorization Policies

### 2. Correction Trail (Stack)

* Configuration changes are pushed onto a stack.
* Undo operation removes the latest change.

### 3. Compute Line (Queue)

* New documents enter a queue.
* Documents are processed in upload order.

### 4. Checksum Lookup (Hash Table)

* Every document contains a unique checksum.
* Duplicate files are detected in O(1) average time.

### 5. Hit Sorter (Max Heap)

* Documents are ranked according to relevance score.
* Highest score appears first.

### 6. Citation Map (Graph)

* Documents are represented as vertices.
* Citations are represented as edges.

### 7. Validation Path (BFS)

* BFS finds the shortest path between two documents.
* Useful for tracking document dependencies.

### 8. Compression Balancer (Greedy Algorithm)

* Documents are selected based on maximum savings-to-cost ratio.
* Produces efficient storage optimization.

---

## 2.7 Time and Space Complexity Analysis

| Operation                | Time Complexity | Space Complexity |
| ------------------------ | --------------- | ---------------- |
| Trie Insert              | O(L)            | O(L)             |
| Trie Search              | O(P + N)        | O(N)             |
| Stack Push               | O(1)            | O(A)             |
| Stack Pop                | O(1)            | O(A)             |
| Queue Enqueue            | O(1)            | O(D)             |
| Queue Dequeue            | O(1)            | O(D)             |
| Hash Insert              | O(1) Avg        | O(D)             |
| Hash Search              | O(1) Avg        | O(D)             |
| Heap Insert              | O(log D)        | O(D)             |
| Heap Extract             | O(log D)        | O(D)             |
| Graph Edge Insert        | O(1) Avg        | O(V + E)         |
| BFS Traversal            | O(V + E)        | O(V)             |
| Compression Optimization | O(D log D)      | O(D)             |

Where:

* L = Length of document name
* P = Prefix length
* N = Nodes in Trie subtree
* D = Number of documents
* V = Number of vertices
* E = Number of edges
* A = Number of actions

---

## 2.8 Execution Steps

### Prerequisites

* C++ Compiler (GCC/G++)
* C++11 or above

### Compile

```bash
g++ main.cpp -o EnterpriseKnowledgeSystem
```

### Run

```bash
./EnterpriseKnowledgeSystem
```

### Menu Operations

```text
1. Add Document
2. Search Document by Prefix
3. Upload Document to Queue
4. Process Indexing Queue
5. Check Duplicate Document
6. Change Search Configuration
7. Undo Configuration Change
8. Add Citation Link
9. Display Citation Graph
10. Find Shortest Validation Path
11. Rank Search Results
12. Run Compression Optimizer
13. Display All Documents
14. Exit
```

---

## 2.9 Sample Inputs and Outputs

### Prefix Search

Input:

```text
auth
```

Output:

```text
Authentication Guide
Authorization Policies
```

### Ranking Results

Output:

```text
Rank 1 | Score: 100 | Getting Started
Rank 2 | Score: 99 | API Reference
Rank 3 | Score: 98 | Security Best Practices
```

### Validation Path

Input:

```text
Start ID : 13
Target ID : 19
```

Output:

```text
13 -> 1 -> 19
```

### Compression Optimizer

Input:

```text
Budget = 50
```

Output:

```text
Total Savings Achieved: 280 units
Selected Documents:
CI/CD Pipeline
Architecture Overview
Authentication Guide
```

---

## 2.10 Screenshots

### Main Menu

<img width="391" height="289" alt="Screenshot 2026-06-12 at 8 34 13 PM" src="https://github.com/user-attachments/assets/9a0a73ce-35d6-46eb-b03c-a6b070fbecfc" />

### Trie Search Results

<img width="381" height="118" alt="Screenshot 2026-06-12 at 8 35 20 PM" src="https://github.com/user-attachments/assets/37467fa3-a466-4e59-816b-c83583e78616" />


### Citation Graph

<img width="374" height="374" alt="Screenshot 2026-06-12 at 8 36 15 PM" src="https://github.com/user-attachments/assets/4f65483c-e31a-4583-82a7-00cf19689171" />


### Ranking System

<img width="432" height="351" alt="Screenshot 2026-06-12 at 8 36 39 PM" src="https://github.com/user-attachments/assets/b816fbe6-1bf6-4f98-b4c1-01fcdc88a65e" />


### Compression Optimizer

<img width="462" height="187" alt="Screenshot 2026-06-12 at 8 37 10 PM" src="https://github.com/user-attachments/assets/b095ce44-7665-4548-ab5a-f76938ce4b7d" />

### Validation Path

<img width="382" height="120" alt="Screenshot 2026-06-12 at 8 38 13 PM" src="https://github.com/user-attachments/assets/b3a223e5-a77d-4dbc-b499-f68974807108" />


---

## 2.11 Results and Observations

* Successfully implemented eight major DSA concepts in a single enterprise application.
* Trie provided efficient prefix searching.
* Hash Table enabled constant-time duplicate detection.
* Priority Queue correctly ranked documents by relevance.
* BFS efficiently found shortest citation paths.
* Queue maintained strict document processing order.
* Stack supported instant rollback of configuration changes.
* Greedy compression strategy achieved high storage savings within budget constraints.

---

## 2.12 Conclusion

The Enterprise Knowledge Documentation Core successfully demonstrates how classical Data Structures and Algorithms can solve real-world enterprise knowledge management problems. The system integrates Tries, Stacks, Queues, Hash Tables, Priority Queues, Graphs, BFS, and Greedy Algorithms into a unified platform that supports efficient document indexing, searching, ranking, validation, and optimization.

This project highlights the practical significance of DSA in building scalable enterprise-level software systems and provides a strong foundation for future enhancements such as full-text search, database integration, machine learning-based ranking, and distributed indexing.


## Author

Sejal Singh 
