#include <iostream>
#include <string>
using namespace std;

// Define a RopeNode structure
struct RopeNode {
    string value;     // The actual string data (only for leaf nodes)
    int weight;       // Weight of the node
    RopeNode* left;   // Left child node
    RopeNode* right;  // Right child node

    // Constructor for creating a node
    RopeNode(int w, string val = "", RopeNode* l = nullptr, RopeNode* r = nullptr)
        : weight(w), value(val), left(l), right(r) {}
};

// Recursive function to find the character at index i in the rope
char index(RopeNode* node, int i) {
    // Base case: if node is null, return a null character
    if (node == nullptr) return '\0';

    // If we're in an internal node with weight less than i, we should go right
    if (node->weight <= i && node->right != nullptr) {
        return index(node->right, i - node->weight); // Update i for the right side
    }

    // If we're in an internal node and need to go left
    if (node->left != nullptr) {
        return index(node->left, i);
    }

    // If we're at a leaf node, return the character at index i
    return node->value[i];
}

int main() {
    // Example usage of RopeNode and index function

    // Create leaf nodes
    RopeNode* leaf1 = new RopeNode(4, "Data");      // Left leaf with "Data"
    RopeNode* leaf2 = new RopeNode(5, "Struc");     // Left of right subtree with "Struc"
    RopeNode* leaf3 = new RopeNode(5, "tures");     // Right leaf with "tures"

    // Create internal nodes
    RopeNode* rightNode = new RopeNode(5, "", leaf2, leaf3); // Node with weight 5, combining "Struc" and "tures"
    RopeNode* root = new RopeNode(4, "", leaf1, rightNode);  // Root node with weight 4, combining "Data" and the right subtree

    // Test the index function
    int indexToFind = 7;
    char result = index(root, indexToFind);

    if (result != '\0') {
        cout << "Character at index " << indexToFind << " is: " << result << endl;
    } else {
        cout << "Index out of bounds!" << endl;
    }

    // Clean up memory
    delete leaf1;
    delete leaf2;
    delete leaf3;
    delete rightNode;
    delete root;

    return 0;
}
