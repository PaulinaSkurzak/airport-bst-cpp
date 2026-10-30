// EECE 2560 Mini Project - Task 2: Search

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Airport {
public:
    string code;
    string name;
    string city;
    string country;
    string icao;
    double lat;
    double lon;

    void print() {
        cout << code << " " << name << ", " << city << ", " << country << endl;
    }
};

// node of the tree
struct Node {
    Airport data;
    Node* left;
    Node* right;
};

class AirportBST {
private:
    Node* root;

    // helper to delete all the nodes
    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    // helper for height
    int height(Node* node) {
        if (node == nullptr) return -1;
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        if (leftHeight > rightHeight) return leftHeight + 1;
        else return rightHeight + 1;
    }

public:
    AirportBST() {
        root = nullptr;
    }

    ~AirportBST() {
        destroy(root);
    }


    bool insert(Airport a) {
        Node* newNode = new Node;
        newNode->data = a;
        newNode->left = nullptr;
        newNode->right = nullptr;

        if (root == nullptr) {
            root = newNode;
            return true;
        }

        Node* cur = root;
        while (true) {
            if (a.code < cur->data.code) {
                if (cur->left == nullptr) {
                    cur->left = newNode;
                    return true;
                }
                cur = cur->left;
            }
            else if (a.code > cur->data.code) {
                if (cur->right == nullptr) {
                    cur->right = newNode;
                    return true;
                }
                cur = cur->right;
            }
            else {
                delete newNode;   // duplicate code
                return false;
            }
        }
    }


    Airport* find(string code) {
        Node* cur = root;
        int comparisons = 0;

        cout << "Path: ";
        while (cur != nullptr) {
            comparisons++;
            cout << cur->data.code;

            if (code == cur->data.code) {
                cout << " (comparisons: " << comparisons << ")" << endl;
                return &cur->data;          // found it
            }

            cout << " -> ";
            if (code < cur->data.code) {
                cur = cur->left;            // smaller, go left
            }
            else {
                cur = cur->right;           // bigger, go right
            }
        }

        cout << "(NULL) not found (comparisons: " << comparisons << ")" << endl;
        return nullptr;
    }


    Airport* findMin() {
        Node* cur = root;
        while (cur->left != nullptr) {
            cur = cur->left;
        }
        return &cur->data;
    }

    // largest code is all the way to the right
    Airport* findMax() {
        Node* cur = root;
        while (cur->right != nullptr) {
            cur = cur->right;
        }
        return &cur->data;
    }

    string getRootCode() {
        return root->data.code;
    }


    string findLeaf() {
        Node* cur = root;
        while (cur->left != nullptr || cur->right != nullptr) {
            if (cur->left != nullptr) cur = cur->left;
            else cur = cur->right;
        }
        return cur->data.code;
    }


    string findDeepest() {
        Node* cur = root;
        while (cur->left != nullptr || cur->right != nullptr) {
            if (height(cur->left) >= height(cur->right)) cur = cur->left;
            else cur = cur->right;
        }
        return cur->data.code;
    }

    int getHeight() {
        return height(root);
    }
};


vector<string> splitLine(string line) {
    vector<string> fields;
    string field = "";
    bool inQuotes = false;

    for (int i = 0; i < (int)line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field = "";
        }
        else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}


void doFind(AirportBST& tree, string code) {
    Airport* a = tree.find(code);
    if (a != nullptr) {
        a->print();
    }
}

int main(int argc, char* argv[]) {
    string fileName = "data/airports.dat";
    if (argc > 1) fileName = argv[1];

    ifstream file(fileName);
    if (!file) {
        cout << "Could not open " << fileName << endl;
        return 1;
    }


    AirportBST tree;
    int loaded = 0;
    int skipped = 0;
    int duplicates = 0;
    string line;

    while (getline(file, line)) {
        
        if (line.size() > 0 && line[line.size() - 1] == '\r') {
            line.pop_back();
        }

        vector<string> f = splitLine(line);

        // skip airports with no IATA code
        if (f.size() < 8 || f[4] == "\\N") {
            skipped++;
            continue;
        }

        Airport a;
        a.name = f[1];
        a.city = f[2];
        a.country = f[3];
        a.code = f[4];
        a.icao = f[5];
        a.lat = stod(f[6]);
        a.lon = stod(f[7]);

        if (tree.insert(a)) loaded++;
        else duplicates++;
    }
    file.close();

    cout << "Loaded " << loaded << " airports (" << skipped
         << " lines skipped: no IATA code). Duplicates: " << duplicates << endl;

    cout << endl << "--- Case 1: the root" << endl;
    doFind(tree, tree.getRootCode());

    cout << endl << "--- Case 2: a leaf" << endl;
    doFind(tree, tree.findLeaf());

    cout << endl << "--- Case 3: a code deep in the tree" << endl;
    doFind(tree, tree.findDeepest());

    cout << endl << "--- Case 4: a code that does not exist" << endl;
    doFind(tree, "XYZ");

 
    cout << endl << "--- Case 5: smaller than every code" << endl;
    doFind(tree, "000");

    cout << endl << "--- Case 6: larger than every code" << endl;
    doFind(tree, "ZZZ");

    cout << endl << "Minimum: ";
    tree.findMin()->print();
    cout << "Maximum: ";
    tree.findMax()->print();
    cout << "Height: " << tree.getHeight() << endl;


    cout << endl << "Type: find CODE, min, max or quit" << endl;
    string command;
    while (true) {
        cout << "> ";
        cin >> command;

        if (!cin || command == "quit") {
            break;
        }
        else if (command == "find") {
            string code;
            cin >> code;
            // make it uppercase
            for (int i = 0; i < (int)code.size(); i++) {
                if (code[i] >= 'a' && code[i] <= 'z') {
                    code[i] = code[i] - 32;
                }
            }
            doFind(tree, code);
        }
        else if (command == "min") {
            tree.findMin()->print();
        }
        else if (command == "max") {
            tree.findMax()->print();
        }
        else {
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}