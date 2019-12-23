//
//  main.cpp
//  BDDEquivalenceChecker
//
//  Created by Henrique Valcanaia on 2019-12-21.
//  Copyright © 2019 Henrique Valcanaia. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "include/aigbdd/aiglec.cpp"
#include "include/gerentebdd/gerentebdd.h"

//#include "aigreader.cpp"
void create_graph(char* filename, int& M, int& I, int& L, int& O, int& A, vertex*& vertices, int*& outputs);
void print_aig(string filename, vertex*& vertices, int* outputs, int& M, int& I, int& O);
void print_bdd(string filename, vector<triple>& T, int bdd_root, int output_index);
string bdd_signature(vector<triple>& T, int bdd_root);


/* GLOBALS
******************************************************************************/

// AIG data
vertex* vertices = NULL;
int* inputs = NULL;
int* outputs = NULL;

// Max variables number (M=I+L+A)
int M = 0;

// Inputs
int I = 0;

// Latches
int L = 0;

// Outputs
int O = 0;

// ANDs
int A = 0;

// Auxiliary stack
stack<int, vector<int>>* stk;

void create_graph(const char* filename, int& M, int& I, int& L, int& O, int& A, vertex*& vertices, int*& outputs);

int areEquivalent(string eq1, string eq2) {
    gerentebdd g;
    set<string> conjunto_variaveis;
    string outputVariable;
    nodobdd *nd1 = g.create_from_equation(eq1, conjunto_variaveis);
    nodobdd *nd2 = g.create_from_equation(eq2, conjunto_variaveis);
    nodobdd *outputNode = g.create_from_equation(outputVariable, conjunto_variaveis);
    nodobdd *nd3 = g.cofactor(nd1, outputNode, '+');
    nodobdd *nd4 =  g.cofactor(nd2, outputNode, '+');
    return (nd3 == nd4);
}

int main(int argc, const char * argv[]) {
    //    aag 6 3 0 1 3
    //    2            // input 1
    //    4            // input 2
    //    6            // input 3
    //    13           // output 1
    //    8 4 2        // 8 = 6 and 2
    //    10 6 4       // 10 = 6 and 4
    //    12 11 9      // 12 = 11 and 9
    //    14 ((!6*!4)*(!4*!2)) 10      // 12 = 11 and 9

    string line;
    char delim[] = " ";
    if (argc != 3 ){
        cout << "Usage: ./main aig1.aag aig2.aag" << endl;
    }

    ifstream myfile (argv[1]);
    if (myfile.is_open() == 0) {
        cout << "Cannot open file:" << argv[1] << endl;
    }

    getline(myfile,line);
    std::vector<std::string> v;
    std::istringstream buf(line);
    for(std::string token; getline(buf, token, ' '); ) {
        v.push_back(token);
    }
//    copy(v.begin(), v.end(), std::ostream_iterator<std::string>(std::cout, "_token "));
    std::cout << '\n';

    // Check if header information (MILOA) is alright
    int M = atoi(v[1].c_str());
    int I = atoi(v[2].c_str());
    int L = atoi(v[3].c_str());
    int O = atoi(v[4].c_str());
    int A = atoi(v[5].c_str());
    cout << "Header Info: "               << endl;
    cout << "M (maximum variable index):" << v[1] << endl;
    cout << "I (number of inputs):"       << v[2] << endl;
    cout << "L (number of latches):"      << v[3] << endl;
    cout << "O (number of outputs):"      << v[4] << endl;
    cout << "A (number of AND gates):"    << v[5] << endl;
    cout << "----------------------------" << endl;
    cout << "Verifying if M=I+L+A" << endl;
    if (M != I+L+A) {
        cout << "ERROR: Invalid! M != I+L+A" << endl;
        exit(-1);
    }

    int inputsID[I], outputsID[O];

    // Primary Inputs
    cout << "Inputs: \n";
    for (int i = 0; i < I; i++) {
        getline(myfile, line);
        int var = atoi(line.c_str());
        int id = var/2;
        inputsID[i] = id;
        cout << atoi(line.c_str()) << " //ID:" << inputsID[i] << endl;
    }

    // Primary Ouputs
    cout << "Outputs: \n";
    for (int i = 0; i < O; ++i) {
        getline(myfile,line);
        outputsID[i] = (atoi(line.c_str()))/2;
        cout << atoi(line.c_str()) << " //ID:" << outputsID[i] << endl;
    }

    // Algorithm(TODO):
    // 1 - Create clauses
    // 2 - Create a dictionary so we can find the original inputs
    // Ex.:
    // A = B*C
    // B = X*Y
    // Therefore: A = X*Y*C
    string eq1, eq2;
    std::vector<string> equation1; // ((A*B)*(B*C))
    std::vector<string> terms;     // (A*B)
    std::vector<string> literals;  // A
    for (int andIndex = 0; andIndex < A; andIndex++) {
        // Concatenate new equation with AND
        if (!eq1.empty()) {
            eq1.append("*");
        }
        
        getline(myfile,line);
        std::vector<std::string> gates;
        std::istringstream buf(line);
        for (std::string token; getline(buf, token, ' '); ) {
            gates.push_back(token);
            cout << token;
        }

        int in1 = atoi(gates.at(1).c_str());
        int in2 = atoi(gates.at(2).c_str());
        int isIn1Negated = (in1 % 2 == 1);
        int isIn2Negated = (in2 % 2 == 1);
        string in1Prefix = isIn1Negated ? "!" : "";
        string in2Prefix = isIn2Negated ? "!" : "";
        string gate1 = to_string(isIn1Negated ? in1-1 : in1);
        string gate2 = to_string(isIn2Negated ? in2-1 : in2);
        
        string currentAndEquation = in1Prefix.append(gate1).append("*").append(in2Prefix).append(gate2);
        eq1.append("(").append(currentAndEquation).append(")");
        eq2.append(eq1);

        cout << "\n";
        cout << gates.at(0) << " = ";
        cout << in1Prefix << gate1;
        cout << in2Prefix << gate2;
        cout << "\n";
    }

    cout << eq1 << "\n";
    
    // Check equivalence
    if (areEquivalent(eq1, eq2)) {
        cout << "Equivalent";
    } else {
        cout << "NOT equivalent";
    }

    return EXIT_SUCCESS;
}
