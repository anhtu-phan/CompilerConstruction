/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */
#ifndef __PARSER_H__
#define __PARSER_H__
#include "token.h"

void scan(void);
void eat(TokenType tokenType);

void compileProgram(void);
void compileConstDecls(void);
void compileTypeDecls(void);
void compileVarDecls(void);
void compileFuncDecl(void);
void compileProcDecl(void);
void compileUnsignedConstant(void);
void compileConstant(void);
void compileType(void);
void compileBasicType(void);
void compileParams(void);
void compileStatements(void);
void compileAssignSt(void);
void compileCallSt(void);
void compileGroupSt(void);
void compileIfSt(void);
void compileElseSt(void);
void compileWhileSt(void);
void compileForSt(void);
void compileCondition(void);
void compileExpression(void);
void compileTerm(void);
void compileFactor(void);

int compile(char *fileName);

#endif
