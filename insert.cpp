#include <iostream>
#include <string>

struct RopeNode {
    std::string value;  // Holds the actual string data (only for leaf nodes)
    int weight;         // Weight of the node
    RopeNode* left;     // Left child node
    RopeNode* right;    // Right child node

    RopeNode(int w, const std::string& val = "", RopeNode* l = nullptr, RopeNode* r = nullptr)
        : weight(w), value(val), left(l), right(r) {}
};

// Function to get the weight of a node
int getWeight(RopeNode* node) {
    return node ? (node->left ? node->weight : node->value.size()) : 0;
}
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
RopeNode* concatenate(RopeNode* left, RopeNode* right) {
    if (!left) return right;
    if (!right) return left;
    return new RopeNode(getWeight(left) + getWeight(right), "", left, right);
}
void insert(RopeNode*& root, const std::string& str, int index) {
    RopeNode *left = nullptr, *right = nullptr;
    split(root, index, left, right);                     // Split at the insertion index
    RopeNode* middle = new RopeNode(str.size(), str);    // Create a new node for the inserted string
    root = concatenate(concatenate(left, middle), right); // Reassemble the rope with the new string
}
void printRope(RopeNode* node) {
    if (!node) return;
    if (!node->left && !node->right) {
        std::cout << node->value;
    } else {
        printRope(node->left);
        printRope(node->right);
    }
}
int main() {
    RopeNode* rope = new RopeNode(10, "hello i am a datastructure");

    std::string insertStr = "Rope ";
    int insertPos = 1;
    insert(rope, insertStr, insertPos); // Insert "Rope" at position 5

    std::cout << "After Insertion: ";
    printRope(rope); // Expected output: "HelloRopeWorld"
    std::cout << std::endl;

    return 0;
}
