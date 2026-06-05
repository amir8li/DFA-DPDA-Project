#include <bits/stdc++.h>
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
}

void input_finals(DFA &dfa){
    cout << "Final states: ";
    string line;
    getline(cin, line);

    stringstream ss(line);
    string fi;

    while (ss >> fi){
        Node* state = find_node(dfa, fi);
        if (state){
            dfa.finals.push_back(state);
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
        if (org_node && dest_node){
            org_node->neighbors.push_back({dest_node, alph});
            dfa.transitions.push_back(Transition(org_node, dest_node, alph));
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
}

int main(){
    DFA dfa;

    get_inputs(dfa);

    cout << dfa.Qs[0]->name << endl;
    cout << dfa.alphabet[0] << endl;
    cout << dfa.start_state->name << endl;
    cout << dfa.finals[0]->name << endl;
    cout << dfa.transitions[0].origin->name << endl;
    cout << dfa.transitions[0].dest->name << endl;
    cout << dfa.transitions[0].input_alphabet << endl;

    return 0;
}
