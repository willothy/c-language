#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "vector.h"


void* vRemove(int index, Vector object) {
	for (int i = 0; i < index-1; i++) {
		if (object == NULL) 
			return NULL;
		object = object->next;
	}
	if (object == NULL) {
		return NULL;
	}
	Vector temp = object->next;
	object->next = object->next->next;
	return temp->value;
}

void vAppend(Vector object, void* value) {
	while (object->next != NULL) {
		object = object->next;
	}
	Vector new = calloc(1, sizeof(struct vector));
	new->value = value;
	object->next = new;
	return;
}

int vLen(Vector object) {
	int i = 0;
	while (object->next != NULL) {
		i++;
		object = object->next;
		continue;
	}
	return i;
}

void* vGet(int index, Vector object) {
	for (int i = 0; i < index+1; i++) {
		if (object == NULL) {
			return NULL;
		}
		object = object->next;
	}
	return object->value;
}

void vInsert(int index, Vector object, void* value) {
	for (int i = 0; i < index; i++) {
		if (object == NULL) {
			return;
		}
		object = object->next;
	}
	Vector next = object->next;
	Vector new = calloc(1, sizeof(struct vector));
	new->value = value;
	new->next = next;
	object->next = new;
	return;

}

Vector vNew() {
	Vector vector = calloc(1, sizeof(struct vector));
	vector->value = NULL;
	return vector;
}

void vDelete(Vector object) {
	Vector new = object->next;
	while (new != NULL) {
		free(object);
		new = new->next;
		object = object->next;
	}
	free(new);
	return;
}

VarP varFind(Vector object, char* varName) {
	Vector original = object;
	for (int i = 0; i < vLen(original); i++) {
		if (object->next != NULL)
			object = object->next;
		else
			return NULL;

		if (strcmp(((VarP)object->value)->name, varName) == 0) {
			return (VarP)object->value;
		}
	}
	return NULL;
}

bool varExists(Vector object, char* varName) {
	if (varFind(object, varName) != NULL) {
		//printf("true\n");
		return true;
	}
	//printf("false\n");	
	return false;
}

Val varGet(Vector object, char* varName) {
	if (varExists(object, varName)) {
		//printf("var hath been got\n");
		VarP v = varFind(object, varName);
		return v->value;
	} else {
		printf("Could not find variable %s. Terminating.", varName);
		exit(0);
	}
}

void varSet(Vector object, char* varName, Val value) {
	if (varExists(object, varName) == true) {
		varFind(object, varName)->value = value;
		//printf("var exists and has been found\n");
		//printf("var value has been set\n");
	} else {
		//printf("var has not been found, creating var now. Name: %s, Value: %d\n", varName, value);
		VarP newVarP = calloc(1, sizeof(struct var));
		//printf("var object created\n");
		newVarP->name = varName;
		//printf("var name has been assigned. newVarP->name: %s\n", newVarP->name);
		newVarP->value = value;
		//printf("value has been assigned. newVarP->value: %d\n", newVarP->value);
		vAppend(object, newVarP);
		//printf("Vector has been appended.\n");
	}
	return;
}

void varPrint(Vector object) {
	Vector original = object;
	for (int i = 0; i < vLen(original); i++) {
		object = object->next;
		//printf("i: %d, Name: %s, Value: %d\n", i, ((VarP)object->value)->name, ((VarP)object->value)->value);
	}
	return;
}