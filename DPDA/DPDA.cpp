#include <bits/stdc++.h>
#include "DPDA.hpp"
using namespace std;

DPDA::DPDA(){
    start_state = nullptr;
    num_of_inputs = 0;
    num_of_transitions = 0;
}

DPDA::~DPDA(){
    for (Node* q : Qs)
        delete q;
}

Node* find_node(DPDA& dpda, string& name){
    for(Node* q : dpda.Qs)
        if(q->name == name) return q;
    return nullptr;
}

bool DPDA::is_accepting_state(Node* state){
    for(Node* f : finals)
        if(f == state) return true;
    return false;
}

Transition* DPDA::find_transition(Node* state, string& symbol, string& top){
    for(auto& t : transitions)
        if(t.origin == state && t.input_alphabet == "eps" && t.top_of_stack == top)
            return &t;
    if(symbol != "eps")
        for(auto& t : transitions)
            if(t.origin == state && t.input_alphabet == symbol && t.top_of_stack == top)
                return &t;
    return nullptr;
}

string DPDA::stack_to_string(stack<string> stk){
    string result = "";
    while (!stk.empty()){
        result += stk.top();
        stk.pop();
    }
    return result.empty() ? "empty" : result;
}

void input_Qs(DPDA& dpda){
    cout << "States: ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    string q;
    while (ss >> q) dpda.Qs.push_back(new Node(q));
}

void input_string_alphabet(DPDA& dpda){
    cout << "Input alphabet: ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    string al;
    while (ss >> al) dpda.string_alphabet.push_back(al);
}

void input_stack_alphabet(DPDA& dpda){
    cout << "Stack alphabet: ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    string al;
    while (ss >> al) dpda.stack_alphabet.push_back(al);
}

void input_start_state(DPDA& dpda){
    cout << "Start state: ";
    string q;
    getline(cin, q);
    dpda.start_state = find_node(dpda, q);
    if(dpda.start_state == nullptr){
        cout << "Start state not found. Try again." << endl;
        input_start_state(dpda);
    }
}

void input_start_stack(DPDA& dpda){
    cout << "Initial stack symbol: ";
    string q;
    getline(cin, q);
    if(find(dpda.stack_alphabet.begin(), dpda.stack_alphabet.end(), q) == dpda.stack_alphabet.end()){
        cout << "Stack start symbol not found. Try again." << endl;
        input_start_stack(dpda);
    }
    dpda.start_symbol_stack = q;
}

void input_finals(DPDA& dpda){
    cout << "Final states: ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    string fi;
    while (ss >> fi){
        Node* state = find_node(dpda, fi);
        if(state){
            dpda.finals.push_back(state);
        } else{
            dpda.finals.clear();
            cout << "Final state(states) not found. Try again." << endl;
            input_finals(dpda);
            return;
        }
    }
}

void input_mode(DPDA& dpda){
    cout << "Acceptance mode: ";
    string mode;
    cin >> mode;
    if(mode != "final" && mode != "empty"){
        cout << "Undefined mode, Try again." << endl;
        input_mode(dpda);
    }
    dpda.mode = mode;
}

void input_transitions(DPDA& dpda){
    cout << "Number of transitions: ";
    cin >> dpda.num_of_transitions;
    cin.ignore();
    for(int i = 0; i < dpda.num_of_transitions; i++){
        string org, alph, top_of_stack, dest, add_to_top;
        cin >> org >> alph >> top_of_stack >> dest >> add_to_top;

        Node* org_node  = find_node(dpda, org);
        Node* dest_node = find_node(dpda, dest);

        if(!org_node){
            cout << "Origin state not found. Try again." << endl;
            i--; continue;
        }
        if(!dest_node){
            cout << "Destination state not found. Try again." << endl;
            i--; continue;
        }
        bool is_eps_input = (alph == "eps");
        if(!is_eps_input){
            bool found = find(dpda.string_alphabet.begin(), dpda.string_alphabet.end(), alph) != dpda.string_alphabet.end();
            if(!found){
                cout << "Input alphabet not found. Try again." << endl;
                i--; continue;
            }
        }

        bool conflict = false;
        for(auto& t : dpda.transitions){
            if(t.origin != org_node || t.top_of_stack != top_of_stack) continue;
            if(t.input_alphabet == alph){
                cout << "Duplicate transition. Try again." << endl;
                conflict = true; break;
            }
            if(is_eps_input && t.input_alphabet != "eps"){
                cout << "Conflict: eps-transition conflicts with symbol-transition. Try again." << endl;
                conflict = true; break;
            }
            if(!is_eps_input && t.input_alphabet == "eps"){
                cout << "Conflict: symbol-transition conflicts with eps-transition. Try again." << endl;
                conflict = true; break;
            }
        }
        if(conflict){
            i--;
            continue;
        }

        dpda.transitions.push_back(Transition(org_node, dest_node, alph, top_of_stack, add_to_top));
    }
}

void input_tests(DPDA& dpda){
    cout << "Number of test strings: ";
    cin >> dpda.num_of_inputs;
    for(int i = 0; i < dpda.num_of_inputs; i++){
        string input;
        cin >> input;
        dpda.inputs.push_back(input);
    }
}

void get_inputs(DPDA& dpda){
    input_Qs(dpda);
    input_string_alphabet(dpda);
    input_stack_alphabet(dpda);
    input_start_state(dpda);
    input_start_stack(dpda);
    input_finals(dpda);
    input_mode(dpda);
    input_transitions(dpda);
    input_tests(dpda);
}

void DPDA::print_output(){
    for (string& input : inputs){
        cout << "Input string: " << input << endl;
        cout << "Acceptance mode: " << mode << endl;
        cout << endl;

        Node* current = start_state;
        stack<string> stk;
        stk.push(start_symbol_stack);

        int pos = 0;

        cout << "State: " << current->name << " , Stack: " << stack_to_string(stk) << endl;

        while(true){
            string symbol = (pos < (int)input.size()) ? string(1, input[pos]) : "eps";

            if(stk.empty()) break;
            string top = stk.top();

            Transition* t = find_transition(current, symbol, top);
            if (!t) 
                break;

            bool is_eps_move = (t->input_alphabet == "eps");
            string read_sym = is_eps_move ? "eps" : symbol;

            string stack_op;
            if(t->add_to_top == "eps"){
                stack_op = "pop " + top;
            } else{
                stack_op = "push " + string(1, t->add_to_top[0]);
            }

            stk.pop();
            if(t->add_to_top != "eps"){
                for(int j = (int)t->add_to_top.size() - 1; j >= 0; j--)
                    stk.push(string(1, t->add_to_top[j]));
            }

            current = t->dest;
            if(!is_eps_move) pos++;

            cout << "Read " << read_sym << " -> " << stack_op << endl;
            cout << "State: " << current->name << " , Stack: " << stack_to_string(stk) << endl;
            cout << endl;
        }
        cout << "Halted at state: " << current->name << endl;
        bool accepted = false;
        if(mode == "final"){
            accepted = (pos == (int)input.size()) && is_accepting_state(current);
        } else{
            accepted = (pos == (int)input.size()) && stk.empty();
        }
        cout << "Result: " << (accepted ? "Accepted" : "Rejected") << endl;
        cout << endl;
    }
}

int main(){
    DPDA dpda;
    get_inputs(dpda);
    cout << endl;
    // for(auto t : dpda.transitions)
    //     cout << t.origin->name << " " << t.input_alphabet << " " << t.top_of_stack << endl;
    dpda.print_output();
    return 0;
}