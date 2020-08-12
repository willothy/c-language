#ifndef AST_H
#define AST_H

#include "vector.h"

enum asttype {
	Beginning,
	Value,
	Print,
	Variable,
	Assignment,
	VariableDeclaration,
	StringSection,
	StringReplace,
	Comparison,
	While,
	If,
	Plus,
	Minus,
	Times,
	Divide,
	Strlen,
	Input,
	FunctionDeclaration,
	FunctionCall,
	Increment,
	Decrement,
	PlusEquals,
	MinusEquals
};

typedef enum asttype ASTType;

struct ASTNode {
	struct ASTNode* right;
	struct ASTNode* left;
	struct ASTNode* extra;
	ASTType type;
	char* text;
	Val value;
	Vector vector;
};

//void printAST();

#endif