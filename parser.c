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

    //printf("%s - \n",tokenToString(lookAhead->tokenType));
    //printf("%s \n",tokenToString(tokenType));
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
	eat(KW_BEGIN);
	compileStatements();
	//eat(SB_SEMICOLON);
	while(lookAhead->tokenType == SB_SEMICOLON){
		eat(SB_SEMICOLON);
		compileStatements();
	}
	eat(KW_END);
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
	eat(SB_LPAR);	
	while(1){
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
		default : error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
	}
}

void compileConstant(void) {
  // TODO of anhtu
	if(lookAhead->tokenType == TK_CHAR){
		eat(TK_CHAR);
	}else if(lookAhead->tokenType == SB_PLUS || lookAhead->tokenType == SB_MINUS){
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
	}else if(lookAhead->tokenType == TK_IDENT){
		eat(TK_IDENT);
	}else if(lookAhead->tokenType == TK_NUMBER){
		eat(TK_NUMBER);
	}else{
		error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
	}
}

void compileType(void) {
  // TODO of anhtu
	switch(lookAhead->tokenType){
		case TK_IDENT : eat(TK_IDENT); break;
		case KW_ARRAY : eat(KW_ARRAY); eat(SB_LSEL); eat(TK_NUMBER); eat(SB_RSEL); eat(KW_OF); compileType(); break;
		case KW_INTEGER : compileBasicType(); break;
		case KW_CHAR : compileBasicType(); break;
		default : error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo); break;
	}
}

void compileBasicType(void) {
  // TODO of anhtu
	switch(lookAhead->tokenType){
		case KW_INTEGER : eat(KW_INTEGER); break;
		case KW_CHAR : eat(KW_CHAR); break;
		default : error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
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
  compileVariable();
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void) {
  assert("Parsing a call statement ....");
  // TODO of TuanDat
  eat(KW_CALL);
  eat(TK_IDENT);
  eat(SB_LPAR);
  compileExpression();
  while (lookAhead->tokenType==SB_COMMA){
    eat(SB_COMMA);
    compileExpression();
  }
  eat(SB_RPAR);
  assert("Call statement parsed ....");
}

void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  // TODO of TuanDat
    eat(KW_BEGIN);
    compileStatements();
    while(lookAhead->tokenType==SB_SEMICOLON){
        eat(SB_SEMICOLON);
        compileStatements();
    }
    eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatements();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatements();
}

void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  // TODO of TuanDat
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatements();
  assert("While statement pased ....");
}

void compileForSt(void) {
  assert("Parsing a for statement ....");
  // TODO of TuanDat
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatements();
  assert("For statement parsed ....");
}
void compileVariable(void){
    eat(TK_IDENT);
    while(lookAhead->tokenType==SB_LSEL){
        eat(SB_LSEL);
        compileExpression();
        eat(SB_RSEL);
    }
}
void compileExpression(void) {
  assert("Parsing an expression");
  // TODO of TuanDat
  if(lookAhead->tokenType==SB_PLUS)
    eat(SB_PLUS);
  else if(lookAhead->tokenType==SB_MINUS)
    eat(SB_MINUS);
  compileTerm();
  while(lookAhead->tokenType==SB_MINUS || lookAhead->tokenType==SB_PLUS){
    if(lookAhead->tokenType==SB_PLUS)
    eat(SB_PLUS);
  else
    eat(SB_MINUS);
   compileTerm();
  }
  assert("Expression parsed");
}

void compileTerm(void) {
  // TODO of TuanDat
  assert("Parsing a Term");
  compileFactor();
  while(lookAhead->tokenType==SB_TIMES || lookAhead->tokenType==SB_SLASH){
    if(lookAhead->tokenType==SB_TIMES){
        eat(SB_TIMES);
    } else {
        eat(SB_SLASH);
    }
    compileFactor();
  }
  assert("Term parsed");
}

void compileFactor(void) {
  // TODO of TuanDat
  assert("Compile a Factor");

  switch(lookAhead->tokenType){
      case SB_LPAR: eat(SB_LPAR); compileExpression(); eat(SB_RPAR); break;
      case TK_CHAR: compileUnsignedConstant(); break;
      case TK_NUMBER: compileUnsignedConstant(); break;
      case TK_IDENT: eat(TK_IDENT);
            switch(lookAhead->tokenType){
                    case SB_LPAR: eat(SB_LPAR);
                            compileExpression();
                            while (lookAhead->tokenType==SB_COMMA){
                                    eat(SB_COMMA);
                                    compileExpression();
                            }
                            eat(SB_RPAR);
                            break;
                    case SB_LSEL: while(lookAhead->tokenType==SB_LSEL){
                                eat(SB_LSEL);
                                compileExpression();
                                eat(SB_RSEL);
                            }
                            break;
            };
             break;
      default : error(ERR_INVALIDFACTOR,lookAhead->lineNo,lookAhead->colNo);
  }
  assert("Factor parsed");
}

void compileCondition(void) {
  // TODO of TuanDat
  compileExpression();
  switch(lookAhead->tokenType) {
    case SB_EQ:eat(SB_EQ); break;
    case SB_NEQ:eat(SB_NEQ); break;
    case SB_LT:eat(SB_LT); break;
    case SB_LE:eat(SB_LE); break;
    case SB_GT:eat(SB_GT); break;
    case SB_GE:eat(SB_GE); break;
    default :error(ERR_INVALIDCOMPARATOR,lookAhead->lineNo,lookAhead->colNo);
  }
  compileExpression();
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
