//   list_string.c
//   David Gregg
//   January 2021

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "list_string.h"



// create a new list string with the same value as a normal
// NULL-terminated C string
struct list_string * new_list_string(char * text) {
	struct list_string * result;
	result = malloc(sizeof(struct list_string));
	result -> head = malloc(sizeof(struct ls_node)); // the head is a dummy node
	struct ls_node * p;
	p = result -> head;
	while (text != NULL && *text != '\0') {
		struct ls_node * current_node;
		current_node = malloc(sizeof(struct ls_node));
		current_node -> c = *text;
		p -> next = current_node;
		p = p -> next;
		text++;
	}
	p -> next = NULL;
	result -> head = result -> head -> next;
	return result;
}

// find the length of the list string
int list_string_length(struct list_string * this) {
	int length = 0;
	struct ls_node * p = this -> head;
	while (p != NULL) {
		length++;
		p = p -> next;
	}
	return length;
}


// compare two strings; return -1 is s1 is lexicographically less that s2;
// return 0 if the strings are equal; return 1 if s1 is lexicographically
// larger than s2. E.g. "aB" is less than "ab" because 'B' has a smaller
// ASCII code than 'b'. Also "abc" is less that "abcd". 
int list_string_compare(struct list_string * s1, struct list_string * s2) {
	struct ls_node * p1 = s1 -> head;
	struct ls_node * p2 = s2 -> head;
	while (p1 != NULL && p2 != NULL) {
		if (p1 -> c < p2 -> c) {
			return -1;
		} else if (p1 -> c > p2 -> c) {
			return 1;
		} else {
			p1 = p1 -> next;
			p2 = p2 -> next;
		}
	}
	if (p1 == NULL && p2 == NULL) {
		return 0;
	} else if (p1 == NULL) {
		return -1;
	} else {
		return 1;
	}
}

// return 1 if the string starts at n2 is the substring of the string starts at n1; 0 otherwise
int list_string_compare_by_node(struct ls_node * n1, struct ls_node * n2) {
	struct ls_node * p1 = n1;
	struct ls_node * p2 = n2;
	while (p1 != NULL && p2 != NULL) {
		if (p1 -> c != p2 -> c) {
			return 0;
		} else {
			p1 = p1 -> next;
			p2 = p2 -> next;
		}
	}
	if (p2 == NULL) {
		return 1;
	} else {
		return 0;
	}
}

// return 1 if str is a substring of text; 0 otherwise
int list_string_substring(struct list_string * text, struct list_string * str) {
	struct ls_node * p = text -> head;
	while (p != NULL) {
		if (list_string_compare_by_node(p, str -> head) == 1) {
			return 1;
		}
		p = p -> next;
	}
	return 0;
}


