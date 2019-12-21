//copyright Prof. Andre Reis - UFRGS


#include "gerentebdd.h"


Token_value gerentebdd::curr_tok;
string gerentebdd::string_value;  




//***************************************************************************
nodobdd::nodobdd(int var, nodobdd* n0, nodobdd* n1, int id)
{
    f0=n0;
    f1=n1;
    variavel=var;
    idunico=id;
}

//***************************************************************************
void nodobdd::imprime(ostream& saida)
{
    if (variavel != 150)
    {
        saida<<"nodo: "<<idunico
        <<"\tVar: "<<variavel<<"\tP0: "<<f0->idunico
        <<"\tP1: "<<f1->idunico<<endl;
        f0->imprime(saida);
        f1->imprime(saida);
    }
}

//***************************************************************************
gerentebdd::gerentebdd( )
{
    nodobdd *temp=NULL;
    t0=new nodobdd(150, temp, temp, 0);
    t1=new nodobdd(150, temp, temp, 1);
    tabelaunica["t0"]=t0;
    tabelaunica["t1"]=t1;
    proximoIdNodo=2;
    proximoIndiceVar=0;
    variaveis.resize(1000);
}

//***************************************************************************
string gerentebdd::chaveunica(string v, nodobdd* n0, nodobdd* n1)
{
    ostringstream saida;
    saida <<"v"<<v<<"n0"<< (n0->idunico) <<"n1"<< (n1->idunico);
    return saida.str();
}


//***************************************************************************
nodobdd* gerentebdd::novo_ou_velho(string v, nodobdd* n0, nodobdd* n1)
{
    nodobdd *nodoretorno=NULL;
    string chave=chaveunica(v, n0, n1);
    map<string, nodobdd*>::iterator it;
    it = tabelaunica.find(chave);
    //cout<<chave<<endl;
    if (it == tabelaunica.end())
    {//nodo nao existe, deve ser criado
        int vi=var_indices[v];
        nodoretorno = new nodobdd(vi, n0, n1, proximoIdNodo);
        proximoIdNodo++;
        tabelaunica[chave]=nodoretorno;
    }
    else
    {//nodo existe, eh o segundo elemento do iterador do mapa
        nodoretorno = it->second;
    }
    return nodoretorno;
}

//***************************************************************************
nodobdd* gerentebdd::novo_ou_velho(int indice, nodobdd* n0, nodobdd* n1)
{
    nodobdd *nodoretorno=NULL;
    string v=variaveis[indice];
    string chave=chaveunica(v, n0, n1);
    map<string, nodobdd*>::iterator it;
    it=tabelaunica.find(chave);
    //cout<<chave<<endl;
    if (it==tabelaunica.end())
    {//nodo nao existe, deve ser criado
        nodoretorno = new nodobdd(indice, n0, n1, proximoIdNodo);
        proximoIdNodo++;
        tabelaunica[chave]=nodoretorno;
        //cout<<"criou"<<endl<<endl;
    }
    else
    {//nodo existe, eh o segundo elemento do iterador do mapa
        nodoretorno = it->second;
        //cout<<"nao criou"<<endl<<endl;
    }
    return nodoretorno;
}



//***************************************************************************
nodobdd* gerentebdd::cadastravariavel(string v)
{
    nodobdd *nodoretorno=NULL;
    string chave=chaveunica(v, t0, t1);
    map<string, nodobdd*>::iterator it;
    it=tabelaunica.find(chave);
    if (it==tabelaunica.end())
    {//nodo nao existe, deve ser criado
        var_indices[v]=proximoIndiceVar;
        variaveis[proximoIndiceVar] = v;
        nodoretorno = new nodobdd(proximoIndiceVar, t0, t1, proximoIdNodo);
        cout<<v<<"=   "<<proximoIndiceVar<<endl;
        tabelaunica[chave]=nodoretorno;
        proximoIdNodo++;
        proximoIndiceVar++;
    }
    else
    {//nodo existe, eh o segundo elemento do iterador do mapa
        nodoretorno = it->second;
    }
    return nodoretorno;}


//***************************************************************************
nodobdd* gerentebdd::ite(nodobdd* condicao, nodobdd* entao, nodobdd* senao, int indice=0)
{
    int indiceTmp;
    nodobdd* nodoretorno = NULL;
    if (condicao == t1)
    {return entao;}
    else if (condicao == t0)
    {return senao;}
    else if ((entao == t1) && (senao == t0))
    {return condicao;}
    else
    {
        nodobdd *condicao_cf0, *condicao_cf1, *entao_cf0, *entao_cf1,
        *senao_cf0, *senao_cf1, *n0, *n1;
        if (indice < (condicao->variavel) || condicao==t0 || condicao==t1)
        {condicao_cf0 = condicao; condicao_cf1 = condicao;}
        else
        {condicao_cf0 = condicao->f0; condicao_cf1 = condicao->f1;}
        if (indice < (entao->variavel) || entao==t0 || entao==t1)
        {entao_cf0 = entao; entao_cf1 = entao;}
        else
        {entao_cf0 = entao->f0; entao_cf1 = entao->f1;}
        if (indice < (senao->variavel) || senao==t0 || senao==t1)
        {senao_cf0 = senao; senao_cf1 = senao;}
        else
        {senao_cf0 = senao->f0; senao_cf1 = senao->f1;}
        indiceTmp=condicao_cf0->variavel;
        if ((entao_cf0->variavel)<indiceTmp) indiceTmp=entao_cf0->variavel;
        if ((senao_cf0->variavel)<indiceTmp) indiceTmp=senao_cf0->variavel;
        n0=ite(condicao_cf0, entao_cf0, senao_cf0, indiceTmp);
        
        indiceTmp=condicao_cf1->variavel;
        if ((entao_cf1->variavel)<indiceTmp) indiceTmp=entao_cf1->variavel;
        if ((senao_cf1->variavel)<indiceTmp) indiceTmp=senao_cf1->variavel;
        n1=ite(condicao_cf1, entao_cf1, senao_cf1, indiceTmp);
        if (n0==n1)
            nodoretorno=n0;
        else
            nodoretorno=novo_ou_velho(indice, n0, n1);
    }
    return nodoretorno;
}

//***************************************************************************
nodobdd* gerentebdd::and2(nodobdd* n1, nodobdd* n2)
{
    return ite(n1, n2, t0);
}



//***************************************************************************
nodobdd* gerentebdd::or2(nodobdd* n1, nodobdd* n2)
{
    return ite(n1, t1, n2);
}



//***************************************************************************
nodobdd* gerentebdd::nand2(nodobdd* n1, nodobdd* n2)
{
    return ite(n1, inv(n2), t1);
}

//***************************************************************************
nodobdd* gerentebdd::nor2(nodobdd* n1, nodobdd* n2)
{
    return ite(n1, t0, inv(n2));
}

//***************************************************************************
nodobdd* gerentebdd::exor2(nodobdd* n1, nodobdd* n2)
{
    return ite(n1, inv(n2), n2);
}


//***************************************************************************
nodobdd* gerentebdd::inv(nodobdd* n1)
{
    return ite(n1, t0, t1);
}

//***************************************************************************
nodobdd* gerentebdd::cofactor(nodobdd* function, nodobdd* varFactor, char polarity)
{
    //cout<< "function->variavel="<<function->variavel<<endl;
    //cout<< "varFactor->variavel="<<varFactor->variavel<<endl;
    
    //if(varFactor==NULL) cout<<"achei";
    nodobdd* nodoretorno = NULL, *nd0=NULL, *nd1=NULL;
    if (function == t1)
        return t1;
    else if (function == t0)
        return t0;
    else if ((function->variavel) < (varFactor->variavel))
    {
        nd0=cofactor(function->f0, varFactor, polarity);
        nd1=cofactor(function->f1, varFactor, polarity);
        if (nd0==nd1)
            return nd0;
        else{
            nodoretorno=novo_ou_velho(function->variavel, nd0, nd1);
            return nodoretorno;
        }
    }
    else if (function->variavel > varFactor->variavel)
    {return function;}
    else
    {
        if (polarity == '-')
            return function->f0;
        if (polarity == '+')
            return function->f1;
    }
    //cout<<"very strange"<<endl;
    return nodoretorno;
}

//***************************************************************************
int gerentebdd::xstoresSerie(nodobdd *nd, int i)
{
    int retorno=0;
    if ((nd->idunico) > 1) //nodo nao terminal
    {
        int rf0=xstoresSerie(nd->f0, i);
        int rf1=xstoresSerie(nd->f1, i);
        if (rf0>rf1)
            retorno=rf0+1;
        else
            retorno=rf1+1;
    }
    else if ((nd->idunico) !=  i)
        retorno = -10;
    return retorno;
}

//***************************************************************************
nodobdd* gerentebdd::create_from_equation(string equation, set<string> &variaveis)
{
    istringstream functionality(equation);
    return expr(functionality, variaveis);
}

//***************************************************************************
nodobdd* gerentebdd::prim(istringstream &equation, set<string> &variaveis)
//nary_tree* nary_tree::prim(bool get, istringstream &equation)
{
    get_token(equation);
    switch (curr_tok)
    {
        case LIT:
        {   get_token(equation);
            nodobdd* ndtmp=cadastravariavel(string_value);
            variaveis.insert(string_value);
            return ndtmp;
        }
        case NOT:
        {
            return inv(prim(equation, variaveis));
        }
        case LP:
        {//cout<<"abre parenteses"<<endl;
            nodobdd* ndtmp=expr(equation, variaveis);
            //cout<<"fecha parenteses"<<endl;
            if (curr_tok != RP) {/*cout<<"ERROR: current token="<<curr_tok<<endl; */while(1){};}
            get_token(equation); //eat )
            return ndtmp;
        }
        default:
        {}
    }
    
    cout << "Algo de errado não está certo";
    return NULL;
}


//***************************************************************************
nodobdd* gerentebdd::term (istringstream &equation, set<string> &variaveis)
//nary_tree* nary_tree::term (bool get, istringstream &equation)
{
    nodobdd* left=prim(equation, variaveis);
    for (;;)
    {
        switch (curr_tok)
        {
            case AND:
                //create and node
                left =and2(left, prim(equation, variaveis));
                break;
            default:
                return left;
        }
    }
}


//***************************************************************************
nodobdd* gerentebdd::expr (istringstream &equation, set<string> &variaveis)
//nary_tree* nary_tree::expr (bool get, istringstream &equation)
{
    
    nodobdd* left=term(equation, variaveis);
    for (;;)
    {
        switch (curr_tok)
        {
            case OR:
                left = or2(left, term(equation, variaveis));
                break;
            default:
                return left;
        }
    }
}

//***************************************************************************
Token_value gerentebdd::get_token(istringstream &equation)              
//Token_value nary_tree::get_token(istringstream &equation)
{
    char ch = 0;
    equation >> ch;
    switch(ch) {
        case 0 :
            return curr_tok=END;
        case ';':
        case '*':
        case '+':
        case '!':
        case '(':
        case ')':
        case '=':
            //cout<<"current token="<<ch<<endl;
            return curr_tok=Token_value(ch);
            
        default: // NAME, NAME =, or error
            if(isalpha(ch)) {
                string_value="";
                for( ; isalnum(ch) && !equation.eof(); equation>>ch)
                {
                    //cout<<ch;
                    string_value+=ch;  //; needed at the end of string
                    //ch=0;
                }
                
                equation.putback(ch);
                //cout<<"current token=literal "<<string_value<<endl;
                return curr_tok=LIT;
            }
            //error("bad token");
            return curr_tok=END;
    }
}


//***************************************************************************
nodobdd* gerentebdd::getConstantZero()
{return t0;}


//***************************************************************************
nodobdd* gerentebdd::getConstantOne()
{return t1;}






//copyright Prof. Andre Reis - UFRGS

