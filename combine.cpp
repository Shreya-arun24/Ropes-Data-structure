#include <iostream>
#include <string>

using namespace std;

// Forward declarations
struct RopeNode;
void deleteRope(RopeNode* node);
void printRope(RopeNode* node, string& result);
void printRope(RopeNode* node);
void updateWeights(RopeNode* node);

struct RopeNode {
    string value;      // Holds the actual string data (only for leaf nodes)
    int weight;        // Weight of the node (total length of left subtree)
    RopeNode* left;    // Left child node
    RopeNode* right;   // Right child node

    // Constructor for creating a node
    RopeNode(const string& val = "") 
        : value(val), weight(val.length()), left(nullptr), right(nullptr) {}
};

// Function to calculate total length of a rope
int calculateLength(RopeNode* node) {
    if (!node) return 0;
    if (!node->left && !node->right) return node->value.length();
    return calculateLength(node->left) + calculateLength(node->right);
}

// Function to update weights recursively
void updateWeights(RopeNode* node) {
    if (!node) return;
    
    if (!node->left && !node->right) {
        node->weight = node->value.length();
    } else {
        node->weight = calculateLength(node->left);
        updateWeights(node->left);
        updateWeights(node->right);
    }
}

// Function to create a balanced rope from a string
RopeNode* createRope(const string& s, int start, int end) {
    if (start > end) return nullptr;
    if (start == end) return new RopeNode(s.substr(start, 1));
    
    // For small strings, create a leaf node
    if (end - start < 8) {  // threshold of 8 characters
        return new RopeNode(s.substr(start, end - start + 1));
    }
    
    // For larger strings, split and create internal node
    int mid = (start + end) / 2;
    RopeNode* node = new RopeNode();
    node->left = createRope(s, start, mid);
    node->right = createRope(s, mid + 1, end);
    node->weight = calculateLength(node->left);
    return node;
}

// Function to split a rope at a specified index
void split(RopeNode* node, int index, RopeNode*& left, RopeNode*& right) {
    if (!node) {
        left = right = nullptr;
        return;
    }

    if (!node->left && !node->right) {  // Leaf node
        if (index <= 0) {
            left = nullptr;
            right = node;
        } else if (index >= node->value.length()) {
            left = node;
            right = nullptr;
        } else {
            left = new RopeNode(node->value.substr(0, index));
            right = new RopeNode(node->value.substr(index));
        }
        return;
    }

    int leftLength = calculateLength(node->left);
    if (index < leftLength) {
        RopeNode* newLeft;
        split(node->left, index, newLeft, node->left);
        left = newLeft;
        right = node;
    } else {
        RopeNode* newRight;
        split(node->right, index - leftLength, node->right, newRight);
        left = node;
        right = newRight;
    }
    
    if (node) updateWeights(node);
}

// Function to concatenate two ropes
RopeNode* concatenate(RopeNode* left, RopeNode* right) {
    if (!left) return right;
    if (!right) return left;
    
    RopeNode* node = new RopeNode();
    node->left = left;
    node->right = right;
    updateWeights(node);
    return node;
}

// Helper function to delete rope
void deleteRope(RopeNode* node) {
    if (!node) return;
    deleteRope(node->left);
    deleteRope(node->right);
    delete node;
}

// Function to print rope to string
void printRope(RopeNode* node, string& result) {
    if (!node) return;
    if (!node->left && !node->right) {
        result += node->value;
    } else {
        printRope(node->left, result);
        printRope(node->right, result);
    }
}

// Function to print rope to cout
void printRope(RopeNode* node) {
    string result;
    printRope(node, result);
    cout << result;
}

// Function to insert a string at a specified index
void insert(RopeNode*& root, const string& str, int index) {
    if (!root) {
        root = createRope(str, 0, str.length() - 1);
        return;
    }

    int totalLength = calculateLength(root);
    if (index < 0) index = 0;
    if (index > totalLength) index = totalLength;

    RopeNode *left = nullptr, *right = nullptr;
    split(root, index, left, right);
    
    RopeNode* middle = createRope(str, 0, str.length() - 1);
    root = concatenate(concatenate(left, middle), right);
    updateWeights(root);
}

// Function to erase a substring between specified indices
void erase(RopeNode*& root, int start, int end) {
    if (!root) return;
    
    int totalLength = calculateLength(root);
    if (start < 0) start = 0;
    if (end >= totalLength) end = totalLength - 1;
    if (start > end) return;

    RopeNode *left = nullptr, *middle = nullptr, *right = nullptr;
    split(root, start, left, middle);
    split(middle, end - start + 1, middle, right);
    
    root = concatenate(left, right);
    if (middle) deleteRope(middle);
    if (root) updateWeights(root);
}

// Function to find character at index
char search(RopeNode* node, int index) {
    if (!node) return '\0';
    
    if (!node->left && !node->right) {
        return (index < node->value.length()) ? node->value[index] : '\0';
    }
    
    int leftLength = calculateLength(node->left);
    if (index < leftLength) {
        return search(node->left, index);
    } else {
        return search(node->right, index - leftLength);
    }
}

// Function to extract substring
string report(RopeNode* root, int start, int end) {
    if (!root) return "";
    
    int totalLength = calculateLength(root);
    if (start < 0) start = 0;
    if (end >= totalLength) end = totalLength - 1;
    if (start > end) return "";

    string result;
    RopeNode *left = nullptr, *middle = nullptr, *right = nullptr;
    split(root, start, left, middle);
    split(middle, end - start + 1, middle, right);
    
    if (middle) {
        printRope(middle, result);
        root = concatenate(concatenate(left, middle), right);
    } else {
        root = concatenate(left, right);
    }
    
    updateWeights(root);
    return result;
}

int main() {
    string initialStr;
    cout << "Enter the initial string for the rope: ";
    getline(cin, initialStr);

    RopeNode* rope = createRope(initialStr, 0, initialStr.length() - 1);

    int choice;
    while (true) {
        cout << "\nCurrent rope content: ";
        printRope(rope);
        cout << "\nLength: " << calculateLength(rope) << "\n\n";
        
        cout << "Choose an operation:\n";
        cout << "1. Insert\n";
        cout << "2. Delete\n";
        cout << "3. Concatenate\n";
        cout << "4. Split\n";
        cout << "5. Search\n";
        cout << "6. Print\n";
        cout << "7. Report (Extract Substring)\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        try {
            if (choice == 1) {
                string insertStr;
                int insertPos;
                cout << "Enter the string to insert: ";
                getline(cin, insertStr);
                cout << "Enter the position to insert at: ";
                cin >> insertPos;
                insert(rope, insertStr, insertPos);
                cout << "String inserted successfully.\n";
            
            } else if (choice == 2) {
                int start, end;
                cout << "Enter the start and end indices for erasing: ";
                cin >> start >> end;
                erase(rope, start, end);
                cout << "Substring erased successfully.\n";
            
            } else if (choice == 3) {
                string concatStr;
                cout << "Enter the string to concatenate: ";
                getline(cin, concatStr);
                RopeNode* newRope = createRope(concatStr, 0, concatStr.length() - 1);
                rope = concatenate(rope, newRope);
                cout << "Strings concatenated successfully.\n";
            
            } else if (choice == 4) {
                int splitIndex;
                cout << "Enter the index to split at: ";
                cin >> splitIndex;
                RopeNode *left = nullptr, *right = nullptr;
                split(rope, splitIndex, left, right);
                cout << "Left part: ";
                printRope(left);
                cout << "\nRight part: ";
                printRope(right);
                cout << "\n";
                rope = concatenate(left, right);
            
            } else if (choice == 5) {
                int searchIndex;
                cout << "Enter the index to search: ";
                cin >> searchIndex;
                char result = search(rope, searchIndex);
                if (result != '\0') {
                    cout << "Character at index " << searchIndex << ": " << result << endl;
                } else {
                    cout << "Index out of bounds\n";
                }
            
            } else if (choice == 6) {
                cout << "Rope content: ";
                printRope(rope);
                cout << endl;
            
            } else if (choice == 7) {
                int start, end;
                cout << "Enter the start and end indices for the substring: ";
                cin >> start >> end;
                string substring = report(rope, start, end);
                cout << "Extracted Substring: " << substring << endl;
            
            } else if (choice == 8) {
                break;
            
            } else {
                cout << "Invalid choice\n";
            }
        } catch (const exception& e) {
            cout << "Error occurred: " << e.what() << endl;
        }
    }

    deleteRope(rope);
    return 0;
}