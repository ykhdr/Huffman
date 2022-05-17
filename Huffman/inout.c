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
		Введено меньше 3 параметров.\n\
		Для работы программы необходимо ввести:\n\
		-[ax]   : направление работы архиватора (архивация/деархивация)\n\
		fileIn  : входной файл\n\
		fileOut : выходной файл\n\n");
		break;
	case COUNTARGMAXERROR:
		fprintf(stdout, "\n\
		Введено больше 5 параметров.\n\
		Для работы программы необходимо ввести:\n\
		-[ax]   : направление работы архиватора (архивация/деархивация)\n\
		fileIn  : входной файл\n\
		fileOut : выходной файл\n\n");
		break;
	case PARAMERROR:
		fprintf(stdout, "\n\
		Неправильно введен параметр.\n\
		Доступные параметры:\n\
		-a : архивация файла\n\
		-x : деархивация файла\n\n");
		break;
	case HELP:
		fprintf(stdout, "\n\
		Для работы программы необходимо ввести:\n\
		-[ax]   : направление работы архиватора (архивация/деархивация)\n\
		fileIn  : входной файл\n\
		fileOut : выходной файл\n\n");
		break;
	default:
		break;
	}

	system("pause");
}

void successfulExitPrint(char* argv) {
	fprintf(stdout, "\n\
		Программа успешно завершила работу.\n\
		Результат находится в файле %s\n", argv);
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
