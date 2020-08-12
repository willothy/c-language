#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include "type.h"

struct vector {
	void* value;
	struct vector* next;
};

typedef struct vector* Vector;

void* vRemove(int index, Vector object);

void vAppend(Vector object, void* value);

int vLen(Vector object);

void* vGet(int index, Vector object);

void vInsert(int index, Vector object, void* value);

Vector vNew();

void vDelete(Vector object);

struct val {
	ValType type;
	union {
		char* string;
		int integer;
	};
};

typedef struct val Val;

struct var {
	char* name;
	Val value;
};

typedef struct var* VarP;
typedef struct var Var;

bool varExists(Vector object, char* varName);

Val varGet(Vector object, char* varName);

void varSet(Vector object, char* varName, Val value);

VarP varFind(Vector object, char* varName);

void varPrint(Vector object); // Prints values and names of all variables

#endif
