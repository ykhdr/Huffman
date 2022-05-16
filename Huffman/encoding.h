#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "types.h"
#include "inout.h"
#include <stdio.h>
#include <stdlib.h>

void fillNumberOfSymbolsIn(FILE* in, sRepeats* NumberOfSymbols, int* N);
void swapForNumberOfSymbols(sRepeats* arr, int i, int j);
void searchForTheSameAmount(sRepeats* NumberOfSymbols, int N);
void ShellSortForNumberOfSymbols(sRepeats* arr, int N);
int countDifferentSymbols(sRepeats arr[]);

void swapForTree(sTree* arr[], int i, int j);
void bubbleSort(sTree* arr[], int N);
void fillArrayOfTreeVertices(sTree* ArrayOfTreeVertices[], sRepeats NumberOfSymbols[], int N);
sTree* createCodingTree(sRepeats NumberOfSymbols[], int N);
void codingSymbol(sEncode* EncodedValues, sTree* t, int counter, int numberOfEV, unsigned char TreeEdgeDirection[]);
void prefOrder(sTree* t, sEncode* EncodedValues, int counter, unsigned char direction, char TreeEdgeDirection[], unsigned char* TreeRecration, bool flag);
void sortEV(sEncode* arr, int N);

void writeByte(FILE* out, unsigned char* bitBuffer, unsigned char bitCounter, unsigned char sym, bool flag);
unsigned char printInfoAboutCoding(FILE* out, int  difSym, unsigned char TreeRecration[], unsigned char* count);
void printEncodedMessage(FILE* in, FILE* out, sEncode* EncodedValues, int lenght, unsigned char bitBuffer, unsigned char bitCounter);
void print(FILE* in, FILE* out, sEncode* EncodedValues, int lenght, unsigned char TreeRecration[], int difSym);
void freeTreeForEncoding(sTree* t);
void encodeMessage(char* fileIn, char* fileOut);
