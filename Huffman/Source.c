#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#include "types.h"
#include "inout.h"
#include "encoding.h"
#include "decoding.h"


int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "rus");
	int paramsCount = 1;
	if (checkInput(argc, argv, &paramsCount))
		return 0;
	for (int i = 1; i <= paramsCount; i++) {
		switch (argv[i][1]) {
		case 'a':
			encodeMessage(argv[3 + paramsCount - 1], argv[2 + paramsCount - 1]);
			break;
		case 'x':
			decodeMessage(argv[3 + paramsCount - 1], argv[2 + paramsCount - 1]);
			break;
		default:
			errorPrint(PARAMERROR);
			return 0;
			break;
		}
	}
	return 0;
}

