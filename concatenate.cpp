#include <iostream>
#include <string>
using namespace std;

// Define a RopeNode structure
struct RopeNode {
    string value;      // Actual string data (only for leaf nodes)
    int weight;        // Weight of the node
    RopeNode* left;    // Left child node
    RopeNode* right;   // Right child node

    // Constructor for creating a leaf node with a string value
    RopeNode(string val) : value(val), weight(val.size()), left(nullptr), right(nullptr) {}

    // Constructor for creating an internal node with left and right children
    RopeNode(RopeNode* leftNode, RopeNode* rightNode) : left(leftNode), right(rightNode) {
        weight = calculateWeight(leftNode);  // Set the weight based on the left child's weight
        value = ""; // Internal nodes don't store value
    }

    // Function to calculate the weight of a node (total characters in the left subtree)
    static int calculateWeight(RopeNode* node) {
        if (node == nullptr) return 0;
        
        if (node->left == nullptr && node->right == nullptr) {
            return node->weight; // Leaf node weight
        }
        
        int x = 0;
        if (node->left != nullptr) x += calculateWeight(node->left);
        return x;
    }

    // Function to retrieve the combined string from the rope
    string getString() const {
        if (left == nullptr && right == nullptr) {
            return value; // Leaf node, return the stored value
        }
        
        string result = "";
        if (left != nullptr) result += left->getString();
        if (right != nullptr) result += right->getString();
        
        return result;
    }
};

// Function to concatenate two rope nodes
RopeNode* concatenate(RopeNode* node1, RopeNode* node2) {
    RopeNode* new_node = new RopeNode(node1, node2);
    return new_node;
}

int main() {
    // Creating leaf nodes for s1
    RopeNode* s1_left = new RopeNode("i a");
    RopeNode* s1_right = new RopeNode("m ");
    RopeNode* s1 = concatenate(s1_left, s1_right);

    // Creating leaf nodes for s2
    RopeNode* s2_left = new RopeNode("a r");
    RopeNode* s2_right = new RopeNode("ope");
    RopeNode* s2 = concatenate(s2_left, s2_right);

    // Combine s1 and s2 into a single rope
    RopeNode* root = concatenate(s1, s2);

    // Output the result
    cout << "s1 string: " << s1->getString() << endl;
    cout << "s2 string: " << s2->getString() << endl;
    cout << "Combined string: " << root->getString() << endl;

    // Clean up memory
    delete s1_left;
    delete s1_right;
    delete s2_left;
    delete s2_right;
    delete s1;
    delete s2;
    delete root;

    return 0;
}
