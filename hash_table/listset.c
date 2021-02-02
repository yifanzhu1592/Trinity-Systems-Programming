#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// include the header files with the declarations of listset
#include "listset.h"

// create a new, empty linked list set
struct listset * listset_new() {
	struct listset * result;
	result = malloc(sizeof(struct listset));
	assert(result != NULL);
	result -> head = NULL;
	return result;
}

/* check to see if an item is in the set
   returns 1 if in the set, 0 if not */
int listset_lookup(struct listset * this, char * item) {
	struct listnode * p;
	for (p = this -> head; p != NULL; p = p -> next) {
		if (strcmp(p -> str, item) == 0) {
			return 1;
		}
	}
	return 0;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set.
// New items that are not already in the set should
// be added to the start of the list
void listset_add(struct listset * this, char * item) {
	if (listset_lookup(this, item) == 0) {
		struct listnode * node;
		node = malloc(sizeof(struct listnode));
		node -> str = item;
		node -> next = this -> head;
		this -> head = node;
	}
}

// remove an item with number 'item' from the set
void listset_remove(struct listset * this, char * item) {
	if (strcmp(this -> head -> str, item) == 0) {
		this -> head = this -> head -> next;
	} else {
		struct listnode * p, * pre;
		for (pre = this -> head, p = this -> head -> next; p != NULL; pre = pre -> next, p = p -> next) {
			if (strcmp(p -> str, item) == 0) {
				pre -> next = p -> next;
			}
		}
	}
}
  
// place the union of src1 and src2 into dest
void listset_union(struct listset * dest, struct listset * src1,
		   struct listset * src2) {
	dest -> head = src1 -> head;
	struct listnode * p;
	for (p = src1 -> head; p -> next != NULL; p = p -> next) {
	}
	for (struct listnode * p2 = src2 -> head; p2 != NULL; p2 = p2 -> next) {
		if (listset_lookup(src1, p2 -> str) == 0) {
			p -> next = p2;
			p = p -> next;
		}
	}
	p -> next = NULL;
}

// place the intersection of src1 and src2 into dest
void listset_intersect(struct listset * dest, struct listset * src1,
		       struct listset * src2) {
	struct listnode * p = malloc(sizeof(struct listnode));
	dest -> head = p;
	for (struct listnode * p1 = src1 -> head; p1 != NULL; p1 = p1 -> next) {
		if (listset_lookup(src2, p1 -> str) == 1) {
			struct listnode * node = malloc(sizeof(struct listnode));
			node -> str = p1 -> str;
			p -> next = node;
			p = p -> next;
		}
	}
	p -> next = NULL;
	dest -> head = dest -> head -> next;
}

// return the number of items in the listset
int listset_cardinality(struct listset * this) {
	struct listnode * p;
	int counter = 0;
	for (p = this -> head; p != NULL; p = p -> next) {
		counter++;
	}
	return counter;
}

// print the elements of the list set
void listset_print(struct listset * this) {
  struct listnode * p;

  for ( p = this->head; p != NULL; p = p->next ) {
    printf("%s, ", p->str);
  }
  printf("\n");
}
