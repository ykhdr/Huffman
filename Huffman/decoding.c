#include "decoding.h"

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
	for (int i = startPos; i < endPos || (i <= endPos && lenghtOfLastBit == 0); i++) {
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
void freeTreeForDecoding(sTree* t) {
	if (t == NULL)
		return;
	freeTreeForDecoding(t->left);
	freeTreeForDecoding(t->right);
	free(t);
}

bool decodeMessage(char* fileIn, char* fileOut) {
	sStream streams;
	unsigned char bitCounter = 0;
	long pos;

	char streamsStatus = fillStreams(&streams, fileIn, fileOut);
	if (streamsStatus != SUCCESS) {
		errorPrint(streamsStatus);
		return EXIT_FAILURE;
	}

	sTree* head = treeRestoration(streams.in, &bitCounter, &pos);
	scanAndPrintDecodeMessage(streams.in, streams.out, head, pos, bitCounter);
	freeTreeForDecoding(head);
	closeStream(&streams);

	return EXIT_SUCCESS;
}