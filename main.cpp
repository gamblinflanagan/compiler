//
//  main.cpp
//  JCF_CS280_4
//
//  Created by Joey on 12/3/18.
//  Copyright © 2018 Joey. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <string>
#include <istream>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <map>
#include <vector>
#include "parse.h"
//#include "lex.h"
#include "value.h"
using namespace std;




Token    pushed_token;
bool pushedBack = false;
int Icounter = 0;
int Scounter = 0;
map<string, int> identMap;
map<string, int>::iterator k;

static Token GetNextToken(istream *in, int *linenum)
{
    if( pushedBack )
    {
        pushedBack = false;
        return pushed_token;
    }
    
    return getNextToken(in, linenum);
}

static void PushBackToken(const Token& t)
{
    if( pushedBack )
    {
        abort();
    }
    pushedBack = true;
    pushed_token = t;
    
}

static int error_count = 0;

void
ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;
}

ParseTree *Prog(istream *in, int *line)
{
    ParseTree *sl = Slist(in, line);
    if( sl == 0 )
    {
        ParseError(*line, "No statements in program");
    }
    if(error_count)
    {
        return 0;
    }
    return sl;
}


ParseTree *Slist(istream *in, int *line)
{
    ParseTree *s = Stmt(in, line);
    if( s == 0 )
    {
        return 0;
    }
    if( GetNextToken(in, line) != SC) //&& GetNextToken(in, line) != endline characther)
    {
        ParseError(*line, "Missing semicolon");
        return 0;
    }
    
    return new StmtList(s, Slist(in,line));
}

ParseTree *Stmt(istream *in, int *line)
{
    ParseTree *s;
    
    Token t = GetNextToken(in, line);
    switch( t.GetTokenType() ) {
        case IF:
            s = IfStmt(in, line);
            break;
            
        case PRINT:
            s = PrintStmt(in, line);
            break;
            
        case DONE:
            return 0;
            
        case ERR:
            ParseError(*line, "Invalid token");
            return 0;
            
        default:
            // put back the token and then see if it's an Expr
            PushBackToken(t);
            s = Expr(in, line);
            
            if( s == 0 )
            {
                ParseError(*line, "Invalid statement");
                return 0;
            }
            
            //return s;
            break;
    }
    return s;
}

ParseTree *IfStmt(istream *in, int *line)
{
    // ADD HANDLER
    
    ParseTree *ex = Expr(in, line);
    if(!ex)
    {
        ParseError(*line, "Bad If");
        return 0;
    }
    Token t = GetNextToken(in, line);
    if (t != THEN)
    {
        ParseError(*line, "Missing Then");
    }
    
    ParseTree *stmt = Stmt(in, line);
    
    if (!stmt)
    {
        ParseError(*line, "Missing statement");
    }
    
    return new IfStatement(t.GetLinenum(), ex, stmt);
    
}

ParseTree *PrintStmt(istream *in, int *line)
{
    // ADD HANDLER
    ParseTree *ex = Expr(in, line);
    if(!ex)
    {
        ParseError(*line, "Bad Print statement");
        return 0;
    }
    else
    {
        return new PrintStatement(*line, ex);
    }
}

ParseTree *Expr(istream *in, int *line)
{
    ParseTree *t1 = LogicExpr(in, line);
    if( t1 == 0 ) {
        return 0;
    }
    
    Token t = GetNextToken(in, line);
    
    if( t != ASSIGN ) {
        PushBackToken(t);
        return t1;
    }
    
    ParseTree *t2 = Expr(in, line); // right assoc
    if( t2 == 0 )
    {
        ParseError(*line, "Missing expression after operator");
        return 0;
    }
    
    return new Assignment(t.GetLinenum(), t1, t2);
}

ParseTree *LogicExpr(istream *in, int *line)
{
    ParseTree *t1 = CompareExpr(in, line);
    if( t1 == 0 ) {
        return 0;
    }
    
    // HANDLE OP
    Token t = GetNextToken(in, line);
    
    if( t != LOGICAND && t != LOGICOR ) {
        PushBackToken(t);
        return t1;
    }
    
    ParseTree *t2 = CompareExpr(in, line);
    if( t2 == 0 )
    {
        ParseError(*line, "Missing expression after operator");
        return 0;
    }
    
    // if t was LOGICAND return a new LogicAnd node
    // otherwise return a new LogicOr node
    // and note you should be doing this in some kind
    // of a loop like you do in Add
    if(t == LOGICAND)
    {
        return new LogicAndExpr(t.GetLinenum(), t1, t2);
    }
    return new LogicOrExpr(t.GetLinenum(), t1, t2);
    
}

ParseTree *CompareExpr(istream *in, int *line)
{
    ParseTree *t1 = AddExpr(in, line);
    if( t1 == 0 )
    {
        return 0;
    }
    
    // HANDLE OP
    Token t = GetNextToken(in, line);
    
    if(t != EQ && t != GEQ && t != LEQ && t != NEQ && t != LT && t != GT) {
        PushBackToken(t);
        return t1;
    }
    
    ParseTree *t2 = AddExpr(in, line); // right assoc
    if( t2 == 0 )
    {
        ParseError(*line, "Missing expression after operator");
        return 0;
    }
    
    // you need to make a different class based on t
    if(t == EQ)
    {
        return new EqExpr(t.GetLinenum(), t1, t2);
    }
    else if(t == GEQ)
    {
        return new GEqExpr(t.GetLinenum(), t1, t2);
    }
    else if(t == LEQ)
    {
        return new LEqExpr(t.GetLinenum(), t1, t2);
    }
    else if(t == NEQ)
    {
        return new NEqExpr(t.GetLinenum(), t1, t2);
    }
    else if(t == LT)
    {
        return new LtExpr(t.GetLinenum(), t1, t2);
    }
    else if(t == GT)
    {
        return new GtExpr(t.GetLinenum(), t1, t2);
    }
    else
    {
        return 0;
    }
}

ParseTree *AddExpr(istream *in, int *line)
{
    ParseTree *t1 = MulExpr(in, line);
    if( t1 == 0 )
    {
        return 0;
    }
    while(true)
    {
        Token t = GetNextToken(in, line);
        if( t != PLUS && t != MINUS )
        {
            PushBackToken(t);
            return t1;
        }
        ParseTree *t2 = MulExpr(in, line);
        if( t2 == 0 )
        {
            ParseError(*line, "Missing expression after operator");
            return 0;
        }
        if( t == PLUS )
        {
            t1 = new PlusExpr(t.GetLinenum(), t1, t2);
        }
        else
        {
            t1 = new MinusExpr(t.GetLinenum(), t1, t2);
        }
    }
    //return 0;
}

ParseTree *MulExpr(istream *in, int *line)
{
    ParseTree *t1 = Factor(in, line);
    if( t1 == 0 )
    {
        return 0;
    }
    
    while(true)
    {
        Token t = GetNextToken(in, line);
        if( t != STAR && t != SLASH )
        {
            PushBackToken(t);
            return t1;
        }
        ParseTree *t2 = Factor(in, line);
        if( t2 == 0 )
        {
            ParseError(*line, "Missing expression after operator");
            return 0;
        }
        if( t == STAR )
        {
            t1 = new TimesExpr(t.GetLinenum(), t1, t2);
        }
        else
        {
            t1 = new DivideExpr(t.GetLinenum(), t1, t2);
        }
        
    }
    //return 0;
}

ParseTree *Factor(istream *in, int *line)
{
    bool neg = false;
    Token t = GetNextToken(in, line);
    
    if( t == MINUS )
    {
        neg = true;
    }
    else
    {
        PushBackToken(t);
    }
    
    ParseTree *p1 = Primary(in, line);
    
    if( p1 == 0 )
    {
        ParseError(*line, "Missing primary");
        return 0;
    }
    
    if( neg )
    {
        // handle as -1 * Primary
        return new TimesExpr(t.GetLinenum(), new IConst(t.GetLinenum(), -1), p1);
    }
    else
        return p1;
}

ParseTree *Primary(istream *in, int *line)
{
    Token t = GetNextToken(in, line);
    //cout << "In primary, token is " << t << endl;
    
    // PROCESS TOKEN, IDENTIFY PRIMARY, RETURN SOMETHING
    if(t.GetTokenType() == IDENT)
    {
        Icounter ++;
        identMap[t.GetLexeme()]++;
        return new Ident(t);
        
    }
    else if(t.GetTokenType() == ICONST)
    {
        return new IConst(t);
    }
    else if(t.GetTokenType() == SCONST)
    {
        Scounter ++;
        return new SConst(t);
    }
    
    else if(t.GetTokenType() == TRUE)
    {
        return new BoolConst(t, 1);
    }
    else if(t.GetTokenType() == FALSE)
    {
        return new BoolConst(t, 0);
    }
    //if left paren check for right paren if not error
    else if(t.GetTokenType() == LPAREN)
    {
        ParseTree *ex = Expr(in, line);
        if(!ex)
        {
            ParseError(*line, "Invalid Expression");
            return 0;
        }
        Token tt = GetNextToken(in, line);
        if(tt != RPAREN)
        {
            ParseError(*line, "Missing RPAREN");
            return 0;
        }
        return ex;
    }
    else
    {
        ParseError(*line, "Invalid primary");
        return 0;
    }
}

void printMap()
{
    
    int temp = 0;
    int counter1 = 0;
    int counter2 = 0;
    
    for(k = identMap.begin(); k != identMap.end(); k++)
    {
        
        if(k->second > temp)
        {
            temp = k->second;
        }
    }
    
    for(k = identMap.begin(); k != identMap.end(); k++)
    {
        if(k->second == temp)
        {
            counter1++;
        }
    }
    
    
    for(k = identMap.begin(); k != identMap.end(); k++)
    {
        if(k->second == temp)
        {
            cout << k->first;
            
            
            if(counter2 < counter1-1)
            {
                cout << ", ";
            }
            counter2++;
        }
    }
    
    if(Icounter != 0)
    {
        cout<<endl;
    }
    
}

void RuntimeError(string err)
{
    cerr<<"0: RUNTIME ERROR "<<err<<endl;
    exit(1);
}




int main(int argc, char * argv[])
{
    ifstream infile;
    istream *in = &cin;
    int linenum = 0;
    
    for(int j = 1; j < argc; j++)
    {
        string word = argv[j];
        
        
        if( in != &cin )
        {
            cout<<"TOO MANY FILE NAMES"<<endl;
            return 0;
        }
        infile.open(word);
        if(!infile.is_open())
        {
            cout << "COULD NOT OPEN" << word << endl;
            return 0;
        }
        
        in = &infile;
    }
    
    ParseTree *prog = Prog(in, &linenum);
    
    if( prog == 0 )
    {
        return 0;
    }
    
    map<string,Value> syms;
    prog->Eval(syms);
    
    //cout << "do the traversals here" << endl;
    
    /*
     cout << "LEAF COUNT: " << prog->LeafCount() << endl;
     cout << "STRING COUNT: " << Scounter<<endl;
     if(Icounter > 0)
     {
     cout << "IDENT COUNT: " << Icounter<<endl;
     }
     printMap();
     */
    
    return 0;
}

