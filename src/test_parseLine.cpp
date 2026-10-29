#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> parseLine(const string& s) {
    vector<string> fields;
    string currentField = "";
    bool insideQuotes = false;

    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '"') {
            insideQuotes = !insideQuotes;
        }
        else if (c == ',' && !insideQuotes) {
            fields.push_back(currentField);
            currentField = "";
        }
        else {
            currentField += c;
        }
    }
    fields.push_back(currentField);
    return fields;
}

int main() {
    // Test case 1: simple, no quotes
    vector<string> f1 = parseLine("Boston,Logan");
    cout << "Test 1 (" << f1.size() << " fields): ";
    for (auto& f : f1) cout << "[" << f << "] ";
    cout << endl;

    // Test case 2: quotes, no inner comma
    vector<string> f2 = parseLine("3,\"Logan Airport\",\"Boston\"");
    cout << "Test 2 (" << f2.size() << " fields): ";
    for (auto& f : f2) cout << "[" << f << "] ";
    cout << endl;

    // Test case 3: comma INSIDE quotes (the tricky case)
    vector<string> f3 = parseLine("3,\"Smith, John Airport\",\"Boston\"");
    cout << "Test 3 (" << f3.size() << " fields): ";
    for (auto& f : f3) cout << "[" << f << "] ";
    cout << endl;

    return 0;
}