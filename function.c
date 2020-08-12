#include "function.h"
#include "string.h"

struct ASTNode* fFind(char* name, Vector object) {
	Vector original = object;
	for (int i = 0; i < vLen(original); i++) {
		object = object->next;
		if (strcmp(((struct ASTNode*)object->value)->text, name) == 0) {
			return object->value;
		}
	}
	return NULL;
}