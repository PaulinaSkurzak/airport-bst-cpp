#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Airport
{
public:
    string name;
    string city;
    string country;
    string iata;
    string icao;
    double latitude;
    double longitude;

    Airport() {}
};

class AirportBST
{
private:
    struct Node          
    {
        Airport data;
        Node* left;
        Node* right;
        Node(Airport a) : data(a), left(nullptr), right(nullptr) {}
    };

    int heightHelper(Node* node)
{
    if (node == nullptr) return -1;
    int leftH = heightHelper(node->left);
    int rightH = heightHelper(node->right);
    return 1 + max(leftH, rightH);
}
void sumDepths(Node* node, int depth, long long& sum, int& count)
{
    if (node == nullptr) return;
    sum += depth;
    count++;
    sumDepths(node->left, depth + 1, sum, count);
    sumDepths(node->right, depth + 1, sum, count);
}

    Node* root;           

public:
    AirportBST() : root(nullptr) {}

    double averageDepth()
    {
        long long sum = 0;
        int count = 0;
        sumDepths(root, 0, sum, count);
        return count == 0 ? 0.0 : (double)sum / count;
    }
    int height()
    {
        return heightHelper(root);
    }
    bool insert(const Airport& a)
{
    if (root == nullptr)
    {
        root = new Node(a);
        return true;
    }
    Node* cur = root;
    while (true)
    {
        if (a.iata == cur->data.iata)
            return false;    
        else if (a.iata < cur->data.iata)
        {
            if (cur->left == nullptr) { cur->left = new Node(a); return true; }
            cur = cur->left;
        }
        else
        {
            if (cur->right == nullptr) { cur->right = new Node(a); return true; }
            cur = cur->right;
        }
    }
}
};

bool readFile(const string &fileName, vector<string> &lines)
{
    ifstream file(fileName);
    if (!file)
    {
        return false;
    }
    string line;
    while (getline(file, line))
    {
        lines.push_back(line);
    }
    return true;
}

// Your parser, built and tested earlier
vector<string> parseLine(const string& s)
{
    vector<string> fields;
    string currentField = "";
    bool insideQuotes = false;

    for (size_t i = 0; i < s.length(); i++)
    {
        char c = s[i];
        if (c == '"')
        {
            insideQuotes = !insideQuotes;
        }
        else if (c == ',' && !insideQuotes)
        {
            fields.push_back(currentField);
            currentField = "";
        }
        else
        {
            currentField += c;
        }
    }
    fields.push_back(currentField);
    return fields;
}

int main()
{
    string fileName = "data/airports.dat";
    vector<string> lines;

    if (!readFile(fileName, lines))
    {
        cout << "Error: cannot open " << fileName << endl;
        return 1;
    }

    vector<Airport> airports;
    int skipped = 0;

    for (size_t i = 0; i < lines.size(); i++)
    {
        vector<string> fields = parseLine(lines[i]);
        if (fields[4] == "\\N")
        {
            skipped++;      
            continue;
        }
        Airport a;
        a.name = fields[1];
        a.city = fields[2];
        a.country = fields[3];
        a.iata = fields[4];
        a.icao = fields[5];
        a.latitude = stod(fields[6]);
        a.longitude = stod(fields[7]);
        airports.push_back(a);
    }

    cout << "Loaded " << airports.size() << " airports." << endl;
    cout << "Skipped: " << skipped << endl; 
    
    AirportBST tree;
int duplicates = 0;

for (size_t i = 0; i < airports.size(); i++)
{
    if (!tree.insert(airports[i]))
        duplicates++;
}

cout << "Duplicates: " << duplicates << endl;
cout << "Height: " << tree.height() << endl;
cout << "Average depth: " << tree.averageDepth() << endl;

    return 0;
}