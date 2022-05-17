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
		return EXIT_SUCCESS;
	for (int i = 1; i <= paramsCount; i++) {
		switch (argv[i][1]) {
		case 'a':
			encodeMessage(argv[2 + paramsCount + i - 2], argv[3 + paramsCount + i - 2]);
			break;
		case 'x':
			decodeMessage(argv[2 + paramsCount + i - 2], argv[3 + paramsCount + i - 2]);
			break;
		default:
			errorPrint(PARAMERROR);
			return EXIT_FAILURE;
			break;
		}
	}
	successfulExitPrint(argv[argc - 1]);
	return EXIT_SUCCESS;
}

