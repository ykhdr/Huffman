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
	switch (caseError)
	{
	case COUNTARGMINERROR:
		fprintf(stdout, "\n\
		������� ������ 3 ����������.\n\
		��� ������ ��������� ���������� ������:\n\
		-[ax]   : ����������� ������ ���������� (���������/�����������)\n\
		fileIn  : ������� ����\n\
		fileOut : �������� ����\n\n");
		break;
	case COUNTARGMAXERROR:
		fprintf(stdout, "\n\
		������� ������ 5 ����������.\n\
		��� ������ ��������� ���������� ������:\n\
		-[ax]   : ����������� ������ ���������� (���������/�����������)\n\
		fileIn  : ������� ����\n\
		fileOut : �������� ����\n\n");
		break;
	case PARAMERROR:
		fprintf(stdout, "\n\
		����������� ������ ��������.\n\
		��������� ���������:\n\
		-a : ��������� �����\n\
		-x : ����������� �����\n\n");
		break;
	case HELP:
		fprintf(stdout, "\n\
		��� ������ ��������� ���������� ������:\n\
		-[ax]   : ����������� ������ ���������� (���������/�����������)\n\
		fileIn  : ������� ����\n\
		fileOut : �������� ����\n\n");
		break;
	default:
		break;
	}

	system("pause");
}

void successfulExitPrint(char* argv) {
	fprintf(stdout, "\n\
		��������� ������� ��������� ������.\n\
		��������� ��������� � ����� %s\n", argv);
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
	if (argc == 6) {
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
	if (argc > 6) {
		errorPrint(COUNTARGMAXERROR);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
