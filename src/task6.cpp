// EECE 2560 Mini Project - Task 6: The shape experiment

// The sorted-order tree is a chain of 6,072 nodes, so insert, search,
// height, average depth and clear all use loops instead of recursion.
// Only the balanced build is recursive (its depth is only about 13).

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>     
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


    Node* buildBalanced(vector<Airport>& sorted, int lo, int hi) {
        if (lo > hi) return nullptr;
        int mid = (lo + hi) / 2;
        Node* node = new Node;
        node->data = sorted[mid];
        node->left = buildBalanced(sorted, lo, mid - 1);
        node->right = buildBalanced(sorted, mid + 1, hi);
        return node;
    }

public:
    AirportBST() {
        root = nullptr;
    }

    ~AirportBST() {
        clear();
    }


    void clear() {
        vector<Node*> stack;
        if (root != nullptr) stack.push_back(root);
        while (stack.size() > 0) {
            Node* cur = stack.back();
            stack.pop_back();
            if (cur->left != nullptr) stack.push_back(cur->left);
            if (cur->right != nullptr) stack.push_back(cur->right);
            delete cur;
        }
        root = nullptr;
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


    void shape(int& height, double& averageDepth) {
        height = -1;
        averageDepth = 0;
        if (root == nullptr) return;

        vector<Node*> queue;
        vector<int> depth;
        queue.push_back(root);
        depth.push_back(0);

        long long depthSum = 0;
        int front = 0;
        while (front < (int)queue.size()) {
            Node* cur = queue[front];
            int d = depth[front];
            front++;

            depthSum += d;
            if (d > height) height = d;

            if (cur->left != nullptr) {
                queue.push_back(cur->left);
                depth.push_back(d + 1);
            }
            if (cur->right != nullptr) {
                queue.push_back(cur->right);
                depth.push_back(d + 1);
            }
        }
        averageDepth = (double)depthSum / queue.size();
    }


    vector<Airport> getInorder() {
        vector<Airport> list;
        vector<Node*> stack;
        Node* cur = root;
        while (cur != nullptr || stack.size() > 0) {
            while (cur != nullptr) {          // go as far left as possible
                stack.push_back(cur);
                cur = cur->left;
            }
            cur = stack.back();
            stack.pop_back();
            list.push_back(cur->data);
            cur = cur->right;
        }
        return list;
    }


    void buildFromSorted(vector<Airport>& sorted) {
        clear();
        root = buildBalanced(sorted, 0, (int)sorted.size() - 1);
    }


    void rebalance() {
        vector<Airport> sorted = getInorder();
        buildFromSorted(sorted);
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


unsigned int seed = 2560;
int randomNumber(int n) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % n;
}


void buildByInsert(AirportBST& tree, vector<Airport>& list) {
    tree.clear();
    for (int i = 0; i < (int)list.size(); i++) {
        tree.insert(list[i]);
    }
}


double timeSearches(AirportBST& tree, vector<string>& keys) {
    auto start = chrono::high_resolution_clock::now();
    int found = 0;
    for (int i = 0; i < (int)keys.size(); i++) {
        if (tree.search(keys[i])) found++;
    }
    auto end = chrono::high_resolution_clock::now();

    if (found != (int)keys.size()) cout << "(warning: some searches failed)" << endl;
    return chrono::duration<double, milli>(end - start).count();
}


void printRow(string name, AirportBST& tree, vector<string>& keys) {
    int height;
    double avgDepth;
    tree.shape(height, avgDepth);
    double ms = timeSearches(tree, keys);

    cout << name;
    for (int i = (int)name.size(); i < 26; i++) cout << " ";   // line up the columns
    cout << height << "\t\t" << avgDepth << "\t\t" << ms << " ms" << endl;
}

int main(int argc, char* argv[]) {
    string fileName = "data/airports.dat";
    if (argc > 1) fileName = argv[1];

    ifstream file(fileName);
    if (!file) {
        cout << "Could not open " << fileName << endl;
        return 1;
    }


    vector<Airport> fileOrder;
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
        fileOrder.push_back(a);
    }
    file.close();
    int n = (int)fileOrder.size();
    cout << "Loaded " << n << " airports" << endl;


    vector<string> keys;
    for (int i = 0; i < 10000; i++) {
        keys.push_back(fileOrder[randomNumber(n)].code);
    }


    AirportBST tree;
    buildByInsert(tree, fileOrder);


    vector<Airport> sorted = tree.getInorder();


    vector<Airport> shuffled = fileOrder;
    for (int i = n - 1; i > 0; i--) {
        int j = randomNumber(i + 1);
        Airport temp = shuffled[i];
        shuffled[i] = shuffled[j];
        shuffled[j] = temp;
    }

    cout << endl << "Build order               Height\t\tAvg depth\tTime for 10,000 searches" << endl;
    cout << "-------------------------------------------------------------------------" << endl;

    buildByInsert(tree, fileOrder);
    printRow("File order", tree, keys);

    buildByInsert(tree, sorted);
    printRow("Sorted order", tree, keys);

    tree.rebalance();                   
    printRow("Balanced (middle rule)", tree, keys);

    buildByInsert(tree, shuffled);
    printRow("Random shuffle (bonus)", tree, keys);

    cout << endl << "Expected heights: file order 177, sorted 6071, balanced 12, shuffle about 25 to 29" << endl;
    return 0;
}