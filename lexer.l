%{
    #include <stdlib.h>
    #include "ast.h"
    #include "vector.h"
    #include "y.tab.h"
    #include <string.h>
    void yyerror(char *);

%}


%%

[0-9]+      {
                yylval.ast = calloc(1, sizeof(struct ASTNode));
                yylval.ast->value.integer = atoi(yytext);
                yylval.ast->value.type = Integer;
                yylval.ast->type = Value;
                yylval.ast->left = NULL;
                yylval.ast->right = NULL;
                return INTEGER;
            }

\"[^"]*\"     {
                yylval.ast = calloc(1, sizeof(struct ASTNode));
                yylval.ast->value.string = strndup(yytext+1, yyleng-2);
                yylval.ast->value.type = String;
                yylval.ast->type = Value;
                yylval.ast->left = NULL;
                yylval.ast->right = NULL;
                return STRING;
            }

[-+()=/*<>{}\[\]:;] { return *yytext; }

"string"        { return TYP_STRING; }

"int"           { return TYP_INTEGER; }

"Array"         { return TYP_ARRAY; }

"+="            { return PLUSEQ; }

"-="            { return MINUSEQ; }

"++"            { return INCR; }

"--"            { return DECR; }

">="            { return GEQ; }

"<="            { return LEQ; }

"=="            { return YEQ; }

"!="            { return NEQ; }

"while"         { return WHILE; }

"do"            { return DO; }

"if"            { return IF; }

"else"          { return ELSE; }

"print"         { return PRINT; }

"strlen"        { return STRLEN; }

"prompt"        { return INPUT; }

"function"      { return FUNCTION; }

[_a-zA-Z][_a-zA-Z0-9]*       {
                yylval.ast = calloc(1, sizeof(struct ASTNode));
                char* i = calloc((int)yyleng+1, sizeof(char));
                strncpy(i, yytext, (int)yyleng);
                yylval.ast->text = i;
                yylval.ast->left = NULL;
                yylval.ast->right = NULL;
                return VARIABLE;
            }

[ \t\n\r]   ;       /* skip whitespace */

.               { 
                    printf("%d\\/\n", (int)*yytext);
                    yyerror("Unknown character");
                }

%%

int yywrap(void) {
    return 1;
}
