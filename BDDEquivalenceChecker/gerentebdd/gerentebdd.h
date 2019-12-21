//copyright Prof. Andre Reis - UFRGS


#ifndef GERENTEBDD_H
#define GERENTEBDD_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
//#include "cube.h"

using namespace std;
enum Token_value{AND='*',   OR='+', LP='(', RP=')', NOT='!', LIT, END=';'};


class nodobdd{
    nodobdd *f0;
    nodobdd *f1;
    int variavel;
    int idunico;
public:
    nodobdd(int, nodobdd*, nodobdd*, int);
    void imprime(ostream& saida);
    friend class gerentebdd;
    friend class BooleanFunctionWrapper;

};

class gerentebdd{
    map<string, nodobdd*> tabelaunica;
    map<string, int> var_indices;
    vector <string> variaveis;
    nodobdd *t0;
    nodobdd *t1;
    int proximoIdNodo;
    int proximoIndiceVar;
    static Token_value curr_tok;
    static string string_value;  
    nodobdd* prim(istringstream &equation, set<string> &variaveis);
    nodobdd* term (istringstream &equation, set<string> &variaveis);
    nodobdd* expr (istringstream &equation, set<string> &variaveis);   
    Token_value get_token(istringstream &equation);     
public:
    gerentebdd();
    string chaveunica(string v, nodobdd* n0, nodobdd* n1);
    nodobdd* novo_ou_velho(string s, nodobdd* n0, nodobdd* n1);
    nodobdd* novo_ou_velho(int i, nodobdd* n0, nodobdd* n1);
    nodobdd* cadastravariavel(string s);
    nodobdd* ite(nodobdd* se, nodobdd* entao, nodobdd* senao, int indice);
    nodobdd* and2(nodobdd* n1, nodobdd* n2);
    nodobdd* or2(nodobdd* n1, nodobdd* n2);
    nodobdd* nand2(nodobdd* n1, nodobdd* n2);
    nodobdd* nor2(nodobdd* n1, nodobdd* n2);
    nodobdd* exor2(nodobdd* n1, nodobdd* n2);
    nodobdd* inv(nodobdd* n1);
    nodobdd* cofactor(nodobdd* function, nodobdd* variable, char polarity);
    //nodobdd* createCube(Cube c, int n, vector<string> x17);
    int xstoresSerie(nodobdd*, int);
    nodobdd* create_from_equation(string equation, set<string> &variaveis);
    nodobdd* getConstantZero();
    nodobdd* getConstantOne();
};


//copyright Prof. Andre Reis - UFRGS


#endif



