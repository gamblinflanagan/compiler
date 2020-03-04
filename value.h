/*
 * value.h
 */

#ifndef VALUE_H_
#define VALUE_H_

#include <string>
#include <iostream>
using namespace std;
extern void RuntimeError(string);
//extern Parsetree *left;

// object holds boolean, integer, or string, and remembers which it holds
class Value {
    bool        bval;
    int        ival;
    string    sval;
    enum VT { isBool, isInt, isString, isTypeError } type;
    
public:
    Value() : bval(false), ival(0), type(isTypeError) {}
    Value(bool bval) : bval(bval), ival(0), type(isBool) {}
    Value(int ival) : bval(false), ival(ival), type(isInt) {}
    Value(string sval) : bval(false), ival(0), sval(sval), type(isString) {}
    
    // in the case of an error, I use the value to hold the error message
    Value(string sval, bool isError) : bval(false), ival(0), sval(sval), type(isTypeError) {}
    
    bool isBoolType() const { return type == VT::isBool; }
    bool isIntType() const { return type == VT::isInt; }
    bool isStringType() const { return type == VT::isString; }
    bool isError() const { return type == VT::isTypeError; }
    bool hasMessage() const { return isError() && sval.size() > 0; }
    
    bool isTrue() const { return isBoolType() && bval; }
    bool getBoolean() const {
        if( !isBoolType() )
            throw "Not boolean valued";
        return bval;
    }
    
    int getInteger() const {
        if( !isIntType() )
            throw "Not integer valued";
        return ival;
    }
    
    string getString() const {
        if( !isStringType() )
            throw "Not string valued";
        return sval;
    }
    
    string getMessage() const {
        if( !hasMessage() )
            throw "No message";
        return sval;
    }
    
    friend ostream& operator<<(ostream& out, const Value& v)
    {
        if( v.type == VT::isBool )
        {
            out <<(v.bval ? "True" : "False");
        }
        else if( v.type == VT::isInt )
        {
            out << v.ival;
        }
        else if( v.type == VT::isString )
        {
            out << v.sval;
        }
        else if( v.sval.size() > 0 )
        {
            out << "RUNTIME ERROR " << v.sval;
        }
        else
        {
            out << "TYPE ERROR";
        }
        
        return out;
    }
    
    Value operator+(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() + v.getInteger());
        }
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() + v.getString());
        }
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator-(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() - v.getInteger());
        }
        /*
         else if(v.isStringType() && v.isStringType())
         {
         return Value(v.getString() - v.getString());
         }
         */
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator*(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() * v.getInteger());
        }
        
        
        else if(this->isIntType() && v.isStringType() && this->getInteger() > 0)
        {
            string test;
            
            for(int i = 0; i < this->getInteger(); i++)
            {
                test += v.getString();
            }
            
            return test;//Value(v.getString());
            
        }
        
        else if(this->isStringType() && v.isIntType() && v.getInteger() > 0)
        {
            string test;
            for(int i = 0; i < v.getInteger(); i++)
            {
                test += this->getString();
            }
            return test;//Value(v.getString());
        }
        
        else if(this->isStringType() && v.isIntType() && v.getInteger() == 0)
        {
            string test = "";
            //this->getString() = "";
            return test;//Value(v.getString());
        }
        
        else if(this->isIntType() && v.isStringType() && this->getInteger() == 0)
        {
            string test = "";
            //v.getString() = "";
            return test;//Value(v.getString());
        }
        else if(this->isIntType() && v.isBoolType())
        {
            return Value(!v.getBoolean());
        }
        
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator/(const Value& v)
    {
        if(this->isIntType() && v.isIntType() && v.getInteger() != 0)
        {
            return Value(this->getInteger() / v.getInteger());
        }
        else if(this->isIntType() && v.isIntType() && v.getInteger() == 0)
        {
            RuntimeError("Cannot divide by 0");
            return Value();
        }
        /*
         else if(v.isStringType() && v.isStringType())
         {
         return Value(v.getString() + v.getString());
         }
         */
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator<(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() < v.getInteger());
        }
        
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() < v.getString());
        }
        
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator<=(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() <= v.getInteger());
        }
        
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() <= v.getString());
        }
        
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator>(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() > v.getInteger());
        }
        
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() > v.getString());
        }
        
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator>=(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() >= v.getInteger());
        }
        
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() >= v.getString());
        }
        
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator==(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() == v.getInteger());
        }
        
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() == v.getString());
        }
        
        else if(this->isBoolType() && v.isBoolType())
        {
            return Value(this->getBoolean() == v.getBoolean());
        }
        
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            return Value();
        }
    }
    Value operator!=(const Value& v)
    {
        if(this->isIntType() && v.isIntType())
        {
            return Value(this->getInteger() != v.getInteger());
        }
        
        else if(this->isStringType() && v.isStringType())
        {
            return Value(this->getString() != v.getString());
        }
        
        else if(this->isBoolType() && v.isBoolType())
        {
            return Value(this->getBoolean() != v.getBoolean());
        }
        else
        {
            //cout<<"0: ";
            RuntimeError("type mismatch");
            Value ans = this->operator==(v);
            ans.bval = !ans.bval;
            return ans;
        }
    }
};

#endif /* VALUE_H_ */
