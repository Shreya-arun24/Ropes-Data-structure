#include <iostream>
#include <string>
using namespace std;

// Define a RopeNode structure
struct RopeNode {
    string value;      // String data (for leaf nodes)
    int weight;        // Weight of the node
    RopeNode* left;    // Left child node
    RopeNode* right;   // Right child node

    // Constructor for leaf node
    RopeNode(string val) : value(val), weight(val.size()), left(nullptr), right(nullptr) {}

    // Constructor for internal node
    RopeNode(RopeNode* leftNode, RopeNode* rightNode) : left(leftNode), right(rightNode) {
        weight = (leftNode ? leftNode->weight : 0);  // Internal node weight is left child's weight
        value = ""; // Internal nodes don't hold string value
    }

    // Retrieve the full string of the rope by concatenating leaf nodes
    string getString() const {
        if (!left && !right) return value; // If it's a leaf, return the value
        string result = "";
        if (left) result += left->getString();
        if (right) result += right->getString();
        return result;
    }
};

// Function to split a leaf node at a specific position
pair<RopeNode*, RopeNode*> splitLeaf(RopeNode* leaf, int splitPos) {
    // Split the string in the leaf node into two parts
    string leftPart = leaf->value.substr(0, splitPos);
    string rightPart = leaf->value.substr(splitPos);

    // Create new leaf nodes for both parts
    RopeNode* leftNode = new RopeNode(leftPart);
    RopeNode* rightNode = new RopeNode(rightPart);

    return {leftNode, rightNode};
}

// Function to split a rope at the given index
pair<RopeNode*, RopeNode*> splitRope(RopeNode* root, int index) {
    if (root->left == nullptr && root->right == nullptr) {
        // If root is a leaf node, split it at the index
        return splitLeaf(root, index);
    }

    // Traverse down the tree to find the split point
    if (index < root->weight) {
        // Split in the left subtree
        auto splitResult = splitRope(root->left, index);
        
        // Create two new ropes after split
        RopeNode* leftRope = splitResult.first;
        RopeNode* rightRope = new RopeNode(splitResult.second, root->right);

        return {leftRope, rightRope};
    } else {
        // Split in the right subtree
        auto splitResult = splitRope(root->right, index - root->weight);

        // Create two new ropes after split
        RopeNode* leftRope = new RopeNode(root->left, splitResult.first);
        RopeNode* rightRope = splitResult.second;

        return {leftRope, rightRope};
    }
}

int main() {
    // Create initial ropes
    RopeNode* leaf1 = new RopeNode("hello i am a");
    RopeNode* leaf2 = new RopeNode(" rope data structure");
    RopeNode* root = new RopeNode(leaf1, leaf2);

    cout << "Original Rope: " << root->getString() << endl;

    // Split the rope at index 11 (after "hello_i_am_a")
    int splitIndex = 11;
    auto splitResult = splitRope(root, splitIndex);

    cout << "Left Rope after Split: " << splitResult.first->getString() << endl;
    cout << "Right Rope after Split: " << splitResult.second->getString() << endl;

    // Clean up memory
    delete leaf1;
    delete leaf2;
    delete root;

    return 0;
}
