#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

// ==========================================
// 0. DOCUMENT CLASS
// ==========================================
class Document {
public:
    int id;
    string name;
    string path;
    string checksum;
    int relevanceScore;
    int compressionCost;
    int compressionSavings;

    Document() : id(0), relevanceScore(0), compressionCost(0), compressionSavings(0) {}

    Document(int id, string name, string path, string checksum, int relevanceScore, int cost, int savings)
        : id(id), name(name), path(path), checksum(checksum), relevanceScore(relevanceScore), 
          compressionCost(cost), compressionSavings(savings) {}
};

// ==========================================
// 1. NAMING REGISTRY (TRIE)
// ==========================================
// Time Complexity: Insertion: O(L), Search/Autocomplete: O(P + N) where L is name length, P is prefix length, N is nodes in subtrie.
// Space Complexity: O(C * M) where C is alphabet size, M is total chars across all names.
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    Document doc;
    TrieNode() : isEndOfWord(false) {}
};

class TrieManager {
private:
    TrieNode* root;

    void collectAllWords(TrieNode* node, const string& prefix, vector<Document>& results) {
        if (node->isEndOfWord) results.push_back(node->doc);
        for (auto& pair : node->children) {
            collectAllWords(pair.second, prefix + pair.first, results);
        }
    }

    void destroyTrie(TrieNode* node) {
        if (!node) return;
        for (auto& pair : node->children) destroyTrie(pair.second);
        delete node;
    }

public:
    TrieManager() { root = new TrieNode(); }
    ~TrieManager() { destroyTrie(root); }

    void insert(const Document& doc) {
        TrieNode* current = root;
        string nameLower = doc.name;
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

        for (char c : nameLower) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEndOfWord = true;
        current->doc = doc;
    }

    vector<Document> getAutocompleteSuggestions(const string& prefix) {
        TrieNode* current = root;
        vector<Document> results;
        string prefixLower = prefix;
        transform(prefixLower.begin(), prefixLower.end(), prefixLower.begin(), ::tolower);

        for (char c : prefixLower) {
            if (current->children.find(c) == current->children.end()) return results;
            current = current->children[c];
        }
        collectAllWords(current, prefixLower, results);
        return results;
    }
};

// ==========================================
// 2. CORRECTION TRAIL (STACK)
// ==========================================
// Time Complexity: Push: O(1), Pop (Undo): O(1).
// Space Complexity: O(A) where A is the number of stored actions.
class StackManager {
private:
    stack<string> historyStack;

public:
    void pushAction(const string& action) {
        historyStack.push(action);
    }

    string undoLastAction() {
        if (historyStack.empty()) return "History is empty. Nothing to undo.";
        string lastAction = historyStack.top();
        historyStack.pop();
        return lastAction;
    }

    void displayHistory() const {
        if (historyStack.empty()) {
            cout << "Correction Trail is empty.\n";
            return;
        }
        stack<string> tempStack = historyStack;
        cout << "\n--- Search Configuration History ---\n";
        int count = 1;
        while (!tempStack.empty()) {
            cout << count++ << ". " << tempStack.top() << "\n";
            tempStack.pop();
        }
        cout << "------------------------------------\n";
    }
};

// ==========================================
// 3. COMPUTE LINE (QUEUE)
// ==========================================
// Time Complexity: Enqueue: O(1), Dequeue: O(1).
// Space Complexity: O(D) where D is the number of documents in queue.
class QueueManager {
private:
    queue<Document> processingQueue;

public:
    void enqueueDocument(const Document& doc) {
        processingQueue.push(doc);
    }

    bool processNextDocument(Document& outDoc) {
        if (processingQueue.empty()) return false;
        outDoc = processingQueue.front();
        processingQueue.pop();
        return true;
    }

    bool isEmpty() const {
        return processingQueue.empty();
    }
};

// ==========================================
// 4. CHECKSUM LOOKUP (HASH TABLE)
// ==========================================
// Time Complexity: Insert: O(1) avg, Check: O(1) avg.
// Space Complexity: O(D) where D is the number of unique checksums.
class HashIndex {
private:
    unordered_set<string> checksums;

public:
    bool insertChecksum(const string& checksum) {
        auto result = checksums.insert(checksum);
        return result.second;
    }

    bool isDuplicate(const string& checksum) const {
        return checksums.find(checksum) != checksums.end();
    }
};

// ==========================================
// 5. HIT SORTER (PRIORITY QUEUE / MAX HEAP)
// ==========================================
// Time Complexity: Insert: O(log D), Get Max: O(1), Extract Max: O(log D).
// Space Complexity: O(D).
struct DocumentCompare {
    bool operator()(const Document& a, const Document& b) {
        return a.relevanceScore < b.relevanceScore; // Max heap based on relevance
    }
};

class RankingSystem {
private:
    priority_queue<Document, vector<Document>, DocumentCompare> hitQueue;

public:
    void addHit(const Document& doc) {
        hitQueue.push(doc);
    }

    void displayRankings() const {
        if (hitQueue.empty()) {
            cout << "No hits available for ranking.\n";
            return;
        }
        priority_queue<Document, vector<Document>, DocumentCompare> tempQueue = hitQueue;
        cout << "\n--- Document Hit Rankings (Max Heap) ---\n";
        int rank = 1;
        while (!tempQueue.empty()) {
            Document doc = tempQueue.top();
            cout << "Rank " << rank++ << " | Score: " << doc.relevanceScore 
                 << " | ID: " << doc.id << " | Name: " << doc.name << "\n";
            tempQueue.pop();
        }
        cout << "----------------------------------------\n";
    }
};

// ==========================================
// 6. CITATION MAP (GRAPH) & 7. VALIDATION PATH (BFS)
// ==========================================
// Time Complexity: Add Edge: O(1) avg. BFS: O(V + E).
// Space Complexity: O(V + E) where V = documents, E = citations.
class GraphManager {
private:
    unordered_map<int, vector<int>> adjList;

public:
    void addDocument(int docId) {
        if (adjList.find(docId) == adjList.end()) {
            adjList[docId] = vector<int>();
        }
    }

    void addCitation(int docId1, int docId2) {
        addDocument(docId1);
        addDocument(docId2);
        auto& neighbors = adjList[docId1];
        if (find(neighbors.begin(), neighbors.end(), docId2) == neighbors.end()) {
            neighbors.push_back(docId2);
        }
    }

    void displayCitationMap() const {
        cout << "\n--- Citation Graph (Adjacency List) ---\n";
        for (const auto& pair : adjList) {
            cout << "Document ID " << pair.first << " cites: ";
            if (pair.second.empty()) cout << "None";
            else for (int neighbor : pair.second) cout << neighbor << " ";
            cout << "\n";
        }
        cout << "---------------------------------------\n";
    }

    void findValidationPath(int startId, int targetId) {
        if (adjList.find(startId) == adjList.end() || adjList.find(targetId) == adjList.end()) {
            cout << "One or both documents are not in the graph.\n";
            return;
        }

        unordered_map<int, int> parentMap;
        unordered_set<int> visited;
        queue<int> q;

        q.push(startId);
        visited.insert(startId);
        parentMap[startId] = -1;

        bool found = false;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (current == targetId) {
                found = true;
                break;
            }

            for (int neighbor : adjList[current]) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    parentMap[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }

        cout << "\n--- Shortest Validation Path (BFS) ---\n";
        if (found) {
            vector<int> path;
            int curr = targetId;
            while (curr != -1) {
                path.push_back(curr);
                curr = parentMap[curr];
            }
            reverse(path.begin(), path.end());
            cout << "Path: ";
            for (size_t i = 0; i < path.size(); ++i) {
                cout << path[i];
                if (i < path.size() - 1) cout << " -> ";
            }
            cout << "\n";
        } else {
            cout << "No reference path exists between Document " << startId << " and " << targetId << ".\n";
        }
        cout << "--------------------------------------\n";
    }
};

// ==========================================
// 8. COMPRESSION BALANCER (GREEDY ALGORITHM)
// ==========================================
// Time Complexity: PQ build: O(D log D), Greedy Selection: O(K log D).
// Space Complexity: O(D).
struct CompressionCompare {
    bool operator()(const Document& a, const Document& b) {
        // Cross multiply to avoid division by zero: sort by savings/cost ratio (Max Heap)
        return (a.compressionSavings * b.compressionCost) < (b.compressionSavings * a.compressionCost);
    }
};

class CompressionOptimizer {
private:
    priority_queue<Document, vector<Document>, CompressionCompare> compressionQueue;

public:
    void addBlock(const Document& doc) {
        if (doc.compressionCost > 0) {
            compressionQueue.push(doc);
        }
    }

    void optimizeCompression(int maxCostBudget) {
        priority_queue<Document, vector<Document>, CompressionCompare> tempQueue = compressionQueue;
        int currentCost = 0;
        int totalSavings = 0;
        vector<Document> selected;

        while (!tempQueue.empty()) {
            Document topDoc = tempQueue.top();
            tempQueue.pop();

            if (currentCost + topDoc.compressionCost <= maxCostBudget) {
                currentCost += topDoc.compressionCost;
                totalSavings += topDoc.compressionSavings;
                selected.push_back(topDoc);
            }
        }

        cout << "\n--- Compression Balancer (Greedy Algorithm) ---\n";
        cout << "Budget allocated: " << maxCostBudget << "\n";
        cout << "Total Savings Achieved: " << totalSavings << " units\n";
        cout << "Blocks selected for compression:\n";
        for (const auto& doc : selected) {
            cout << "- Doc ID: " << doc.id << " (" << doc.name << ") | Cost: " << doc.compressionCost 
                 << " | Savings: " << doc.compressionSavings << "\n";
        }
        cout << "-----------------------------------------------\n";
    }
};

// ==========================================
// 9. ENTERPRISE KNOWLEDGE SYSTEM FACADE
// ==========================================
class EnterpriseKnowledgeSystem {
private:
    TrieManager namingRegistry;
    StackManager correctionTrail;
    QueueManager computeLine;
    HashIndex checksumLookup;
    RankingSystem hitSorter;
    GraphManager citationMap;
    CompressionOptimizer compressionBalancer;
    unordered_map<int, Document> database; // Central storage

public:
    // Feature 3: Queue document for indexing
    void uploadDocument(const Document& doc) {
        // Feature 4: Checksum Hash Table check
        if (checksumLookup.isDuplicate(doc.checksum)) {
            cout << "Duplicate document detected (checksum collision): " << doc.name << "\n";
            return;
        }
        computeLine.enqueueDocument(doc);
        cout << "Document added to Compute Line (Queue): " << doc.name << "\n";
    }

    // Process the Queue and index into all DSAs
    void processComputeLine() {
        Document doc;
        int count = 0;
        while (computeLine.processNextDocument(doc)) {
            database[doc.id] = doc;
            namingRegistry.insert(doc);
            checksumLookup.insertChecksum(doc.checksum);
            hitSorter.addHit(doc);
            citationMap.addDocument(doc.id);
            compressionBalancer.addBlock(doc);
            count++;
        }
        cout << "Processed and indexed " << count << " documents from the Compute Line.\n";
    }

    // Feature 1: Trie Search
    void searchDocuments(const string& prefix) {
        vector<Document> results = namingRegistry.getAutocompleteSuggestions(prefix);
        cout << "\n--- Search Results for prefix '" << prefix << "' (Trie) ---\n";
        if (results.empty()) cout << "No documents found.\n";
        else {
            for (const auto& doc : results) {
                cout << "- " << doc.name << " (Path: " << doc.path << ")\n";
            }
        }
        cout << "--------------------------------------------------\n";
    }

    // Feature 2: Change Config & Undo (Stack)
    void changeSearchConfiguration(const string& newConfig) {
        correctionTrail.pushAction(newConfig);
        cout << "Configuration changed to: " << newConfig << "\n";
    }

    void undoLastAction() {
        string action = correctionTrail.undoLastAction();
        cout << "Undid Action. Removed Configuration: " << action << "\n";
    }

    void displayAllDocuments() const {
        cout << "\n--- All Indexed Documents ---\n";
        for (const auto& pair : database) {
            cout << "ID: " << pair.second.id << " | Name: " << pair.second.name << "\n";
        }
        cout << "-----------------------------\n";
    }

    void addCitation(int id1, int id2) {
        citationMap.addCitation(id1, id2);
        cout << "Citation added: Doc " << id1 << " cites Doc " << id2 << ".\n";
    }

    void displayCitationGraph() const { citationMap.displayCitationMap(); }
    void findValidationPath(int id1, int id2) { citationMap.findValidationPath(id1, id2); }
    void rankSearchResults() const { hitSorter.displayRankings(); }
    void runCompressionOptimizer(int budget) { compressionBalancer.optimizeCompression(budget); }
};

// ==========================================
// 10. MAIN FUNCTION & MENU
// ==========================================
void populateSampleData(EnterpriseKnowledgeSystem& system) {
    cout << "Initializing system with 20 sample documents...\n";
    
    // id, name, path, checksum, relevance, cost, savings
    Document docs[20] = {
        {1, "Authentication Guide", "/docs/auth_guide.md", "hash1", 95, 10, 50},
        {2, "Authorization Policies", "/docs/auth_policy.md", "hash2", 88, 15, 60},
        {3, "API Reference", "/docs/api_ref.md", "hash3", 99, 20, 100},
        {4, "Architecture Overview", "/docs/arch_over.md", "hash4", 92, 25, 120},
        {5, "Backend Setup", "/docs/backend.md", "hash5", 85, 12, 45},
        {6, "Billing Integration", "/docs/billing.md", "hash6", 75, 18, 70},
        {7, "Caching Strategy", "/docs/cache.md", "hash7", 82, 8, 30},
        {8, "CI/CD Pipeline", "/docs/cicd.md", "hash8", 90, 30, 150},
        {9, "Database Schema", "/docs/db_schema.md", "hash9", 96, 22, 110},
        {10, "Data Migration", "/docs/db_mig.md", "hash10", 78, 14, 55},
        {11, "Error Handling", "/docs/error.md", "hash11", 84, 9, 35},
        {12, "Frontend Framework", "/docs/frontend.md", "hash12", 89, 16, 65},
        {13, "Getting Started", "/docs/getting_started.md", "hash13", 100, 5, 20},
        {14, "GraphQL Endpoints", "/docs/gql.md", "hash14", 86, 11, 40},
        {15, "Load Balancing", "/docs/load.md", "hash15", 81, 13, 50},
        {16, "Logging Standards", "/docs/logging.md", "hash16", 79, 7, 25},
        {17, "Microservices Guide", "/docs/micro.md", "hash17", 94, 28, 140},
        {18, "Mobile App Auth", "/docs/mob_auth.md", "hash18", 87, 10, 48},
        {19, "Security Best Practices", "/docs/sec_best.md", "hash19", 98, 19, 90},
        {20, "WebSocket Setup", "/docs/ws.md", "hash20", 72, 15, 58}
    };

    for (int i = 0; i < 20; ++i) system.uploadDocument(docs[i]);
    
    // Process them to build indices
    system.processComputeLine();

    // Add citations (Edges)
    system.addCitation(13, 1);  // Getting Started -> Auth Guide
    system.addCitation(1, 2);   // Auth Guide -> Auth Policies
    system.addCitation(4, 9);   // Architecture -> Database Schema
    system.addCitation(9, 10);  // Database -> Migration
    system.addCitation(4, 17);  // Architecture -> Microservices
    system.addCitation(17, 3);  // Microservices -> API
    system.addCitation(3, 11);  // API -> Error Handling
    system.addCitation(1, 19);  // Auth Guide -> Security
    system.addCitation(13, 12); // Getting started -> Frontend
    cout << "Initialization complete!\n";
}

int main() {
    EnterpriseKnowledgeSystem system;
    populateSampleData(system);

    int choice;
    do {
        cout << "\n===============================================\n";
        cout << " ENTERPRISE KNOWLEDGE DOCUMENTATION CORE MENU\n";
        cout << "===============================================\n";
        cout << "1. Add Document\n";
        cout << "2. Search Document by Prefix\n";
        cout << "3. Upload Document to Queue\n";
        cout << "4. Process Indexing Queue\n";
        cout << "5. Check Duplicate Document (simulate hash check)\n";
        cout << "6. Change Search Configuration\n";
        cout << "7. Undo Configuration Change\n";
        cout << "8. Add Citation Link\n";
        cout << "9. Display Citation Graph\n";
        cout << "10. Find Shortest Validation Path\n";
        cout << "11. Rank Search Results\n";
        cout << "12. Run Compression Optimizer\n";
        cout << "13. Display All Documents\n";
        cout << "14. Exit\n";
        cout << "===============================================\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (choice) {
            case 1:
            case 3: { // Both 1 and 3 map to uploading to queue based on prompt logic
                int id, rel, cost, sav;
                string name, path, chk;
                cout << "Enter ID: "; cin >> id; cin.ignore();
                cout << "Enter Name: "; getline(cin, name);
                cout << "Enter Path: "; getline(cin, path);
                cout << "Enter Checksum: "; cin >> chk;
                cout << "Enter Relevance Score: "; cin >> rel;
                cout << "Enter Compression Cost: "; cin >> cost;
                cout << "Enter Compression Savings: "; cin >> sav;
                system.uploadDocument(Document(id, name, path, chk, rel, cost, sav));
                break;
            }
            case 2: {
                string prefix;
                cout << "Enter search prefix (e.g., 'auth'): "; cin >> prefix;
                system.searchDocuments(prefix);
                break;
            }
            case 4:
                system.processComputeLine();
                break;
            case 5: {
                cout << "(Handled internally during Upload. To test, upload a document with an existing checksum like 'hash1').\n";
                break;
            }
            case 6: {
                string config;
                cout << "Enter new search configuration (e.g., 'SortBy=Date'): ";
                cin.ignore();
                getline(cin, config);
                system.changeSearchConfiguration(config);
                break;
            }
            case 7:
                system.undoLastAction();
                break;
            case 8: {
                int id1, id2;
                cout << "Enter Citing Document ID: "; cin >> id1;
                cout << "Enter Cited Document ID: "; cin >> id2;
                system.addCitation(id1, id2);
                break;
            }
            case 9:
                system.displayCitationGraph();
                break;
            case 10: {
                int id1, id2;
                cout << "Enter Start Document ID: "; cin >> id1;
                cout << "Enter Target Document ID: "; cin >> id2;
                system.findValidationPath(id1, id2);
                break;
            }
            case 11:
                system.rankSearchResults();
                break;
            case 12: {
                int budget;
                cout << "Enter max compression cost budget: "; cin >> budget;
                system.runCompressionOptimizer(budget);
                break;
            }
            case 13:
                system.displayAllDocuments();
                break;
            case 14:
                cout << "Exiting system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please select between 1-14.\n";
        }
    } while (choice != 14);

    return 0;
}
