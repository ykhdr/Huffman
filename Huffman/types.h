#pragma once
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 256
#define NOTNODE INT_MAX
#define LEFT '0'
#define RIGHT '1'
#define TREERECRAIONSIZE 8*SIZE+1
#define SUCCESS '0'
#define COUNTARGMINERROR '1'
#define COUNTARGMAXERROR '2'
#define PARAMERROR '3'
#define HELP '4'
#define STREAMINERROR '5'
#define STREAMOUTERROR '6'

typedef struct stream_s {
	FILE* in;
	FILE* out;
}sStream;

typedef struct	repeats_s {
	int number;
	int amount;
}sRepeats;

typedef struct tree_s {
	int value;
	int amount;
	struct tree_s* left;
	struct tree_s* right;
}sTree;

typedef struct encode_s {
	unsigned char symbol;
	int lenght;
	unsigned char code[SIZE];
}sEncode;
