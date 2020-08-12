%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "vector.h"
    #include "ast.h"
    extern int evaluate();
    void yyerror(char *);
    int yylex(void);
    struct ASTNode* ast;
    extern Vector funcs;
%}

%union {
    struct ASTNode* ast;
    struct vector* vec;
}

%token INTEGER STRING VARIABLE IF ELSE YEQ NEQ WHILE DO PRINT STRLEN INPUT FUNCTION INCR DECR PLUSEQ MINUSEQ TYP_STRING TYP_INTEGER TYP_ARRAY
%nonassoc '[' ']' ':'
%left '+' '-'
%left '*' '/'
%left '<' '>' NEQ YEQ GEQ LEQ PLUSEQ MINUSEQ INCR DECR

%type <ast> expression statement block program function
%type <ast> INTEGER STRING VARIABLE
%type <vec> arglist arglist_nonempty paramlist paramlist_nonempty

%%

program:
        function program                {
                                            vAppend(funcs, $1);
                                        }
        | block                         {
                                            ast = $1;
                                            $$ = $1;
                                        }
        ;

block:
        block statement                 {
                                            vAppend($1->vector, $2);
                                            $$ = $1;
                                        }
        | /*  NULL  */                  {
                                            struct ASTNode* ast2 = calloc(1, sizeof(struct ASTNode));
                                            Vector vector = vNew();
                                            ast2->type = Beginning;
                                            ast2->vector = vector;
                                            $$ = ast2;
                                        }
        ;

statement:
        expression ';'                  { $$ = $1; }
        | VARIABLE '=' expression ';'   {
                                            //printf("var in parser (stmnt): Name: %s, Value: %d, Pointer: %p\n", $1->text, $1->value, $1);
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $1; 
                                            a->left->type = Variable;
                                            a->left->text = $1->text;
                                            a->right = $3;
                                            a->type = Assignment;
                                            $$ = a;
                                        }
        | VARIABLE PLUSEQ expression ';'    {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $1; 
                                            a->left->type = Variable;
                                            a->left->text = $1->text;
                                            a->right = $3;
                                            a->type = PlusEquals;
                                            $$ = a;
                                        }
        | VARIABLE MINUSEQ expression ';'   {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $1; 
                                            a->left->type = Variable;
                                            a->left->text = $1->text;
                                            a->right = $3;
                                            a->type = MinusEquals;
                                            $$ = a;
                                        }
        | VARIABLE '[' expression ':' expression ']' '=' expression ';' {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $3;
                                            a->right = $5;
                                            a->extra = $8;
                                            a->text = $1->text;
                                            a->type = StringReplace;
                                            $$ = a;
                                        }
        | WHILE '(' expression ')' '{' block '}'   {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $3;
                                            a->right = $6;
                                            a->type = While;
                                            $$ = a;
                                        }
        | IF '(' expression ')' '{' block '}' ELSE '{' block '}'     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $3;
                                            a->right = $6;
                                            a->extra = $10;
                                            a->type = If;
                                            $$ = a;
                                        }
        | PRINT expression ';'          {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $2;
                                            a->type = Print;
                                            $$ = a;
                                        }
        | INPUT expression VARIABLE ';' {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $2;
                                            a->right = $3;
                                            a->type = Input;
                                            $$ = a;
                                        }
        | VARIABLE INCR ';'             {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $1;
                                            a->left->text = $1->text;
                                            a->left->type = Variable;
                                            a->type = Increment;
                                            $$ = a;
                                        }
        | VARIABLE DECR ';'             {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $1;
                                            a->left->text = $1->text;
                                            a->left->type = Variable;
                                            a->type = Decrement;
                                            $$ = a;
                                        }
        ;

expression:
        INTEGER
        | STRING
        | VARIABLE '(' paramlist ')'      {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = $1->text;
                                            a->vector = $3;
                                            a->type = FunctionCall;
                                            $$ = a;
                                        }
        | expression '[' expression ':' expression ']' {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $1;
                                            a->right = $3;
                                            a->extra = $5;
                                            a->type = StringSection;
                                            $$ = a;
                                        }

        | VARIABLE                      { 
                                            //printf("var in parser (expr): Name: %s, Value: %d, Pointer: %p\n", $1->text, $1->value, $1);
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = $1->text;
                                            a->left = NULL;
                                            a->right = NULL;
                                            a->type = Variable;
                                            $$ = a;
                                        }
        | STRLEN expression             {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->left = $2;
                                            a->type = Strlen;
                                            $$ = a;
                                        }
        | expression YEQ expression     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = "==";
                                            a->type = Comparison;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a;
                                        }
        | expression NEQ expression     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = "!=";
                                            a->type = Comparison;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a;
                                        }
        | expression '<' expression     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = "<";
                                            a->type = Comparison;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a;
                                        }
        | expression '>' expression     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = ">";
                                            a->type = Comparison;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a;
                                        }
        | expression LEQ expression     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = "<=";
                                            a->type = Comparison;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a;
                                        }
        | expression GEQ expression     {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->text = ">=";
                                            a->type = Comparison;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a;
                                        }
        | expression '+' expression     { 
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->type = Plus;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a; 
                                        }
        | expression '-' expression     { 
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->type = Minus;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a; 
                                        }
        | expression '*' expression     { 
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->type = Times;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a; 
                                        }
        | expression '/' expression     { 
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->type = Divide;
                                            a->left = $1;
                                            a->right = $3;
                                            $$ = a; 
                                        }
        | '(' expression ')'            { $$ = $2; }
        ;

type:
    TYP_INTEGER
    | TYP_STRING
    | TYP_ARRAY '<' type '>'
    ;

arglist:
        arglist_nonempty                { $$ = $1; }
        | /* NULL */                    { $$ = vNew(); }
        ;

arglist_nonempty:
        VARIABLE                        {
                                            Vector localVars = vNew();
                                            VarP v = calloc(1, sizeof(Var));
                                            v->name = $1->text;
                                            vAppend(localVars, v);
                                            $$ = localVars;
                                        }
        | arglist_nonempty ',' VARIABLE          {
                                            VarP v = calloc(1, sizeof(Var));
                                            v->name = $3->text;
                                            vAppend($1, v);
                                            $$ = $1;
                                        }
        ;

paramlist:
        paramlist_nonempty              { $$ = $1; }
        | /* NULL */                    { $$ = vNew(); }
        ;

paramlist_nonempty:
        expression                      {
                                            Vector localVars = vNew();
                                            vAppend(localVars, $1);
                                            $$ = localVars;
                                        }
        | paramlist_nonempty ',' expression          {
                                            vAppend($1, $3);
                                            $$ = $1;
                                        }
        ;

function: 
        FUNCTION VARIABLE '(' arglist ')' '{' block '}' {
                                            struct ASTNode* a = calloc(1, sizeof(struct ASTNode));
                                            a->type = FunctionDeclaration;
                                            a->text = $2->text;
                                            a->left = $7;
                                            a->vector = $4;
                                            $$ = a;
                                        }
        ;

%%

void yyerror(char *s) {
    printf("PARSER ERROR\n");
    fprintf(stderr, "%s\n", s);
}


