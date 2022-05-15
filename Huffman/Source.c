#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 256
#define NOTNODE INT_MAX
#define LEFT '0'
#define RIGHT '1'
#define TREERECRAIONSIZE 8*SIZE+1

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

/////////////////////////////////// INPUT && OUTPUT ///////////////////////////////////

void fillStreamForEncoding(sStream* stream) {
	stream->in = fopen("input.bin", "rb");
	stream->out = fopen("output.bin", "wb");
}

void fillStreamForDecoding(sStream* stream) {
	stream->in = fopen("output.bin", "rb");
	stream->out = fopen("outoutput.bin", "wb");
}

void closeStreamForEncoding(sStream* stream) {
	fclose(stream->in);
	fclose(stream->out);
}

void closeStreamForDecoding(sStream* stream) {
	fclose(stream->in);
	fclose(stream->out);
}


///////////////////////////////////// SCAN MESSAGE //////////////////////////////////////


void fillNumberOfSymbolsIn(FILE* in, sRepeats* NumberOfSymbols, int* N) {
	for (int i = 0; i < SIZE; i++) {
		NumberOfSymbols[i].number = i;
		NumberOfSymbols[i].amount = 0;
	}
	unsigned char val;
	int i = 0;
	for (; fread(&val, 1, 1, in) == 1; i++)
		NumberOfSymbols[val].amount++;
	*N = i;
	fseek(in, -i, SEEK_CUR);
}
void swapForNumberOfSymbols(sRepeats* arr, int i, int j) {
	sRepeats buff = arr[i];
	arr[i] = arr[j];
	arr[j] = buff;
}

void searchForTheSameAmount(sRepeats* NumberOfSymbols, int N) {
	for (int i = 0; i < N - 1; i++) {
		if (NumberOfSymbols[i].amount == NumberOfSymbols[i + 1].amount)
			if (NumberOfSymbols[i].number > NumberOfSymbols[i + 1].number)
				swapForNumberOfSymbols(NumberOfSymbols, i, i + 1);

	}
}

void ShellSortForNumberOfSymbols(sRepeats* arr, int N) {
	int i, j, step;
	sRepeats tmp;
	for (step = N / 2; step >= 1; step /= 2)
		for (i = step; i < N; i++) {
			tmp = arr[i];
			for (j = i; j >= step; j -= step) {
				if ((tmp.amount >= arr[j - step].amount) || (arr[j - step].amount == 0 && tmp.amount != 0))
					arr[j] = arr[j - step];
				else
					break;
			}
			arr[j] = tmp;
		}
}
unsigned char countDifferentSymbols(sRepeats arr[]) {
	int i = 0;
	for (; arr[i].amount != 0; i++)
		if (i == UCHAR_MAX)
			break;;
	return i;
}


////////////////////////////////////// CREATE TREE ///////////////////////////////////////

void swapForTree(sTree* arr[], int i, int j) {
	sTree* buff = arr[i];
	arr[i] = arr[j];
	arr[j] = buff;
}

void bubbleSort(sTree* arr[], int N) {
	for (int i = 0; i < N - 1; i++)
		for (int j = 0; j < N - 1; j++)
			if (arr[j]->amount < arr[j + 1]->amount)
				swapForTree(arr, j + 1, j);

}

void fillArrayOfTreeVertices(sTree* ArrayOfTreeVertices[], sRepeats NumberOfSymbols[], int N) {
	for (int i = 0; i < N; i++) {
		sTree* empty = (sTree*)malloc(sizeof(sTree));
		ArrayOfTreeVertices[i] = empty;
		ArrayOfTreeVertices[i]->value = NumberOfSymbols[i].number;
		ArrayOfTreeVertices[i]->amount = NumberOfSymbols[i].amount;
		ArrayOfTreeVertices[i]->left = NULL;
		ArrayOfTreeVertices[i]->right = NULL;
	}
}

sTree* createCodingTree(sRepeats NumberOfSymbols[], int N) {
	sTree* ArrayOfTreeVertices[SIZE + 2] = { NULL };
	fillArrayOfTreeVertices(ArrayOfTreeVertices, NumberOfSymbols, SIZE + 2);
	sTree* head = NULL;
	int counter;
	if (N < SIZE)
		counter = N - 1;
	else
		counter = SIZE - 1;
	if (counter == 0)
		return ArrayOfTreeVertices[counter];
	while (counter) {
		sTree* buff = (sTree*)malloc(sizeof(sTree));
		buff->left = ArrayOfTreeVertices[counter - 1];
		buff->right = ArrayOfTreeVertices[counter];
		buff->amount = ArrayOfTreeVertices[counter - 1]->amount + ArrayOfTreeVertices[counter]->amount;
		buff->value = NOTNODE;
		ArrayOfTreeVertices[counter - 1] = buff;
		head = buff;
		bubbleSort(&ArrayOfTreeVertices, counter);
		counter--;
	}
	return head;
}

void codingSymbol(sEncode* EncodedValues, sTree* t, int counter, int numberOfEV, unsigned char TreeEdgeDirection[]) {
	unsigned char buffCodingSymbol[SIZE];
	for (int i = 1; i < counter; i++) {
		buffCodingSymbol[i - 1] = TreeEdgeDirection[i];
	}
	strncpy(EncodedValues[numberOfEV].code, buffCodingSymbol, counter);
	EncodedValues[numberOfEV].lenght = counter - 1;
	EncodedValues[numberOfEV].symbol = t->value;
}

int numberOfEncodedValue = 0;
int counterOfTreeRecration = 0;
void prefOrder(sTree* t, sEncode* EncodedValues, int counter, unsigned char direction, char TreeEdgeDirection[], unsigned char* TreeRecration, bool flag) {
	if (!t)
		return;
	if (counter)
		TreeEdgeDirection[counter] = direction;
	counter++;
	if (flag)
		TreeRecration[counterOfTreeRecration++] = '0';
	prefOrder(t->left, EncodedValues, counter, LEFT, TreeEdgeDirection, TreeRecration, true);
	if (t->value != NOTNODE) {
		TreeRecration[counterOfTreeRecration++] = '1';
		TreeRecration[counterOfTreeRecration++] = t->value;
		codingSymbol(EncodedValues, t, counter, numberOfEncodedValue, TreeEdgeDirection);
		numberOfEncodedValue++;
	}
	prefOrder(t->right, EncodedValues, counter, RIGHT, TreeEdgeDirection, TreeRecration, false);
}

void sortEV(sEncode* arr, int N) {
	int i, j, step;
	sEncode  tmp;
	for (step = N / 2; step >= 1; step /= 2)
		for (i = step; i < N; i++) {
			tmp = arr[i];
			for (j = i; j >= step; j -= step) {
				if ((tmp.symbol > arr[j - step].symbol) || (arr[j - step].symbol == 0 && tmp.symbol != 0))
					arr[j] = arr[j - step];
				else
					break;
			}
			arr[j] = tmp;
		}
}

////////////////////////////////// PRINT ENCODED MESSAGE /////////////////////////////////

void writeByte(FILE* out, unsigned char* bitBuffer, unsigned char bitCounter, unsigned char sym, bool flag) {
	unsigned char bit = 0;
	unsigned char counter = bitCounter;
	if (flag) {
		int i = 7;
		while (counter != 8) {
			bit = (sym >> i) & 1;
			*bitBuffer |= bit;
			if (counter != 7)
				*bitBuffer <<= 1;
			counter++;
			i--;
		}
		fwrite(bitBuffer, sizeof(unsigned char), 1, out);
		*bitBuffer = 0;
		counter = 0;
		while (counter != bitCounter) {
			bit = (sym >> i) & 1;
			*bitBuffer |= bit;
			if (counter != 7)
				*bitBuffer <<= 1;
			counter++;
			i--;
		}
	}
	else {
		fwrite(bitBuffer, sizeof(unsigned char), 1, out);
		*bitBuffer = 0;
	}
}

unsigned char printInfoAboutCoding(FILE* out, unsigned char  difSym, unsigned char TreeRecration[], unsigned char* count) {
	unsigned char bit = 0;
	unsigned char bitBuffer = 0;
	unsigned char bitCounter = 0;
	unsigned char countDifSym = 0;
	for (int i = 1; countDifSym != difSym; i++) {
		if (TreeRecration[i] == '1') {
			i++;
			bitBuffer |= 1;
			bitCounter++;
			if (bitCounter != 8)
				bitBuffer <<= 1;
			countDifSym++;
			if (bitCounter == 8) {
				writeByte(out, &bitBuffer, bitCounter, 1, false);
				bitCounter = 0;
				writeByte(out, &bitBuffer, bitCounter, TreeRecration[i], true);
				continue;
			}
			else {
				writeByte(out, &bitBuffer, bitCounter, TreeRecration[i], true);
				continue;
			}
		}
		else {
			bitBuffer |= 0;
			bitCounter++;
			if (bitCounter != 8)
				bitBuffer <<= 1;
		}
		if (bitCounter >= 8) {
			writeByte(out, &bitBuffer, bitCounter, 1, false);
			bitCounter = 0;
		}
	}

	*count = bitCounter;
	return bitBuffer;
}

void printEncodedMessage(FILE* in, FILE* out, sEncode* EncodedValues, int lenght, unsigned char bitBuffer, unsigned char bitCounter) {
	unsigned char val;
	bool flag = false;
	for (int i = 0; i < lenght; i++) {
		int j;
		fread(&val, 1, 1, in);
		for (j = 0; ; j++)
			if ((val == EncodedValues[j].symbol))
				break;
		unsigned char sym = EncodedValues[j].symbol;
		for (int u = 0; u < EncodedValues[j].lenght; u++) {
			unsigned char bit = EncodedValues[j].code[u] - '0';
			bitBuffer |= bit;
			if (bitCounter != 7) {
				bitBuffer <<= 1;
				bitCounter++;
			}
			else {
				bitCounter++;
				bitBuffer |= bit;
			}
			if (bitCounter == 8) {
				fwrite(&bitBuffer, sizeof(unsigned char), 1, out);
				bitCounter = 0;
				bitBuffer = 0;
			}
			flag = true;
		}
	}
	if (bitCounter != 0 && bitCounter != 8) {
		bitBuffer <<= 7 - bitCounter;
		fwrite(&bitBuffer, sizeof(unsigned char), 1, out);
	}
	if (bitCounter == 8)
		fwrite(&bitBuffer, sizeof(unsigned char), 1, out);
	fwrite(&bitCounter, sizeof(unsigned char), 1, out);
}

void print(FILE* in, FILE* out, sEncode* EncodedValues, int lenght, unsigned char TreeRecration[], unsigned char difSym) {
	unsigned char bitCounter = 0;
	unsigned char bitBuffer = printInfoAboutCoding(out, difSym, TreeRecration, &bitCounter);
	printEncodedMessage(in, out, EncodedValues, lenght, bitBuffer, bitCounter);
}

/////////////////////////////////// TREE RESTORATION /////////////////////////////////////

void scanSym(FILE* in, unsigned char* sym, unsigned char bitCounter, unsigned char* byte) {
	unsigned char bit = 0;
	unsigned char counter = bitCounter;
	while (bitCounter != 8) {
		bit = (*byte >> (7 - bitCounter)) & 1;
		*sym |= bit;

		*sym <<= 1;
		bitCounter++;
	}
	fread(byte, 1, 1, in);
	bitCounter = 0;
	while (counter != bitCounter) {
		bit = (*byte >> (7 - bitCounter)) & 1;
		*sym |= bit;
		if (bitCounter != counter - 1)
			*sym <<= 1;
		bitCounter++;
	}
}

void crtDT(FILE* in, sTree* t, sTree* ancestor, unsigned char* bitCounter, unsigned char* byte, char direction, bool startFlag) {
	if (!startFlag) {
		if (*bitCounter == 8) {
			fread(byte, 1, 1, in);
			*bitCounter = 0;
		}

		sTree* node = (sTree*)malloc(sizeof(sTree));
		node->left = NULL;
		node->right = NULL;
		t = node;
		if (direction == LEFT)
			ancestor->left = t;
		else
			ancestor->right = t;

		unsigned char bit = (*byte >> (7 - *bitCounter)) & 1;
		(*bitCounter)++;

		if (bit == 0)
			t->value = NOTNODE;
		else {
			unsigned char sym = 0;
			scanSym(in, &sym, *bitCounter, byte);
			t->value = sym;
			return;
		}
	}

	crtDT(in, t->left, t, bitCounter, byte, LEFT, false);
	crtDT(in, t->right, t, bitCounter, byte, RIGHT, false);
}

sTree* createStartDecodingTree() {
	sTree* head = (sTree*)malloc(sizeof(sTree));
	head->value = NOTNODE;
	head->right = NULL;
	head->left = NULL;
	return head;
}

sTree* treeRestoration(FILE* in, unsigned char* bitCounter, long* pos) {
	sTree* head = createStartDecodingTree();
	unsigned char byte = 0;
	fread(&byte, 1, 1, in);
	crtDT(in, head, head, bitCounter, &byte, LEFT, true);
	*pos = ftell(in) - 1;
	return head;
}

////////////////////////////////// PRINT DECODE MESSAGE //////////////////////////////////

unsigned char findStartOfStartByte(FILE* in, unsigned char bitStart) {
	unsigned char byte;
	fread(&byte, 1, 1, in);
	return byte;
}

void printDecodeMessage(FILE* in, FILE* out, sTree* head, long startPos, long endPos, unsigned char bitStart, unsigned char lenghtOfLastBit) {
	unsigned char byte = 0;
	unsigned char bit = 0;
	bool flag = true;
	sTree* buffHead = head;
	for (int i = startPos; i < endPos; i++) {
		unsigned char byteLenght = 8;
		if (i == startPos) {
			byte = findStartOfStartByte(in, bitStart);
			byteLenght = 8 - bitStart;
		}
		else
			fread(&byte, 1, 1, in);
		for (int u = 0; u < byteLenght; u++) {
			bit |= byte >> ((byteLenght - 1) - u) & 1;
			switch (bit) {
			case 0:
				buffHead = buffHead->left;
				break;
			case 1:
				buffHead = buffHead->right;
				break;
			}
			if (buffHead->value != NOTNODE) {
				fwrite(&buffHead->value, 1, 1, out);
				buffHead = head;
			}
			bit = 0;

		}
		flag = false;
	}
	if (lenghtOfLastBit > 0) {
		int i = 0;
		if (endPos == startPos) {
			byte = findStartOfStartByte(in, bitStart);
			i = startPos;
		}
		else
			fread(&byte, 1, 1, in);
		for (; i < lenghtOfLastBit; i++) {
			bit |= (byte >> 7 - i) & 1;
			switch (bit) {
			case 0:
				buffHead = buffHead->left;
				break;
			case 1:
				buffHead = buffHead->right;
				break;
			}
			if (buffHead->value != NOTNODE) {
				fwrite(&buffHead->value, 1, 1, out);
				buffHead = head;
			}
			bit = 0;
		}
	}

}

unsigned char scanLenghtLastBit(FILE* in, long* endPos, long startPos) {
	unsigned char lenghtOfLastBit;
	fseek(in, -1, SEEK_END);
	*endPos = ftell(in) - 1;
	fread(&lenghtOfLastBit, 1, 1, in);
	fseek(in, startPos, SEEK_SET);
	return lenghtOfLastBit;
}

void scanAndPrintDecodeMessage(FILE* in, FILE* out, sTree* head, long startPos, unsigned char bitCounter) {
	long endPos;
	unsigned char lenghtOfLastBit = scanLenghtLastBit(in, &endPos, startPos);
	printDecodeMessage(in, out, head, startPos, endPos, bitCounter, lenghtOfLastBit);

}

//////////////////////////////////////// OTHER ///////////////////////////////////////////

void freeTree(sTree* t) {
	if (t == NULL)
		return;
	freeTree(t->left);
	freeTree(t->right);
	free(t);
}

///////////////////////////////////// MAIN FUNCTIONS /////////////////////////////////////

void encodeMessage() {
	int messageLenght = 0;
	sStream stream;
	sRepeats NumberOfSymbolsIn[SIZE];
	sEncode* EncodedValues = (sEncode*)malloc(sizeof(sEncode) * SIZE);
	unsigned char TreeEdgeDirection[SIZE] = { 0 };
	unsigned char TreeRecration[TREERECRAIONSIZE];

	fillStreamForEncoding(&stream);
	fillNumberOfSymbolsIn(stream.in, &NumberOfSymbolsIn, &messageLenght);
	ShellSortForNumberOfSymbols(&NumberOfSymbolsIn, SIZE);
	unsigned char counterOfDifferentSymbols = countDifferentSymbols(NumberOfSymbolsIn);
	searchForTheSameAmount(&NumberOfSymbolsIn, counterOfDifferentSymbols);
	sTree* headTree = createCodingTree(NumberOfSymbolsIn, counterOfDifferentSymbols);

	prefOrder(headTree, EncodedValues, 0, '0', TreeEdgeDirection, &TreeRecration, false);
	sortEV(EncodedValues, counterOfDifferentSymbols);
	print(stream.in, stream.out, EncodedValues, messageLenght, TreeRecration, counterOfDifferentSymbols);
	free(EncodedValues);
	freeTree(headTree);
	closeStreamForEncoding(&stream);
}

void decodeMessage() {
	sStream stream;
	unsigned char bitCounter = 0;
	long pos;

	fillStreamForDecoding(&stream);
	sTree* head = treeRestoration(stream.in, &bitCounter, &pos);
	scanAndPrintDecodeMessage(stream.in, stream.out, head, pos, bitCounter);
	freeTree(head);
}

///////////////////////////////////////// MAIN ///////////////////////////////////////////

int main(int argc, char* argv[]) {

	/*switch (argv[1][0])
	{
	case 'a':
		encodeMessage();
		break;
	case 'x':
		break;
	default:
		break;
	}*/

	encodeMessage();
	decodeMessage();

	return 0;
}

