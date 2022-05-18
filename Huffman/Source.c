#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#include "types.h"
#include "inout.h"
#include "encoding.h"
#include "decoding.h"

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "rus");
	if (checkInput(argc, argv))
		return EXIT_SUCCESS;
	switch (argv[1][1]) {
	case 'a':
		if (encodeMessage(argv[2], argv[3]))
			return EXIT_SUCCESS;
		break;
	case 'x':
		if (decodeMessage(argv[2], argv[3]))
			return EXIT_SUCCESS;
		break;
	default:
		errorPrint(PARAMERROR);
		return EXIT_SUCCESS;
		break;
	}
	successfulExitPrint(argv[argc - 1]);
	return EXIT_SUCCESS;
}

