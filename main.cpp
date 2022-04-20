#include <iostream>

class Token
{
public:
    char kind;
    double value;
    Token(){};
    Token(char kind)
    {
        this->kind=kind;
    }
    Token(char kind,double value)
    {
        this->kind=kind;
        this->value=value;
    }
};

class TokenStream
{
public:
    Token get();
    void putBack(Token t);
    TokenStream(){};
private:
    bool full{false};
    Token buffer;
};

double expression();

double term();

double primary();

TokenStream ts;

int main()
{
    try
    {   
        double val=0;
        while(std::cin)
        {
            Token t=ts.get();
            
            if(t.kind=='q') 
                break;
            if(t.kind==';')
                std::cout<<"="<<val<<'\n';
            else
                ts.putBack(t);
            val=expression();
            
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

double expression()
{
    double left=term();
    Token t=ts.get();
    while(true)
    {
        if(t.kind=='+')
        {
            left+=term();
            t=ts.get();
        }
        else if(t.kind=='-')
        {
            left-=term();
            t=ts.get();
        }
        else
        {
            ts.putBack(t);
            return left;
        }
    }
}

double term()
{
    double left=primary();
    Token t=ts.get();
    while(true)
    {
        if(t.kind=='*')
        {
            left*=primary();
            t=ts.get();
        }
        else if(t.kind=='/')
        {
            double d=primary();
            if(d==0) 
                throw std::runtime_error("divide by zero");
            left/=d;
            t=ts.get();
        }
        else 
        {
            ts.putBack(t);
            return left;
        }
    }
}

double primary()
{
    Token t=ts.get();
    if(t.kind=='(')
    {
        double d=expression();
        t=ts.get();
        if(t.kind!=')') throw std::runtime_error(") expected");
        return d;
    }
    else if(t.kind=='8')
        return t.value;
    else 
        throw std::runtime_error("primary expected");
}

void TokenStream::putBack(Token t)
{
    if(full)
        throw std::runtime_error("using putBack() into a full buffer");
    buffer=t;
    full=true;
}

Token TokenStream::get()
{
    if(full)
    {
        full=false;
        return buffer;
    }
    char ch;
    std::cin>>ch;
    switch (ch)
    {
        case 'q':
        case ';':
        case '(': case ')': case '+': case '-': case '*': case '/':
            return Token{ch};
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            std::cin.putback(ch);
            double val;
            std::cin>>val;
            return Token{'8',val};
        }
        default:
            throw std::runtime_error("bad token");
    }
}

//Grammar
/*
Expression:
    Term
    Expression "+" Term
    Expression "-" Term
Term:
    Primary
    Term "*" Primary
    Term "/" Primary
Primary
    Number
    "(" Expression ")"
*/
