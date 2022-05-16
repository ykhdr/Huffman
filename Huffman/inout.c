#include "inout.h"

void fillStreams(sStream* stream, char* fileIn, char* fileOut) {
	stream->in = fopen(fileIn, "rb");
	stream->out = fopen(fileOut, "wb");
}

void closeStream(sStream* stream) {
	fclose(stream->in);
	fclose(stream->out);
}

void errorPrint(char caseError) {
	if (caseError == COUNTARGMINERROR)
		fprintf(stdout, "	������� ������ 3 ����������.\n	��� ������ ��������� ���������� ������:\n		-[ax]   : ����������� ������ ���������� (���������/�����������)\n		fileOut : �������� ����\n		fileIn  : ������� ����\n\n");

	if (caseError == COUNTARGMAXERROR)
		fprintf(stdout, "	������� ������ 4 ����������.\n	��� ������ ��������� ���������� ������:\n		-[ax]   : ����������� ������ ���������� (���������/�����������)\n		fileOut : �������� ����\n		fileIn  : ������� ����\n\n");

	if (caseError == PARAMERROR)
		fprintf(stdout, "	����������� ������ ��������.\n	��������� ���������:\n		-a : ��������� �����\n		-x : ����������� �����\n\n");

	if (caseError == HELP)
		fprintf(stdout, "	��� ������ ��������� ���������� ������:\n		-[ax]   : ����������� ������ ���������� (���������/�����������)\n		fileOut : �������� ����\n		fileIn  : ������� ����\n\n");

	system("pause");
}

bool checkInput(int argc, char* argv[], int* i) {
	if (!strcmp(argv[1], "-help")) {
		errorPrint(HELP);
		return EXIT_FAILURE;
	}
	if (argc < 4) {
		errorPrint(COUNTARGMINERROR);
		return EXIT_FAILURE;
	}
	if (argc == 4) {
		if (argv[1][0] != '-' && (argv[1][1] != 'a' || argv[1][1] != 'x')) {
			errorPrint(PARAMERROR);
			return EXIT_FAILURE;
		}
	}
	if (argc == 5) {
		if (argv[1][0] != '-' && (argv[1][1] != 'a' || argv[1][1] != 'x')) {
			errorPrint(PARAMERROR);
			return EXIT_FAILURE;
		}
		if (argv[2][0] != '-' && (argv[2][1] != 'a' || argv[2][1] != 'x')) {
			errorPrint(PARAMERROR);
			return EXIT_FAILURE;
		}
		*i = 2;
	}
	if (argc > 5) {
		errorPrint(COUNTARGMAXERROR);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}