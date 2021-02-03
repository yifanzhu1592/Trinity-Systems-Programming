// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "huff.h"

// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{
	struct huffcoder * result;
	result = malloc(sizeof(struct huffcoder));
	for (int i = 0; i < 256; i++) {
		result -> freqs[i] = 0;
	}
	for (int i = 0; i < 256; i++) {
		result -> code_lengths[i] = 0;
	}
	//for (int i = 0; i < 256; i++) {
		//result -> codes[i] = NULL;
	//}
	
	result -> tree = malloc(sizeof(struct huffchar));
	return result;
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
	FILE *fp;
	fp = fopen(filename, "r");
	unsigned char c;
	while (!feof(fp)) {	
		c = fgetc(fp);
		this -> freqs[c]++;
	}
	for (int i = 0; i < 256; i++) {
		if (this -> freqs[i] == 0) {
			this -> freqs[i]++;
		}
	}
	fclose(fp);
}

struct huffchar * new_compound(struct huffchar * min1, struct huffchar * min2, int seqno) {
	struct huffchar * result;
	result = malloc(sizeof(struct huffchar));
	result -> freq = min1 -> freq + min2 -> freq;
	result -> is_compound = 1;
	result -> seqno = seqno;
	result -> u.compound.left = min1;
	result -> u.compound.right = min2;
	return result;
}

int find_min(struct huffchar ** list, int list_size) {
	int min = list[0] -> freq;
	int min_idx = 0;
	int seqno=list[0]->seqno;
	for (int i = 1; i < list_size; i++) {
		if (list[i] -> freq < min || (list[i] -> freq == min && list[i] -> seqno < seqno)) {
			min = list[i] -> freq;
			seqno=list[i]->seqno;
			min_idx = i;
		}
	}
	return min_idx;
}

// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
	struct huffchar ** list;
	list = malloc(sizeof(struct huffchar *) * 256);
	for (int i = 0; i < 256; i++) {
		list[i] = malloc(sizeof(struct huffchar));
		list[i] -> freq = this -> freqs[i];
		list[i] -> is_compound = 0;
		list[i] -> seqno = i;
		list[i] -> u.c = i;
	}
	int list_size = 256;
	int seqno = 256;
	while (list_size > 1) {
		int smallest = find_min(list, list_size);
		struct huffchar * min1 = list[smallest];
		list[smallest] = list[list_size - 1];
		smallest = find_min(list, list_size - 1);
		struct huffchar * min2 = list[smallest];
		list[smallest] = list[list_size - 2];
		struct huffchar * compound;
		compound = new_compound(min1, min2, seqno);
		seqno++;
		list_size--;
		list[list_size - 1] = compound;
	}
	this -> tree = list[0];
}

char * clone_string(char * path, int depth) {
	char * clone = malloc(depth * sizeof(char));
	path[depth] = '\0';
	strcpy(clone, path);
	return clone;
}

void write_codes_recursive(struct huffcoder * this, struct huffchar * current, char * path, int depth) {
	if (current -> is_compound == 1) {
		path[depth] = '0';
		write_codes_recursive(this, current -> u.compound.left, path, depth + 1);
		path[depth] = '1';
		write_codes_recursive(this, current -> u.compound.right, path, depth + 1);
	} else {
		this -> code_lengths[current -> u.c] = depth;
		this -> codes[current -> u.c] = clone_string(path, depth);
	}
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this)
{
	char * path = malloc(sizeof(int) * 256);
	write_codes_recursive(this, this -> tree, path, 0);
}

// print the Huffman codes for each character in order;
// you should not modify this function
void huffcoder_print_codes(struct huffcoder * this)
{
  for ( int i = 0; i < NUM_CHARS; i++ ) {
    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], this->codes[i]);
  }
}

// encode the input file and write the encoding to the output file
void huffcoder_encode(struct huffcoder * this, char * input_filename,
    char * output_filename)
{
	FILE * input = fopen(input_filename, "r");
	FILE * output = fopen(output_filename, "w");
	unsigned char c = fgetc(input);
	while (!feof(input)) {
		fputs(this -> codes[c], output);
		c = fgetc(input);
	}
	fputs(this -> codes[4], output);
	fclose(input);
	fclose(output);
}

// decode the input file and write the decoding to the output file
void huffcoder_decode(struct huffcoder * this, char * input_filename,
    char * output_filename)
{
	FILE * input = fopen(input_filename, "r");
	FILE * output = fopen(output_filename, "w");
	unsigned char c = fgetc(input);
	while (!feof(input)) {
		struct huffchar * p = this -> tree;
		while (p -> is_compound) {
			if (c == '0') {
				p = p -> u.compound.left;
		  } else {
		  	p = p -> u.compound.right;
		  }
		  c = fgetc(input);
		}
		if (p -> u.c == 4) {
			return;
		}
		fputc(p -> u.c, output);
		p = this -> tree;
	}
	fclose(input);
	fclose(output);
}
