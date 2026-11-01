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

    void sumDepths(Node* node, int depth, long long& sum, int& count) {
        if (node == nullptr) return;
        sum += depth;
        count++;
        sumDepths(node->left, depth + 1, sum, count);
        sumDepths(node->right, depth + 1, sum, count);
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

    int getHeight() {
        return height(root);
    }

    double getAverageDepth() {
        long long sum = 0;
        int count = 0;
        sumDepths(root, 0, sum, count);
        if (count == 0) return 0.0;
        return (double)sum / count;
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
    cout << "Height: " << tree.getHeight() << "   Average depth: " << tree.getAverageDepth() << endl;

    return 0;
}