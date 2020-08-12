#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

/*void printAST (struct ASTNode* n) {
	char op = n->operator;
	if (op == 'B') {
		for (int i = 0; i < vLen(n->vector); i++) {
			void* v = vGet(i, n->vector);
			printAST((struct ASTNode*)v);
		}
		return;
	}
	if (op == '\0') {
		// Integer
		printf("Integer: %d\n", n->value.integer);
		return;
	}
	if (op == 'P') {
		printf("Print statement\n");
		printAST(n->left);
		return;
	}
	if (op == 'V') {
		printf("Variable: %s\n", n->text);
		return;
	}
	if (op == '=') {
		printf("=\n");
		printAST(n->left);
		printAST(n->right);
		return;
	}
	if (op == 'C') {
		printf("%s\n", n->text);
		printAST(n->left);
		printAST(n->right);
		return;
	}
	if (op == 'W') {
		printf("while\n");
		printAST(n->left);
		printAST(n->right);
		return;
	}
	if (op == 'I') {
		printf("if\n");
		printAST(n->left);
		printAST(n->right);
		printAST(n->extra);
		return;
	}
	if (op == '+' || op == '*' || op == '-' || op == '/') {
		printf("%c\n", op);
		printAST(n->left);
		printAST(n->right);
		return;
	}
	printf("failed to print AST.");
} */