#pragma once
#include "types.h"
#include "inout.h"
#include <stdlib.h>
#include <stdio.h>

void scanSym(FILE* in, unsigned char* sym, unsigned char bitCounter, unsigned char* byte);
void crtDT(FILE* in, sTree* t, sTree* ancestor, unsigned char* bitCounter, unsigned char* byte, char direction, bool startFlag);
sTree* createStartDecodingTree();
sTree* treeRestoration(FILE* in, unsigned char* bitCounter, long* pos);
unsigned char findStartOfStartByte(FILE* in, unsigned char bitStart);
void printDecodeMessage(FILE* in, FILE* out, sTree* head, long startPos, long endPos, unsigned char bitStart, unsigned char lenghtOfLastBit);
unsigned char scanLenghtLastBit(FILE* in, long* endPos, long startPos);
void scanAndPrintDecodeMessage(FILE* in, FILE* out, sTree* head, long startPos, unsigned char bitCounter);
void freeTreeForDecoding(sTree* t);
void decodeMessage(char* fileIn, char* fileOut);
