/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  // TODO of anhtu
	if(lookAhead->tokenType == KW_CONST){
			compileConstDecls();
	}	
 	if(lookAhead->tokenType == KW_TYPE){
			compileTypeDecls();
	}
	if(lookAhead->tokenType == KW_VAR){
			compileVarDecls();
	}	
	if(lookAhead->tokenType == KW_PROCEDURE){ 
			compileProcDecl();
	}
	if(lookAhead->tokenType == KW_FUNCTION){
		compileFuncDecl();
	}
	if(lookAhead->tokenType == KW_BEGIN){
			compileStatements();
	}
  assert("Block parsed!");
}

void compileConstDecls(void) {
  // TODO of anhtu
	eat(KW_CONST);
	eat(TK_IDENT);
	eat(SB_EQ);
	compileConstant();
	eat(SB_SEMICOLON);
	while(lookAhead->tokenType == TK_IDENT){
		eat(TK_IDENT);
		eat(SB_EQ);
		compileConstant();
		eat(SB_SEMICOLON);
	}	
}

void compileTypeDecls(void) {
  // TODO of anhtu
	eat(KW_TYPE);
	eat(TK_IDENT);
	eat(SB_EQ);
	compileType();
	eat(SB_SEMICOLON);
	while(lookAhead->tokenType == TK_IDENT){
		eat(TK_IDENT);
		eat(SB_EQ);
		compileType();
		eat(SB_SEMICOLON);
	}	
}

void compileVarDecls(void) {
  // TODO of anhtu
	eat(KW_VAR);
	eat(TK_IDENT);
	eat(SB_COLON);
	compileType();
	eat(SB_SEMICOLON);
	while(lookAhead->tokenType == TK_IDENT){
		eat(TK_IDENT);
		eat(SB_COLON);
		compileType();
		eat(SB_SEMICOLON);
	}	
}

void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  // TODO of anhtu
	while(lookAhead->tokenType == KW_PROCEDURE){
		eat(KW_PROCEDURE);
		eat(TK_IDENT);
		if(lookAhead->tokenType != SB_SEMICOLON){
			compileParams();	
		}
		eat(SB_SEMICOLON);
		compileBlock();
		eat(SB_SEMICOLON);
	}

  assert("Procedure parsed ....");
}

void compileFuncDecl(void) {
  assert("Parsing a function ....");
  // TODO of anhtu
	while(lookAhead->tokenType == KW_PROCEDURE || lookAhead->tokenType == KW_FUNCTION){
		if(lookAhead->tokenType == KW_PROCEDURE){
			compileProcDecl();
		}else{
			eat(KW_FUNCTION);
			eat(TK_IDENT);
			if(lookAhead->tokenType != SB_COLON){
				compileParams();
			}
			eat(SB_COLON);
			compileBasicType();
			eat(SB_SEMICOLON);
			compileBlock();
			eat(SB_SEMICOLON);
		}	
	}
  assert("Function parsed ....");
}

void compileParams(void) {
  // TODO of anhtu
	while(true){
		eat(SB_LPAR);
		if(lookAhead->tokenType == KW_VAR){
			eat(KW_VAR);		
		}
		eat(TK_IDENT);
		eat(SB_COLON);
		compileBasicType();
		if(lookAhead->tokenType == SB_RPAR){
			eat(SB_RPAR);
			break;	
		}
		eat(SB_SEMICOLON);
	}	
}

void compileUnsignedConstant(void) {
  // TODO of anhtu
	switch(lookAhead->tokenType){
		case TK_NUMBER: eat(TK_NUMBER); break;
		case TK_CHAR : eat(TK_CHAR); break;
		case TK_IDENT : eat(TK_IDENT); break;
	}
}

void compileConstant(void) {
  // TODO of anhtu
	if(lookAhead->tokenType == SB_PLUS || lookAhead->tokenType == SB_MINUS){
		if(lookAhead->tokenType == SB_PLUS){
			eat(SB_PLUS);
		}else if(lookAhead->tokenType == SB_MINUS){
			eat(SB_MINUS);
		}
		if(lookAhead->tokenType == TK_IDENT){
			eat(TK_IDENT);
		}else if(lookAhead->tokenType == TK_NUMBER){
			eat(TK_NUMBER);
		}
	}else if(lookAhead->tokenType == TK_CHAR){
		eat(TK_CHAR);
	}
}

void compileType(void) {
  // TODO of anhtu
	switch(lookAhead->tokenType){
		case TK_IDENT : eat(TK_IDENT); break;
		case KW_ARRAY : eat(KW_ARRAY); eat(SB_LSEL); eat(TK_NUMBER); eat(SB_RSEL); eat(KW_OF); compileType(); break;
		default : compileBasicType();
	}
}

void compileBasicType(void) {
  // TODO of anhtu
	switch(lookAhead->tokenType){
		case KW_INTEGER : eat(KW_INTEGER); break;
		case KW_CHAR : eat(KW_CHAR); break;	
	}
}

void compileStatements(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  // TODO of TuanDat
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO of TuanDat
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO of TuanDat
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO of TuanDat
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO of TuanDat
  assert("For statement parsed ....");
}

void compileExpression(void) {
  assert("Parsing an expression");
  // TODO of TuanDat
  assert("Expression parsed");
}

void compileTerm(void) {
  // TODO of TuanDat
}

void compileFactor(void) {
  // TODO of TuanDat
}

void compileCondition(void) {
  // TODO of TuanDat
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
