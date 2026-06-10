#include <bits/stdc++.h>
using namespace std;

class Node;
class DPDA;
class Transition;

void input_Qs(DPDA& dpda);
void input_string_alphabet(DPDA& dpda);
void input_stack_alphabet(DPDA& dpda);
void input_start_state(DPDA& dpda);
void input_start_stack(DPDA& dpda);
void input_finals(DPDA& dpda);
void input_mode(DPDA& dpda);
void input_transitions(DPDA& dpda);
void input_tests(DPDA& dpda);
void get_inputs(DPDA& dpda);
Node* find_node(DPDA& dpda, string& name);

class Node{
public:
    string name;
    vector<pair<Node*, string>> neighbors;
    Node() = default;
    Node(string name) { this->name = name; }
};

class Transition{
public:
    Node* origin;
    Node* dest;
    string input_alphabet;
    string top_of_stack;
    string add_to_top;
    Transition(Node* origin, Node* dest, string input_alphabet, string top_of_stack, string add_to_top)
        : origin(origin), dest(dest), input_alphabet(input_alphabet),
          top_of_stack(top_of_stack), add_to_top(add_to_top) {}
};

class DPDA{
public:
    vector<Node*> Qs;
    Node* start_state;
    string start_symbol_stack;
    string mode;
    vector<string> string_alphabet;
    vector<string> stack_alphabet;
    vector<Node*> finals;
    vector<Transition> transitions;
    int num_of_transitions;
    vector<string> inputs;
    int num_of_inputs;
    DPDA();
    ~DPDA();
    bool is_accepting_state(Node* state);
    Transition* find_transition(Node* state, string& symbol, string& top);
    string stack_to_string(stack<string> stk);
    void print_output();
};