#include <iostream>
#include <string>
using namespace std;

struct RopeNode {
    string value;       // The actual string data (only for leaf nodes)
    int weight;         // Weight of the node (length of left subtree for internal nodes)
    RopeNode* left;     // Left child node
    RopeNode* right;    // Right child node

    // Constructor for creating a node
    RopeNode(int w, string val = "", RopeNode* l = nullptr, RopeNode* r = nullptr)
        : weight(w), value(val), left(l), right(r) {}
};

// Class to represent the Rope data structure
class Rope {
private:
    RopeNode* root;

    // Helper function to recursively find the node at a specific index
    void findNodeAtIndex(RopeNode* node, int index, RopeNode*& result, int& localIndex) {
        if (!node) return;

        if (!node->left && !node->right) {  // Leaf node
            result = node;
            localIndex = index;
            return;
        }

        if (index < node->weight) {
            findNodeAtIndex(node->left, index, result, localIndex);
        } else {
            findNodeAtIndex(node->right, index - node->weight, result, localIndex);
        }
    }

    // In-order traversal from start to end node to collect the substring
    void collectSubstring(RopeNode* node, int start, int end, string& result) {
        if (!node) return;

        // Check if this node is within the range and leaf
        if (!node->left && !node->right) {
            result += node->value.substr(start, end - start + 1);
            return;
        }

        // Traverse left subtree
        if (node->left) {
            collectSubstring(node->left, start, end, result);
        }

        // Traverse right subtree if necessary
        if (node->right) {
            collectSubstring(node->right, start - node->weight, end - node->weight, result);
        }
    }

public:
    // Constructor to create a rope with an initial string
    Rope(const string& str) {
        root = new RopeNode(str.length(), str);
    }

    // Function to report (extract) a substring between two indices
    string report(int start, int end) {
        RopeNode* startNode = nullptr;
        RopeNode* endNode = nullptr;
        int startIndex, endIndex;

        // Find the start node and local index
        findNodeAtIndex(root, start, startNode, startIndex);

        // Find the end node and local index
        findNodeAtIndex(root, end, endNode, endIndex);

        // Collect substring between start and end nodes
        string result;
        collectSubstring(root, startIndex, endIndex, result);

        return result;
    }
};

int main() {
    // Create a rope with an initial string
    Rope rope("hello i am a rope datastructure");

    // Extract a substring from position 2 to 9
    int start = 1;
    int end = 16;
    string extracted = rope.report(start-1, end-1);

    // Display the result
    cout << "Extracted Substring: " << extracted << endl;

    return 0;
}
