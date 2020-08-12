#ifndef FUNCTION_H
#define FUNCTION_H

#include "ast.h"
#include "vector.h"

extern Vector funcs;

struct ASTNode* fFind(char* name, Vector object);

#endif