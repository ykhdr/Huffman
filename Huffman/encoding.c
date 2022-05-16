#include "encoding.h"

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
int countDifferentSymbols(sRepeats arr[]) {
	int i = 0;
	for (; arr[i].amount != 0; i++)
		if (i == UCHAR_MAX)
			break;;
	if (i == 255)
		return 256;
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
	if (N < 255)
		counter = N - 1;
	else
		counter = 255;
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

unsigned char printInfoAboutCoding(FILE* out, int  difSym, unsigned char TreeRecration[], unsigned char* count) {
	unsigned char bit = 0;
	unsigned char bitBuffer = 0;
	unsigned char bitCounter = 0;
	int countDifSym = 0;
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

void print(FILE* in, FILE* out, sEncode* EncodedValues, int lenght, unsigned char TreeRecration[], int difSym) {
	unsigned char bitCounter = 0;
	unsigned char bitBuffer = printInfoAboutCoding(out, difSym, TreeRecration, &bitCounter);
	printEncodedMessage(in, out, EncodedValues, lenght, bitBuffer, bitCounter);
}

void freeTreeForEncoding(sTree* t) {
	if (t == NULL)
		return;
	freeTreeForEncoding(t->left);
	freeTreeForEncoding(t->right);
	free(t);
}

void encodeMessage(char* fileIn, char* fileOut) {
	int messageLenght = 0;
	sStream streams;
	sRepeats NumberOfSymbolsIn[SIZE];
	sEncode* EncodedValues = (sEncode*)malloc(sizeof(sEncode) * SIZE);
	unsigned char TreeEdgeDirection[SIZE] = { 0 };
	unsigned char TreeRecration[TREERECRAIONSIZE];

	fillStreams(&streams, fileIn, fileOut);
	fillNumberOfSymbolsIn(streams.in, &NumberOfSymbolsIn, &messageLenght);
	ShellSortForNumberOfSymbols(&NumberOfSymbolsIn, SIZE);
	int counterOfDifferentSymbols = countDifferentSymbols(NumberOfSymbolsIn);
	searchForTheSameAmount(&NumberOfSymbolsIn, counterOfDifferentSymbols);
	sTree* headTree = createCodingTree(NumberOfSymbolsIn, counterOfDifferentSymbols);

	prefOrder(headTree, EncodedValues, 0, '0', TreeEdgeDirection, &TreeRecration, false);
	sortEV(EncodedValues, counterOfDifferentSymbols);
	print(streams.in, streams.out, EncodedValues, messageLenght, TreeRecration, counterOfDifferentSymbols);
	free(EncodedValues);
	freeTreeForEncoding(headTree);
	closeStream(&streams);
}