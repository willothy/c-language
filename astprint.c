#include "y.tab.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

extern struct ASTNode* ast;

int main(int argc, char** argv) {
	yyparse();
	//printAST(ast);
}