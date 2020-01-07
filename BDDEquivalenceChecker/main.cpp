//
//  main.cpp
//  BDDEquivalenceChecker
//
//  Created by Henrique Valcanaia on 2019-12-21.
//  Copyright © 2019 Henrique Valcanaia. All rights reserved.
//

#include "include/gerentebdd/gerentebdd.h"

string equationFromAIG(const char* filename) {
    cout << "Processing file '" << filename << "'";
    ifstream myfile (filename);
    if (myfile.is_open() == 0) {
        cout << "Cannot open file:" << filename << endl;
    }

    string line;
    getline(myfile,line);
    std::vector<std::string> v;
    std::istringstream buf(line);
    for(std::string token; getline(buf, token, ' '); ) {
        v.push_back(token);
    }
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

    // Primary Inputs
    cout << "Inputs: ";
    for (int i = 0; i < I; i++) {
        getline(myfile, line);
        int input = stoi(line);
        cout << input << " ";
    }
    cout << endl;

    // Primary Ouputs
    int negatedOutputs[O];
    cout << "Outputs: ";
    for (int i = 0; i < O; ++i) {
        getline(myfile,line);
        int output = stoi(line);
        int isEven = (output % 2 == 1);
        if (isEven) {
            negatedOutputs[i] = output;
        }
        cout << output << " ";
    }
    cout << endl;

    string eq1;
    int isNegated = 0;
    for (int andIndex = 0; andIndex < A; andIndex++) {
        getline(myfile,line);
        std::vector<std::string> gates;
        std::istringstream buf(line);
        int tokenIndex = 0;
        
        for (std::string token; getline(buf, token, ' '); tokenIndex++) {
            // Control negated output
            if (tokenIndex == 0) {
                int outputVariable = stoi(token);
                for (int i = 0; i < O; i++) {
                    if (negatedOutputs[i] == outputVariable+1) {
                        isNegated = 1;
                        break;
                    }
                }
                continue;
            }
            
            // Process AND inputs
            if (tokenIndex == 1 || tokenIndex == 2) {
                if (!eq1.empty() && tokenIndex == 1) {
                    eq1.append("*");
                }
                
                string negated = "!";
                
                int termInt = stoi(token);
                string term = (termInt % 2 == 0) ? to_string(termInt) : negated.append(to_string(termInt-1));
                
                if (isNegated) {
                    isNegated = 0;
                    eq1.append("!");
                }
                
                if (tokenIndex == 1) {
                    eq1.append("(").append(term);
                } else if (tokenIndex == 2) {
                    eq1.append("*").append(term).append(")");
                }
            }
        }
    }
    
    return eq1;
}

int main(int argc, const char * argv[]) {
    if (argc != 3 ) {
        cout << "Usage: ./main aig1.aag aig2.aag" << endl;
    }
    
    string eq1 = equationFromAIG(argv[1]);
    string eq2 = equationFromAIG(argv[2]);
    gerentebdd g;
    set<string> conjunto_variaveis;
    nodobdd* nodoBddEq1 = g.create_from_equation(eq1, conjunto_variaveis);
    nodobdd* nodoBddEq2 = g.create_from_equation(eq2, conjunto_variaveis);
    int isEqual = nodoBddEq1 == nodoBddEq2;
    printf("Is \n%s \nequal to \n%s?\n%d\n", eq1.c_str(), eq2.c_str(), isEqual);

    return EXIT_SUCCESS;
}
