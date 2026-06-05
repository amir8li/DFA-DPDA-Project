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
void dfs(Node* q);
bool path_to_final(DFA& dfa, Node* q, vector<Node*>& visited);

class Node{
public:
    string name;
    vector<pair<Node*, string>> neighbors;
    bool reachable_from_start = false;
    bool is_dead = false;
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
    Node* Dead_state;
    vector<string> alphabet;
    vector<Node*> finals;
    vector<Transition> transitions;
    int num_of_transitions;
    vector<string> inputs;
    int num_of_inputs;
    DFA();
    ~DFA();
    void print_unreachable_states();
    bool is_path_to_final(Node* q);
    void print_dead_states();
    void connect_alphabet_to_DEAD(Node* q, string alph);
    void add_DEAD_state();
};
