//
//  parse.h
//  JCF_CS_280_3
//
//  Created by Joey on 11/7/18.
//  Copyright © 2018 Joey. All rights reserved.
//

#ifndef parse_h
#define parse_h


#endif /* parse_h */
/*
*
* parse.h
*/

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>
using namespace std;

#include "tokens.h"
#include "parsetree.h"
#include "lex.h"

extern ParseTree *Prog(istream *in, int *line);
extern ParseTree *Slist(istream *in, int *line);
extern ParseTree *Stmt(istream *in, int *line);
extern ParseTree *IfStmt(istream *in, int *line);
extern ParseTree *PrintStmt(istream *in, int *line);
extern ParseTree *Expr(istream *in, int *line);
extern ParseTree *LogicExpr(istream *in, int *line);
extern ParseTree *CompareExpr(istream *in, int *line);
extern ParseTree *AddExpr(istream *in, int *line);
extern ParseTree *MulExpr(istream *in, int *line);
extern ParseTree *Factor(istream *in, int *line);
extern ParseTree *Primary(istream *in, int *line);

#endif /* PARSE_H_ */
