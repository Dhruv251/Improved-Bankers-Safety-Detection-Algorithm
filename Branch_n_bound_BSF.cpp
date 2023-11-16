#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <sstream>
#include <iterator>
#include <unordered_set>

using namespace std;

// Node structure for N-ary tree
struct Node
{
    string name;
    vector<Node *> children;
    Node *parentPointer;
    unordered_set<string> executed;
    int availableResource;
    int waitingTime;
    int burstTimeLastProcess;
    bool exist;

    Node(const string &s)
    {
        this->name = s;
        children = {};
        executed = unordered_set<string>();
        this->parentPointer = nullptr;
        this->availableResource = 0;
        this->waitingTime = 0;
        burstTimeLastProcess = 0;
        this->exist = true;
    }
};

void createNaryTree(Node *root, const vector<string> &processNames, const vector<int> &resourceReq, const vector<int> &allocated, const vector<int> &burstTime)
{
    queue<pair<Node *, int>> levelQueue; // Pair to store node and its level
    levelQueue.push({root, 0});          // Start with root at level 0

    int currentLevel = 0;
    list<Node *> currentLevelNodes;

    while (!levelQueue.empty())
    {
        Node *parent = levelQueue.front().first;
        int level = levelQueue.front().second;
        levelQueue.pop();

        if (!parent->exist)
        {
            cout << "Pruned node: " << parent->name << "\t";
            continue; // Skip processing non-existent nodes
        }

        if (level != currentLevel)
        {
            // Pruning logic
            cout << "\n ..........Next Level...........\n"
                 << endl;

            for (auto it = currentLevelNodes.begin(); it != currentLevelNodes.end();)
            {
                if (!(*it)->exist)
                {
                    it = currentLevelNodes.erase(it); // Erase pruned nodes
                    continue;
                }

                bool pruned = false; // Flag to track if node is pruned

                for (auto it2 = std::next(it); it2 != currentLevelNodes.end(); ++it2)
                {
                    if (!(*it2)->exist)
                        continue; // Skip already pruned nodes

                    Node *node1 = *it;
                    Node *node2 = *it2;

                    cout << node1 << " " << node2 << endl;

                    if (node1->availableResource <= node2->availableResource && node1->waitingTime > node2->waitingTime)
                    {
                        // Prune node1
                        node1->exist = false;
                        cout << "Pruned node: " << node1->name << endl;
                        pruned = true;
                        break; // No need to compare further
                    }
                }

                if (!pruned)
                {
                    ++it; // Move to the next node if not pruned
                }
            }

            currentLevel = level;
            currentLevelNodes.clear();
        }

        currentLevelNodes.push_back(parent);

        for (int i = 0; i < processNames.size(); ++i)
        {
            if (parent->executed.find(processNames[i]) == parent->executed.end() && parent->availableResource >= resourceReq[i])
            {
                Node *child = new Node(parent->name + " " + processNames[i]);
                parent->children.push_back(child);

                child->parentPointer = parent;
                child->availableResource = parent->availableResource + allocated[i];
                child->executed = parent->executed;
                child->executed.insert(processNames[i]);
                child->burstTimeLastProcess = burstTime[i];

                // new waiting time
                int addTime = parent->burstTimeLastProcess;
                child->waitingTime = parent->waitingTime + addTime;
                levelQueue.push({child, level + 1}); // Add child with incremented level
                                                     // print descrption of the node:

                cout << "Description of : " << child->name << "\n";
                cout << "{ " << child << ":  " << child->availableResource << ", " << child->waitingTime << ", " << child->burstTimeLastProcess << " ," << level + 1 << " exist:" << child->exist << "}" << endl
                     << endl;
            }
        }
    }
    
}

// Debugging Function print connected Nodes
void printConnectedNodes(Node *);

// Debugging Function to print level order traversal
void levelOrderTraversal(Node *);

// Debugging Function to print nodes at each level
void printNodesAtEachLevel(const Node &);

int main()
{
    // Process names
    vector<string> processNames = {"P0", "P1", "P2", "P3"};
    vector<int> burstTime = {15, 3, 4, 7};
    vector<int> resources_reqd = {6, 5, 4, 5};
    vector<int> allocated = {2, 4, 3, 3};

    // Create the root node
    string rootName = "r";
    Node *root = new Node(rootName);
    root->availableResource = 8;

    // Create the N-ary tree based on the given logic
    createNaryTree(root, processNames, resources_reqd, allocated, burstTime);

    // perform level order traversal
    levelOrderTraversal(root);

    cout << "FINISHED\n";

    return 0;
}

void levelOrderTraversal(Node *root)
{
    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        int levelSize = q.size();

        for (int i = 0; i < levelSize; ++i)
        {
            Node *current = q.front();
            q.pop();

            if (current->exist)
            {
                cout << current->name << " { "
                     << " wT:" << current->waitingTime << "}";

                for (Node *child : current->children)
                    q.push(child);
            }
        }

        cout << endl;
    }
}