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

#include <unordered_map>
gerentebdd g;

nodobdd* bddFromAIG(const char* filename) {
    nodobdd* finalBdd = NULL;
    cout << endl << endl << "********** Processing file '" << filename << "' **********" << endl;
    ifstream myfile (filename);
    if (myfile.is_open() == 0) {
        cout << "Cannot open file: " << filename << endl;
        return finalBdd;
    }
    
    string line;
    getline(myfile,line);
    std::vector<std::string> v;
    std::istringstream buf(line);
    for(std::string token; getline(buf, token, ' '); ) {
        v.push_back(token);
    }
    std::cout << endl;
    
    // Check if header information (MILOA) is alright
    int M = atoi(v[1].c_str());
    int I = atoi(v[2].c_str());
    int L = atoi(v[3].c_str());
    int O = atoi(v[4].c_str());
    int A = atoi(v[5].c_str());
    cout << "----------------------------" << endl;
    cout << "Header Info: "               << endl;
    cout << "M (maximum variable index):" << v[1] << endl;
    cout << "I (number of inputs):"       << v[2] << endl;
    cout << "L (number of latches):"      << v[3] << endl;
    cout << "O (number of outputs):"      << v[4] << endl;
    cout << "A (number of AND gates):"    << v[5] << endl;
    cout << "----------------------------" << endl;
    cout << "Is MILOA valid? (M=I+L+A)" << endl;
    int isMILOAValid = (M == I+L+A);
    cout << (isMILOAValid ? "YES" : "NO") << endl;
    if (!isMILOAValid) {
        cout << "MILOA invalid" << endl;
        cout << "M" << M << endl;
        cout << "I" << I << endl;
        cout << "L" << L << endl;
        cout << "A" << A << endl;
        return NULL;
    }
    cout << "----------------------------" << endl;
    
    // Primary Inputs
    unordered_map<int, nodobdd*> inputs;
    cout << "Inputs: " << endl;
    for (int i = 0; i < I; i++) {
        getline(myfile, line);
        int input = stoi(line);
        inputs[input] = g.cadastravariavel(line);
        cout << input << " ";
    }
    cout << endl;
    cout << "----------------------------" << endl;
    
    // Primary Ouputs
    int negatedOutputs[O];
    cout << "Outputs: " << endl;
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
    cout << "----------------------------" << endl;
    
    cout << "Processing ANDs" << endl;
    for (int andIndex = 0; andIndex < A; andIndex++) {
        getline(myfile,line);
        std::vector<std::string> gates;
        std::istringstream buf(line);
        
        int tokenIndex = 0;
        nodobdd *n2 = NULL;
        for (std::string token; getline(buf, token, ' '); tokenIndex++) {
            // Control negated output
            if (tokenIndex == 0) {
                int outputVariable = stoi(token);
                inputs[outputVariable] = g.cadastravariavel(token);
            }
            
            // Process AND inputs
            if (tokenIndex == 1 || tokenIndex == 2) {
                int tokenInt = stoi(token);
                int termInt;
                int isNegated = (tokenInt % 2 == 1);
                if (isNegated) {
                    termInt = tokenInt - 1;
                } else {
                    termInt = tokenInt;
                }
                
                nodobdd *termInput = NULL;
                auto search = inputs.find(termInt);
                if (search != inputs.end()) {
                    termInput = search->second;
                    std::cout << "Found " << search->first << " " << search->second << '\n';
                } else {
                    std::cout << "Term (" << termInt << ") not found for token (" << tokenInt << ")" << endl;
                    continue;
                }
                
                if (tokenIndex == 1) {
                    if (isNegated) {
                        finalBdd = g.inv(termInput);
                    } else {
                        finalBdd = termInput;
                    }
                } else if (tokenIndex == 2) {
                    if (isNegated) {
                        n2 = g.inv(termInput);
                    } else {
                        n2 = termInput;
                    }
                }
            }
        }
        
        finalBdd = g.and2(finalBdd, n2);
    }
    cout << "----------------------------" << endl;
    cout << endl;
    
    return finalBdd;
}

int main(int argc, const char * argv[]) {
    if (argc != 3 ) {
        cout << "Usage: ./main aig1.aag aig2.aag" << endl;
    }
    
    nodobdd* bddFile1 = bddFromAIG(argv[1]);
    nodobdd* bddFile2 = bddFromAIG(argv[2]);
    int isEqual = (bddFile1 == bddFile2);
    if (isEqual) {
        cout << "IS EQUAL!!!";
    } else {
        cout << "IS DIFFERENT!!!";
    }
    cout << endl;
    
    return EXIT_SUCCESS;
}
