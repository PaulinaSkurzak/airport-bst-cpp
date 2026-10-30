// EECE 2560 Mini Project - Task 4: Traversals and validation

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

    int height(Node* node) {
        if (node == nullptr) return -1;
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        if (leftHeight > rightHeight) return leftHeight + 1;
        else return rightHeight + 1;
    }

    void inorder(Node* node, vector<Airport>& list) {
        if (node == nullptr) return;
        inorder(node->left, list);
        list.push_back(node->data);
        inorder(node->right, list);
    }

    void preorder(Node* node, ofstream& out) {
        if (node == nullptr) return;
        out << node->data.code << endl;
        preorder(node->left, out);
        preorder(node->right, out);
    }
    bool validate(Node* node, string lo, string hi) {
        if (node == nullptr) return true;
        string code = node->data.code;
        if (lo != "" && code <= lo) return false;
        if (hi != "" && code >= hi) return false;

        return validate(node->left, lo, code) && validate(node->right, code, hi);
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
        while (cur != nullptr) {
            if (code == cur->data.code) return &cur->data;
            if (code < cur->data.code) cur = cur->left;
            else cur = cur->right;
        }
        return nullptr;
    }

   
    bool remove(string code) {
        Node* parent = nullptr;
        Node* cur = root;
        while (cur != nullptr && cur->data.code != code) {
            parent = cur;
            if (code < cur->data.code) cur = cur->left;
            else cur = cur->right;
        }
        if (cur == nullptr) return false;   // not in the tree


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


    vector<Airport> getInorder() {
        vector<Airport> list;
        inorder(root, list);
        return list;
    }


    bool savePreorder(string fileName) {
        ofstream out(fileName);
        if (!out) return false;
        preorder(root, out);
        out.close();
        return true;
    }


    vector<string> getLevelOrder() {
        vector<string> codes;
        if (root == nullptr) return codes;

        vector<Node*> queue;
        queue.push_back(root);
        int front = 0;
        while (front < (int)queue.size()) {
            Node* cur = queue[front];
            front++;
            codes.push_back(cur->data.code);
            if (cur->left != nullptr) queue.push_back(cur->left);
            if (cur->right != nullptr) queue.push_back(cur->right);
        }
        return codes;
    }

    bool validate() {
        return validate(root, "", "");
    }

    int getHeight() {
        return height(root);
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

int loadPreorder(string fileName, AirportBST& original, AirportBST& newTree) {
    ifstream in(fileName);
    if (!in) return -1;

    int count = 0;
    string code;
    while (getline(in, code)) {
        if (code.size() > 0 && code[code.size() - 1] == '\r') code.pop_back();
        if (code == "") continue;

        Airport* a = original.find(code);
        if (a != nullptr) {
            newTree.insert(*a);
            count++;
        }
    }
    in.close();
    return count;
}

void printValidate(AirportBST& tree, string after) {
    cout << "validate after " << after << ": ";
    if (tree.validate()) cout << "OK" << endl;
    else cout << "FAILED" << endl;
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
    int validateFails = 0;
    string line;

    while (getline(file, line)) {
        if (line.size() > 0 && line[line.size() - 1] == '\r') {
            line.pop_back();
        }

        vector<string> f = splitLine(line);
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

        if (!tree.validate()) validateFails++;
    }
    file.close();

    cout << "Loaded " << loaded << " airports (" << skipped
         << " lines skipped: no IATA code). Duplicates: " << duplicates << endl;
    cout << "validate after each of the " << loaded << " inserts: ";
    if (validateFails == 0) cout << "OK every time" << endl;
    else cout << validateFails << " FAILED" << endl;
    cout << "Height: " << tree.getHeight() << endl;

    cout << endl << "========== Task 4(a): list (inorder) ==========" << endl;
    vector<Airport> list = tree.getInorder();


    for (int i = 0; i < 5 && i < (int)list.size(); i++) list[i].print();
    cout << "... (" << list.size() - 10 << " more, type 'list' to print all) ..." << endl;
    for (int i = (int)list.size() - 5; i < (int)list.size(); i++) list[i].print();

    bool sorted = true;
    for (int i = 1; i < (int)list.size(); i++) {
        if (list[i - 1].code >= list[i].code) {
            sorted = false;
            cout << "Not sorted at " << list[i - 1].code << ", " << list[i].code << endl;
        }
    }
    cout << list.size() << " airports in the list. Sorted: ";
    if (sorted) cout << "yes" << endl;
    else cout << "NO" << endl;


    cout << endl << "========== Task 4(b): save and load ==========" << endl;
    string saveFile = "data/preorder.txt";
    if (tree.savePreorder(saveFile)) cout << "Saved preorder codes to " << saveFile << endl;
    else cout << "Could not write " << saveFile << endl;

    AirportBST reloaded;
    int count = loadPreorder(saveFile, tree, reloaded);
    cout << "Loaded " << count << " codes from " << saveFile << endl;

    vector<string> levels1 = tree.getLevelOrder();
    vector<string> levels2 = reloaded.getLevelOrder();

    cout << "Original height: " << tree.getHeight()
         << "   Reloaded height: " << reloaded.getHeight() << endl;

    cout << "Original level order (first 10): ";
    for (int i = 0; i < 10 && i < (int)levels1.size(); i++) cout << levels1[i] << " ";
    cout << endl << "Reloaded level order (first 10): ";
    for (int i = 0; i < 10 && i < (int)levels2.size(); i++) cout << levels2[i] << " ";
    cout << endl;

    // compare the whole level order, node by node
    bool sameShape = (tree.getHeight() == reloaded.getHeight())
                     && (levels1.size() == levels2.size());
    for (int i = 0; sameShape && i < (int)levels1.size(); i++) {
        if (levels1[i] != levels2[i]) sameShape = false;
    }
    cout << "Same shape (same height and same level order): ";
    if (sameShape) cout << "yes" << endl;
    else cout << "NO" << endl;


    cout << endl << "========== Task 4(c): validate in the tests ==========" << endl;

    Airport q;
    q.code = "QQQ";
    q.name = "My Test Airport";
    q.city = "My City";
    q.country = "My Country";
    q.icao = "";
    q.lat = 0;
    q.lon = 0;
    tree.insert(q);
    printValidate(tree, "insert QQQ");

    string leaf = tree.findLeaf();
    tree.remove(leaf);
    printValidate(tree, "delete " + leaf + " (a leaf)");

    string oldRoot = tree.getRootCode();
    tree.remove(oldRoot);
    printValidate(tree, "delete " + oldRoot + " (the root, two children)");

    tree.remove("QQQ");
    printValidate(tree, "delete QQQ");


    cout << endl << "Type: list, validate or quit" << endl;
    string command;
    while (true) {
        cout << "> ";
        cin >> command;

        if (!cin || command == "quit") {
            break;
        }
        else if (command == "list") {
            vector<Airport> all = tree.getInorder();
            for (int i = 0; i < (int)all.size(); i++) all[i].print();
            cout << all.size() << " airports" << endl;
        }
        else if (command == "validate") {
            if (tree.validate()) cout << "validate: OK" << endl;
            else cout << "validate: FAILED" << endl;
        }
        else {
            cout << "Unknown command" << endl;
        }
    }

    return 0;
}