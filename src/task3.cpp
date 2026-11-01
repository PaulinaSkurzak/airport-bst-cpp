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

struct Node {
    Airport data;
    Node* left;
    Node* right;
};

class AirportBST {
private:
    Node* root;

    void destroy(Node* node) {
        if (node == nullptr) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
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
                delete newNode;
                return false;
            }
        }
    }

    bool search(string code) {
        Node* cur = root;
        while (cur != nullptr) {
            if (code == cur->data.code) return true;
            if (code < cur->data.code) cur = cur->left;
            else cur = cur->right;
        }
        return false;
    }

    bool remove(string code) {
        Node* parent = nullptr;
        Node* cur = root;
        while (cur != nullptr && cur->data.code != code) {
            parent = cur;
            if (code < cur->data.code) cur = cur->left;
            else cur = cur->right;
        }
        if (cur == nullptr) return false;

        if (cur->left != nullptr && cur->right != nullptr) {
            Node* succParent = cur;
            Node* succ = cur->right;
            while (succ->left != nullptr) {
                succParent = succ;
                succ = succ->left;
            }
            cur->data = succ->data;
            parent = succParent;
            cur = succ;
        }

        Node* child;
        if (cur->left != nullptr) child = cur->left;
        else child = cur->right;

        if (parent == nullptr) root = child;
        else if (parent->left == cur) parent->left = child;
        else parent->right = child;

        delete cur;
        return true;
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

    string findLeftOnly() {
        Node* cur = root;
        string result = "";
        vector<Node*> stack;
        stack.push_back(cur);
        while (stack.size() > 0) {
            Node* n = stack.back();
            stack.pop_back();
            if (n->left != nullptr && n->right == nullptr) return n->data.code;
            if (n->left != nullptr) stack.push_back(n->left);
            if (n->right != nullptr) stack.push_back(n->right);
        }
        return result;
    }

    string findRightOnly() {
        Node* cur = root;
        string result = "";
        vector<Node*> stack;
        stack.push_back(cur);
        while (stack.size() > 0) {
            Node* n = stack.back();
            stack.pop_back();
            if (n->right != nullptr && n->left == nullptr) return n->data.code;
            if (n->left != nullptr) stack.push_back(n->left);
            if (n->right != nullptr) stack.push_back(n->right);
        }
        return result;
    }

    string findTwoChildren() {
        Node* cur = root;
        string result = "";
        vector<Node*> stack;
        stack.push_back(cur);
        while (stack.size() > 0) {
            Node* n = stack.back();
            stack.pop_back();
            if (n->left != nullptr && n->right != nullptr) return n->data.code;
            if (n->left != nullptr) stack.push_back(n->left);
            if (n->right != nullptr) stack.push_back(n->right);
        }
        return result;
    }

    vector<string> getTopLevels(int maxLevel) {
        vector<string> codes;
        if (root == nullptr) return codes;

        vector<Node*> queue;
        vector<int> depth;
        queue.push_back(root);
        depth.push_back(0);

        int front = 0;
        while (front < (int)queue.size()) {
            Node* cur = queue[front];
            int d = depth[front];
            front++;
            if (d >= maxLevel) continue;

            codes.push_back(cur->data.code);
            if (cur->left != nullptr) {
                queue.push_back(cur->left);
                depth.push_back(d + 1);
            }
            if (cur->right != nullptr) {
                queue.push_back(cur->right);
                depth.push_back(d + 1);
            }
        }
        return codes;
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

void printTopLevels(AirportBST& tree) {
    vector<string> codes = tree.getTopLevels(3);
    for (int i = 0; i < (int)codes.size(); i++) cout << codes[i] << " ";
    cout << endl;
}

void testDelete(AirportBST& tree, string label, string code) {
    cout << "=== Delete " << label << " (" << code << ") ===" << endl;
    cout << "BEFORE: ";
    printTopLevels(tree);
    bool result = tree.remove(code);
    cout << "Delete returned: " << result << endl;
    cout << "AFTER:  ";
    printTopLevels(tree);
    cout << endl;
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
    string line;

    while (getline(file, line)) {
        if (line.size() > 0 && line[line.size() - 1] == '\r') {
            line.pop_back();
        }

        vector<string> f = splitLine(line);
        if (f.size() < 8 || f[4] == "\\N") continue;

        Airport a;
        a.name = f[1];
        a.city = f[2];
        a.country = f[3];
        a.code = f[4];
        a.icao = f[5];
        a.lat = stod(f[6]);
        a.lon = stod(f[7]);

        if (tree.insert(a)) loaded++;
    }
    file.close();

    cout << "Tree built: " << loaded << " airports loaded." << endl << endl;

    string rootCode = tree.getRootCode();
    testDelete(tree, "the root", rootCode);

    testDelete(tree, "a leaf (D1)", tree.findLeaf());
    testDelete(tree, "left-only (D2)", tree.findLeftOnly());
    testDelete(tree, "right-only (D2)", tree.findRightOnly());
    testDelete(tree, "two children (D3)", tree.findTwoChildren());

    cout << "=== made-up airport QQQ ===" << endl;
    Airport mine;
    mine.name = "Paulina Skurzak";
    mine.city = "Boston";
    mine.country = "United States";
    mine.code = "QQQ";
    mine.icao = "ZZZZ";
    mine.lat = 0.0;
    mine.lon = 0.0;

    tree.insert(mine);
    cout << "found after insert: " << tree.search("QQQ") << endl;
    tree.remove("QQQ");
    cout << "found after delete: " << tree.search("QQQ") << endl;

    return 0;
}