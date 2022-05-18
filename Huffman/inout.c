#include "inout.h"


char fillStreams(sStream* stream, char* fileIn, char* fileOut) {
	if ((stream->in = fopen(fileIn, "rb")) == NULL)
		return STREAMINERROR;
	if ((stream->out = fopen(fileOut, "wb")) == NULL)
		return STREAMOUTERROR;
	return SUCCESS;
}

void closeStream(sStream* stream) {
	fclose(stream->in);
	fclose(stream->out);
}

void errorPrint(char caseError) {
	switch (caseError) {
	case HELP:
		fprintf(stdout, "\n\
		��� ������ ��������� ���������� ������:\n\
		-[ax]   : ����������� ������ ���������� (���������/�����������)\n\
		fileIn  : ������� ����\n\
		fileOut : �������� ����\n\n");
		break;
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
		������� ������ 3 ����������.\n\
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
	case STREAMINERROR:
		fprintf(stdout, "\n\
		����������� ������ ���� � �������� �����.\n\
		��������� ������� � ���������� �����.\n\n");
		break;
	case STREAMOUTERROR:
		fprintf(stdout, "\n\
		����������� ������ ���� � ��������� �����.\n\
		��������� ������� � ���������� �����.\n\n");
		break;
	case SAMEINOUTERROR:
		fprintf(stdout, "\n\
		�������������� ������ FileIn � FileOut ���������.\n\
		��������� ������� � ���������� �������.\n\n");
		break;
	default:
		return;
	}
	system("pause");
}

void successfulExitPrint(char* argv) {
	fprintf(stdout, "\n\
		��������� ������� ��������� ������.\n\
		��������� ��������� � ����� %s\n", argv);
}

bool checkInput(int argc, char* argv[]) {
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
	if (argc > 4) {
		errorPrint(COUNTARGMAXERROR);
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[2], argv[3])) {
		errorPrint(SAMEINOUTERROR);
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
