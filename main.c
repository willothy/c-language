#include "y.tab.h"
#include "ast.h"
#include "function.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

extern struct ASTNode* ast;

Vector funcs;

Val evaluate(struct ASTNode* n, Vector Vars) {
	ASTType astType = n->type;
	Val v;
	if (astType == Beginning) {
		for (int i = 0; i < vLen(n->vector); i++) {
			void* vv = vGet(i, n->vector);
			evaluate((struct ASTNode*)vv, Vars);
		}
		v.type = Integer;
		v.integer = 1;
		return v;
	}
	if (astType == Value) {
		return n->value;
	}
	if (astType == Print) {
		Val l = evaluate(n->left, Vars);
		if (l.type == String)
			printf("%s\n", l.string);
		else if (l.type == Integer)
			printf("%d\n", l.integer);
		else {
			printf("FATAL ERROR: Unknown type.\n");
			exit(0);
		}
		v.integer = 1;
		v.type = Integer;
		return v;
	}
	if (astType == FunctionCall) {
		Vector fVars = n->vector;
		Vector current = fVars;
		Vector local = vNew();
		struct ASTNode* Func = fFind(n->text, funcs);
		Vector argList = Func->vector;
		for (int i = 0; i < vLen(fVars); i++) {
			argList = argList->next;
			current = current->next;
			char* vName = ((VarP)argList->value)->name;
			Val fVal = evaluate(current->value, Vars);
			VarP v = calloc(1, sizeof(Var));
			v->name = vName;
			v->value = fVal;
			vAppend(local, v);
		}
		return evaluate(Func->left, local);
	}
	if (astType == Strlen) {
		v.type = Integer;
		v.integer = (int)strlen(evaluate(n->left, Vars).string);
		return v;
	}
	if (astType == Input) {
		printf("%s", evaluate(n->left, Vars).string);
	    char* str = calloc(1, sizeof(char*));
	    fgets(str, 300, stdin);
	    char* toNull = strrchr(str, '\n');
	    *toNull = '\0';
		v.type = String;
		v.string = str;
		varSet(Vars, n->right->text, v);
		return v;
	}
	if (astType == Variable) {
		return varGet(Vars, n->text);
	}
	if (astType == Assignment) {
		v = evaluate(n->right, Vars);
		varSet(Vars, n->left->text, v);
		//varPrint(Vars);
		return v;
	}
	if (astType == PlusEquals) {
		Val varV = varGet(Vars, n->left->text);
		v = evaluate(n->right, Vars);
		varV.integer += v.integer;
		varSet(Vars, n->left->text, varV);
		return v;
	}
	if (astType == MinusEquals) {
		Val varV = varGet(Vars, n->left->text);
		v = evaluate(n->right, Vars);
		varV.integer -= v.integer;
		varSet(Vars, n->left->text, varV);
		return v;
	}
	if (astType == Decrement) {
		v = evaluate(n->left, Vars);
		v.integer--;
		varSet(Vars, n->left->text, v);
		return v;
	}
	if (astType == Increment) {
		v = evaluate(n->left, Vars);
		v.integer++;
		varSet(Vars, n->left->text, v);
		return v;
	}
	if (astType == StringSection) {
		char* text = evaluate(n->left, Vars).string;
		int start = evaluate(n->right, Vars).integer;
		int end = evaluate(n->extra, Vars).integer;

		v.type = String;
		v.string = strndup(text+start, end-start+1);
		return v;
	}
	if (astType == StringReplace) {
		char* text = n->text;
		Val got = varGet(Vars, text);
		int start = evaluate(n->left, Vars).integer;
		int end = evaluate(n->right, Vars).integer;
		char* insert = calloc(1, sizeof(char*));
		char* first = calloc(1, sizeof(char*));
		char* last = calloc(1, sizeof(char*));
		insert = evaluate(n->extra, Vars).string;
		first = strndup(got.string, start);
		last = strndup(got.string+end, strlen(got.string)-end);
		char* final = calloc(strlen(first) + strlen(insert) + strlen(last), sizeof(char));
		strcat(final, first);
		strcat(final, insert);
		strcat(final, last);
		v.type = String;
		v.string = final;
		free(first);
		free(last);
		free(insert);
		varSet(Vars, text, v);
		return v;
	}
	if (astType == Comparison) {
		char* text = n->text;
		v.type = Integer;
		Val l = evaluate(n->left, Vars);
		Val r = evaluate(n->right, Vars);
		/* 
		strcmp(str1, str2)
		if Return value < 0 then it indicates str1 is less than str2.
		if Return value > 0 then it indicates str2 is less than str1.
		if Return value = 0 then it indicates str1 is equal to str2.
		*/
		if (l.type == String && r.type == String) {
			if (strcmp(text, "==") == 0) {
				if (strcmp(l.string, r.string) == 0) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, "!=") == 0) {
				if (strcmp(l.string, r.string) != 0) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, "<") == 0) {
				if (strcmp(l.string, r.string) < 0) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, ">") == 0) {
				if (strcmp(l.string, r.string) > 0) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, "<=") == 0) {
				if (strcmp(l.string, r.string) <= 0) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, ">=") == 0) {
				if (strcmp(l.string, r.string) >= 0) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
		} else if (l.type == Integer && r.type == Integer) {
			if (strcmp(text, "==") == 0) {
				if (l.integer == r.integer) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, "!=") == 0) {
				if (l.integer != r.integer) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, "<") == 0) {
				if (l.integer < r.integer) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, ">") == 0) {
				v.integer = (int)(l.integer > r.integer);
				return v;
			}
			if (strcmp(text, "<=") == 0) {
				if (l.integer <= r.integer) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
			if (strcmp(text, ">=") == 0) {
				if (l.integer >= r.integer) {
					v.integer = 1; return v;
				}
				else {
					v.integer = 0; return v;
				}
			}
		} else {
			printf("ERROR: Cannot compare 2 variables of different types.\n");
			exit(0);
		}
	}
	if (astType == While) {
		while(true) {
			if (evaluate(n->left, Vars).integer == 1)
				evaluate(n->right, Vars);
			else
				break;
		}
		v.type = Integer;
		v.integer = 1;
		return v;
	}
	if (astType == If) {
		if (evaluate(n->left, Vars).integer == 1) {
			return evaluate(n->right, Vars);	
		} else {
			return evaluate(n->extra, Vars);
		}
	}
	if (astType == Plus) {
		Val l = evaluate(n->left, Vars);
		Val r = evaluate(n->right, Vars);
		if (l.type == String && r.type == String) {
			v.type = String;
			v.string = calloc(strlen(l.string) + strlen(r.string)+1, sizeof(char));
			strcat(v.string, l.string); 
			strcat(v.string, r.string);
			return v;
		} else if (l.type == Integer && r.type == Integer) {
			v.type = Integer;
			v.integer = l.integer + r.integer;
			return v;
		} else {
			printf("Failed to evaluate addition operation, arguments were of different types.\n");
			exit(0);
			return v;
		}
	}
	if (astType == Minus) {
		Val l = evaluate(n->left, Vars);
		Val r = evaluate(n->right, Vars);
		if (l.type == Integer && r.type == Integer) {
			v.type = Integer;
			v.integer = l.integer - r.integer;
			return v;
		}
	}
	if (astType == Times) {
		Val l = evaluate(n->left, Vars);
		Val r = evaluate(n->right, Vars);
		if (l.type == Integer && r.type == Integer) {
			v.type = Integer;
			v.integer = l.integer * r.integer;
			return v;
		}
	}
	if (astType == Divide) {
		Val l = evaluate(n->left, Vars);
		Val r = evaluate(n->right, Vars);
		if (l.type == Integer && r.type == Integer) {
			v.type = Integer;
			v.integer = l.integer / r.integer;
			return v;
		}
	}
	printf("failed to evaluate\n");
}

int main(int argc, char** argv) {
	extern FILE * yyin;
	yyin = fopen(argv[1] ,"r");
	funcs = vNew();
  	yyparse();
  	Vector vars = vNew();
  	evaluate(ast, vars);
  	//varPrint(Vars);
  	//printf("%d\n", (int)varExists(Vars, "var"));
  	//printAST(ast);
}


