
#include <cstdlib>
#include <iostream>
#include "gerentebdd.h"

using namespace std;

int main(int argc, char *argv[])
{
    gerentebdd g;    
    set<string> conjunto_variaveis;
    nodobdd *nd1=g.create_from_equation("c*(a+b)", conjunto_variaveis);
    nodobdd *nd2=g.create_from_equation("c+(!(!a*!b))", conjunto_variaveis);
    nodobdd *c=g.create_from_equation("c", conjunto_variaveis);
    nodobdd *nd3=g.cofactor(nd1, c, '+');
    nodobdd *nd4=g.cofactor(nd2, c, '-');
    if (nd3==nd4)
       {
       cout<<"seems to work"<<endl;
       }
    system("PAUSE");
    return EXIT_SUCCESS;
}
