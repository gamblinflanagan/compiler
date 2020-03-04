//
//  parsetree.h
//  JCF_CS_280_3
//
//  Created by Joey on 11/7/18.
//  Copyright © 2018 Joey. All rights reserved.
//

#ifndef parsetree_h
#define parsetree_h


#endif /* parsetree_h */

/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "value.h"
//using std::vector;
//using std::map;
using namespace std;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE, BOOLTYPE };

extern void RuntimeError(string);
extern map<string,Value> syms;



class ParseTree
{
    int            linenum;
public:
    ParseTree    *left;
    ParseTree    *right;
    
public:
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
    : linenum(linenum), left(l), right(r) {}
    
    virtual ~ParseTree()
    {
        delete left;
        delete right;
    }
    
    int GetLinenum() const { return linenum; }
    
    virtual NodeType GetType() const { return ERRTYPE; }
    virtual bool setintconst() const { return false; }
    virtual bool setstrconst() const { return false; }
    virtual bool setboolconst() const { return false; }
    virtual bool setident() const { return false; }
    virtual string getident() const { return ""; }
    virtual bool getboolconst() const { return false; }
    
    
    
    int LeafCount() const
    {
        int lc = 0;
        if( left ) lc += left->LeafCount();
        if( right ) lc += right->LeafCount();
        if( left == 0 && right == 0 )
            lc++;
        return lc;
    }
    
    // other methods
    
    int IdentCount() const
    {
        int ic = 0;
        if( left ) ic += left->LeafCount();
        if( right ) ic += right->LeafCount();
        if( setident() )
            ic++;
        return ic;
    }
    
    int StringCount() const
    {
        int sc = 0;
        if( left ) sc += left->LeafCount();
        if( right ) sc += right->LeafCount();
        if( setstrconst() )
            sc++;
        return sc;
    }
    
    
    //virtual Value Eval(/*map<string,Value>& syms*/) = 0;
    virtual Value Eval(map<string,Value>& syms) = 0;
};

class StmtList : public ParseTree
{
    
public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    //virtual Value Eval(map<string,Value> &syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        
        left->Eval(syms);
        if(right)
        {
            right->Eval(syms);
        }
        
        return Value(); // uses overloaded
        //operator+
    }
};


class IfStatement : public ParseTree
{
public:
    IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        
        if(left->Eval(syms).isBoolType() == true && left->Eval(syms).getBoolean() == true)
        {
            return right->Eval(syms); // uses overloaded
            //operator+
        }
        
        else if(left->Eval(syms).isBoolType() == true && left->Eval(syms).getBoolean() == false)
        {
            return Value(); // uses overloaded
            //operator+
        }
        
        else
        {
            RuntimeError("expression must be boolean typed");
        }
        
        return Value();
        
    }
};

class Assignment : public ParseTree
{
    Value val;
public:
    Assignment(int line, ParseTree *lhs, ParseTree *rhs) : ParseTree(line, lhs, rhs) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        if(left->setident())
        {
            syms[left->getident()] = right->Eval(syms);
            return right->Eval(syms);
        }
        else
        {
            RuntimeError("left side must be identifier");
            return Value(); // uses overloaded
            //operator+
        }
        return Value(val);
        
    }
};

class PrintStatement : public ParseTree
{
    //Value val;
public:
    PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        /*
        cout<<left->Eval(syms)<<endl;
        //cout<<right->Eval(syms)<<endl;
        return Value(); // uses overloaded
        //operator+
         */
        
        Value lVal = left->Eval(syms);
        if (lVal.isIntType())
        {
            cout << lVal.getInteger() << endl;
        }
        else if (lVal.isStringType())
        {
            cout << lVal.getString() << endl;
        }
        else if (lVal.isBoolType())
        {
            cout << lVal << endl;
        }
        return Value();
        
        /*
        if (left->Eval(syms)?isIntType())
        {
            cout << left->Eval(syms)?getInteger() << endl;
        }
        else if (left->Eval(syms)?isStringType())
        {
            cout << left->Eval(syms)?getString() << endl;
        }
        else if (left->Eval(syms)?isBoolType())
        {
            cout << left->Eval(syms) << endl;
        }
        return Value();
        */
    }
};

class PlusExpr : public ParseTree
{
public:
    PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        
        auto a1 = left->Eval(syms);
        auto a2 = right->Eval(syms);
        return a1 + a2;
    }
};

class MinusExpr : public ParseTree
{
public:
    MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        auto a1 = left->Eval(syms);
        auto a2 = right->Eval(syms);
        return a1 - a2;
    }
    
};

class TimesExpr : public ParseTree
{
public:
    TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value> &syms)
    {
        
        auto a1 = left->Eval(syms);
        auto a2 = right->Eval(syms);
        return a1 * a2;
        
    }
    
};

class DivideExpr : public ParseTree
{
public:
    DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //Value Eval(map<string,Value>& syms) { return Value(); }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        auto a1 = left->Eval(syms);
        auto a2 = right->Eval(syms);
        return a1 / a2;
    }
    
};

class LogicAndExpr : public ParseTree
{
public:
    LogicAndExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        if(left->Eval(syms).isBoolType() && right->Eval(syms).isBoolType())
        {
            //return left->Eval(syms).isTrue() && right->Eval(syms).isTrue();
            
            if(left->Eval(syms).isTrue() && right->Eval(syms).isTrue())
            {
                return true;
                //return BOOLTYPE;
            }
            else //if(!left->Eval(syms).isTrue() && !right->Eval(syms).isTrue())
            {
                return false;
                //return BOOLTYPE;
            }
            
        }
        RuntimeError("boolen not typed");
        return Value();
        
        
    }
};

class LogicOrExpr : public ParseTree
{
public:
    LogicOrExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        if(left->Eval(syms).isBoolType() || right->Eval(syms).isBoolType())
        {
            //return left->Eval(syms).isTrue() || right->Eval(syms).isTrue();
            
            if(left->Eval(syms).isTrue() || right->Eval(syms).isTrue())
            {
                return true;
                //return BOOLTYPE;
            }
            else //if(!left->Eval(syms).isTrue() && !right->Eval(syms).isTrue())
            {
                return false;
                //return BOOLTYPE;
            }
            
            
        }
        RuntimeError("boolen not typed");
        return Value();
        
        
    }
};

class EqExpr : public ParseTree
{
public:
    EqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) == right->Eval(syms);
        /*
         if(left == right)
         {
         return true;
         }
         return false;
         */
        
    }
};

class NEqExpr : public ParseTree
{
public:
    NEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) != right->Eval(syms);
        /*
         if(left != right)
         {
         return true;
         }
         return false;
         */
        
    }
};

class LtExpr : public ParseTree
{
public:
    LtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) < right->Eval(syms);
        /*
         if(left < right)
         {
         return true;
         }
         return false;
         */
        
    }
};

class LEqExpr : public ParseTree
{
public:
    LEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        //Value lefty = left->Eval(syms);
        //Value righty = right->(syms);
        
        return left->Eval(syms) <= right->Eval(syms);
        
        /*
         if(left <= right)
         {
         return true;
         }
         return false;
         */
    }
};

class GtExpr : public ParseTree
{
public:
    GtExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) > right->Eval(syms);
        /*
         if(left > right)
         {
         return true;
         }
         return false;
         */
        
        
    }
};

class GEqExpr : public ParseTree
{
public:
    GEqExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    NodeType GetType() const { return BOOLTYPE; }
    
    virtual Value Eval(map<string,Value>& syms)
    {
        return left->Eval(syms) >= right->Eval(syms);
        /*
         if(left >= right)
         {
         return true;
         }
         return false;
         */
        
        
    }
};

class IConst : public ParseTree
{
    int val;
    
public:
    IConst(int l, int i) : ParseTree(l), val(i) {}
    IConst(Token& t) : ParseTree(t.GetLinenum())
    {
        val = stoi(t.GetLexeme());
    }
    
    NodeType GetType() const { return INTTYPE; }
    bool setintconst() const { return true; }
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    virtual Value Eval(map<string,Value>& syms)
    {
        return Value(val);
    }
    
    //};
};

class BoolConst : public ParseTree
{
    bool val;
    
public:
    BoolConst(Token& t, bool val) : ParseTree(t.GetLinenum()), val(val) {}
    
    NodeType GetType() const { return BOOLTYPE; }
    bool setboolconst() const { return true; }
    bool getboolconst() const { return val; }
    
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    virtual Value Eval(map<string,Value>& syms)
    {
        return Value(val);
    }
};

class SConst : public ParseTree
{
    string val;
    
public:
    
    bool setstrconst() const { return true; }
    SConst(Token& t) : ParseTree(t.GetLinenum())
    {
        val = t.GetLexeme();
        //operand1.GetStrValue() + operand2.GetStrValue()
    }
    
    NodeType GetType() const { return STRTYPE; }
    
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    virtual Value Eval(map<string,Value>& syms)
    {
        return Value(val);
    }
};

class Ident : public ParseTree
{
    string id;
    
public:
    Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    
    virtual bool setident() const override { return true; }
    string getident() const override{ return id; }
    //virtual Value Eval(/*map<string,Value>& syms*/) { return Value(); }
    virtual Value Eval(map<string,Value>& syms)
    {
        if(!syms.empty() && syms.count(id))
        {
            return syms[id];
        }
        RuntimeError("bad identifier");
        return Value();
    }
    // look up id in syms, and return the Value if that key is in syms
    //}
    
};

#endif /* PARSETREE_H_ */

