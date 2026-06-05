#include <bits/stdc++.h>
using namespace std;

class Node;
class DFA;
class Transition;

void input_Qs(DFA& dfa);
void input_alphabet(DFA& dfa);
void input_start(DFA& dfa);
void input_finals(DFA& dfa);
void input_transitions(DFA& dfa);
void input_tests(DFA& dfa);
void get_inputs(DFA& dfa);

Node* find_node(DFA& dfa, const string& name);

class Node{
public:
    string name;
    vector<pair<Node*, string>> neighbors;
    Node() = default;
    Node(string name){
        this->name = name;
    }
};

class Transition{
public:
    Node* origin;
    Node* dest;
    string input_alphabet;
    Transition(Node* origin, Node* dest, string input_alphabet)
        : origin(origin), dest(dest), input_alphabet(input_alphabet) {}
};

class DFA{
public:
    vector<Node*> Qs;
    Node* start_state;
    vector<string> alphabet;
    vector<Node*> finals;
    vector<Transition> transitions;
    int num_of_transitions;
    vector<string> inputs;
    int num_of_inputs;
    DFA();
    ~DFA();
};
