#include <bits/stdc++.h>
#include <cstdio>
#include "DFA.hpp"
using namespace std;

DFA::DFA(){
    start_state = nullptr;
    num_of_inputs = 0;
    num_of_transitions = 0;
}
DFA::~DFA(){
    for (Node* q : Qs){
        delete q;
    }
}
void DFA::print_unreachable_states(){
    dfs(start_state);
    cout << "Unreachable states: ";
    for (Node* q : Qs){
        if (!q->reachable_from_start){
            cout << q->name << " ";
        }
    }
    cout << endl;
}
void dfs(Node* q){
    if (q->reachable_from_start){
        return;
    }
    q->reachable_from_start = true;
    for (auto &neighbor : q->neighbors){
        dfs(neighbor.first);
    }
}
bool path_to_final(DFA& dfa, Node* q, vector<Node*>& visited){
    if (find(dfa.finals.begin(), dfa.finals.end(), q) != dfa.finals.end()){
        return true;
    }
    if (find(visited.begin(), visited.end(), q) != visited.end()){
        return false;
    }

    visited.push_back(q);
    for (auto &neighbor : q->neighbors){
        if (path_to_final(dfa, neighbor.first, visited)){
            return true;
        }
    }
    return false;
}

bool DFA::is_path_to_final(Node* q){
    vector<Node*> visited;
    return path_to_final(*this, q, visited);
}
void DFA::print_dead_states(){
    cout << "Dead states: ";
    for(auto st : Qs){
        if(!is_path_to_final(st))
            cout << st->name << " ";
    }
    cout << endl;
}
void DFA::connect_alphabet_to_DEAD(Node* q, string alph){
    transitions.push_back(Transition(q, this->Dead_state, alph));
    q->neighbors.push_back({this->Dead_state, alph});
}
void DFA::add_DEAD_state(){
    Node *Dead = new Node("Dead");
    this->Dead_state = Dead;
    for(auto q : Qs){
        for(auto alph : alphabet){
            bool flag = false;
            for(auto pr : q->neighbors){
                if(alph == pr.second){
                    flag = true;
                    break;
                }
            }
            if(!flag){
                connect_alphabet_to_DEAD(q, alph);
            }
        }
    }
}
void DFA::is_empty(){
    if(is_path_to_final(this->start_state))
        cout << "Language of DFA is not empty" << endl;
    else
        cout << "Language of DFA is empty" << endl;
}
void write_Dead_output(Node* org, string symbol){
    cout << "Read '" << symbol << "' -> move from " << org->name << " to DEAD" << endl;
    cout << "Entered DEAD state." << endl;
    cout << "Execution halted early." << endl;
    cout << "Result: Rejected" << endl;
}
string DFA::print_transition(Node* org, string symbol){
    for (auto temp : org->neighbors){
        if (temp.second == symbol){
            if (temp.first->name == "Dead"){
                write_Dead_output(org, symbol);
                return "Dead";
            }
            cout << "Read '" << symbol << "' -> move from " << org->name << " to " << temp.first->name << endl;
            return temp.first->name;
        }
    }
    return "no-transition";
}
void DFA::print_output(){
    for (string str : inputs){
        cout << "Input string: " << str << endl;
        cout << "Start at state: " << start_state->name << endl;

        Node* current = start_state;
        bool halted = false;

        for (char c : str){
            string symbol(1, c);

            if (find(alphabet.begin(), alphabet.end(), symbol) == alphabet.end()){
                cout << "Input string contains symbol outside alphabet." << endl;
                cout << "Result: Rejected" << endl;
                halted = true;
                break;
            }

            string next = print_transition(current, symbol);
            if (next == "Dead"){
                halted = true;
                break;
            }
            if (next == "no-transition"){
                cout << "No transition defined for symbol '" << symbol << "'." << endl; 
                cout << "Result: Rejected" << endl;
                halted = true;
                break;
            }

            current = find_node(*this, next);
        }

        if (!halted){
            cout << "Halted at state: " << current->name << endl;
            if (find(finals.begin(), finals.end(), current) != finals.end()){
                cout << "Result: Accepted" << endl;
            }
            else{
                cout << "Result: Rejected" << endl;
            }
        }
        cout << endl;
    }
}
Node* find_node(DFA& dfa, const string& name){
    for (Node* q : dfa.Qs){
        if (q->name == name){
            return q;
        }
    }
    return nullptr;
}
void input_Qs(DFA& dfa){
    cout << "States: ";

    string line;
    getline(cin, line);

    stringstream ss(line);
    string q;

    while (ss >> q){
        dfa.Qs.push_back(new Node(q));
    }
}
void input_alphabet(DFA &dfa){
    cout << "Alphabet: ";

    string line;
    getline(cin, line);

    stringstream ss(line);
    string al;

    while (ss >> al){
        dfa.alphabet.push_back(al);
    }
}
void input_start(DFA &dfa){
    cout << "Start state: ";

    string q;
    getline(cin, q);

    dfa.start_state = find_node(dfa, q);
    if(dfa.start_state == nullptr){
        cout << "Start state not found. Try again." << endl;
        input_start(dfa);
        // exit(1);
    }
}
void input_finals(DFA &dfa){
    cout << "Final states: ";
    string line;
    getline(cin, line);

    stringstream ss(line);
    string fi;

    while (ss >> fi){
        Node* state = find_node(dfa, fi);
        if(state){
            dfa.finals.push_back(state);
        }
        else{
            dfa.finals.clear();
            cout << "Final state(states) not found. Try again." << endl;
            input_finals(dfa);
            // exit(1);
        }
    }
}
void input_transitions(DFA &dfa){
    cout << "Number of transitions: ";
    cin >> dfa.num_of_transitions;
    for (int i = 0; i < dfa.num_of_transitions; i++){
        string org, alph, dest;
        cin >> org >> alph >> dest;

        Node* org_node = find_node(dfa, org);
        Node* dest_node = find_node(dfa, dest);
        if (org_node && dest_node && find(dfa.alphabet.begin(), dfa.alphabet.end(), alph) != dfa.alphabet.end()){
            bool is_duplicate = false;
            for(auto &transition : dfa.transitions){
                if(transition.origin == org_node && transition.input_alphabet == alph){
                    is_duplicate = true;
                    break;
                }
            }
            if(is_duplicate){
                cout << "Origin state and input alphabet already exists. Try again." << endl;
                i--;
                continue;
            }
            else{
                org_node->neighbors.push_back({dest_node, alph});
                dfa.transitions.push_back(Transition(org_node, dest_node, alph));
            }
        }
        else if(org_node == nullptr){
            cout << "Origin state not found. Try again." << endl;
            i--;
            continue;
        }
        else if(find(dfa.alphabet.begin(), dfa.alphabet.end(), alph) == dfa.alphabet.end()){
            cout << "Input alphabet not found. Try again." << endl;
            i--;
            continue;
        }
        else if(dest_node == nullptr){
            cout << "Destination state not found. Try again." << endl;
            i--;
            continue;
        }
    }
}
void input_tests(DFA& dfa){
    cout << "Number of test strings: ";
    cin >> dfa.num_of_inputs;
    for (int i = 0; i < dfa.num_of_inputs; i++){
        string input;
        cin >> input;
        dfa.inputs.push_back(input);
    }
}
void get_inputs(DFA& dfa){
    input_Qs(dfa);
    input_alphabet(dfa);
    input_start(dfa);
    input_finals(dfa);
    input_transitions(dfa);
    input_tests(dfa);
}

int main(){
    DFA dfa;
    get_inputs(dfa);

    // for (Node *q : dfa.Qs){
    //     cout << q->name << " -> ";
    //     for (auto &neighbor : q->neighbors){
    //         cout << neighbor.first->name << "(" << neighbor.second << ") ";
    //     }
    //     cout << endl;
    // }

    // cout << "-----------------------" << endl;

    // dfa.print_unreachable_states();
    // dfa.print_dead_states();
    // dfa.add_DEAD_state();
    // for(auto transition : dfa.transitions){
    //     cout << transition.origin->name << " " << transition.input_alphabet << " " << transition.dest->name << endl;
    // }
    dfa.add_DEAD_state();
    dfa.print_output();

    return 0;
}
