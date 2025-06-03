#include <iostream>
#include <string>

struct RopeNode {
    std::string value;  // Holds string data (for leaf nodes)
    int weight;         // Weight of the node (length of left subtree or string length)
    RopeNode* left;     // Left child
    RopeNode* right;    // Right child

    // Constructor to create a node
    RopeNode(int w, const std::string& val = "", RopeNode* l = nullptr, RopeNode* r = nullptr)
        : weight(w), value(val), left(l), right(r) {}
};

// Helper function to get the weight of a node
int getWeight(RopeNode* node) {
    return node ? (node->left ? node->weight : node->value.size()) : 0;
}

// Split function to divide the rope at a given index
void split(RopeNode* node, int index, RopeNode*& left, RopeNode*& right) {
    if (!node) {
        left = right = nullptr;
        return;
    }

    if (!node->left && !node->right) {  // Leaf node
        if (index < node->value.size()) {
            left = new RopeNode(index, node->value.substr(0, index));
            right = new RopeNode(node->value.size() - index, node->value.substr(index));
        } else {
            left = new RopeNode(node->weight, node->value);
            right = nullptr;
        }
        return;
    }

    if (index < node->weight) {
        split(node->left, index, left, node->left);
        right = node;
        node->weight -= index;
    } else {
        split(node->right, index - node->weight, node->right, right);
        left = node;
    }
}

// Concatenate function to join two ropes
RopeNode* concatenate(RopeNode* left, RopeNode* right) {
    if (!left) return right;
    if (!right) return left;
    return new RopeNode(getWeight(left) + getWeight(right), "", left, right);
}

// Delete function to remove a substring from start to end in the rope
void erase(RopeNode*& root, int start, int end) {
    RopeNode *left = nullptr, *middle = nullptr, *right = nullptr;
    split(root, start, left, middle);            // Split at the start index
    split(middle, end - start, middle, right);   // Split at the end index
    root = concatenate(left, right);             // Exclude `middle` by joining left and right
}

// Utility function to print the rope content (in-order traversal)
void printRope(RopeNode* node) {
    if (!node) return;
    if (!node->left && !node->right) {
        std::cout << node->value;
    } else {
        printRope(node->left);
        printRope(node->right);
    }
}

// Main function to test deletion
int main() {
    // Initialize a rope with an initial string
    RopeNode* rope = new RopeNode(10, "hello i am a rope datastructure");

    int deleteStart = 13;
    int deleteEnd = 18;
    erase(rope, deleteStart, deleteEnd);  // Deletes from index 5 to 8

    std::cout << "After Deletion: ";
    printRope(rope);  // Expected output: "Helloorld"
    std::cout << std::endl;

    return 0;
}
