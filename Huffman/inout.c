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
		Для работы программы необходимо ввести:\n\
		-[ax]   : направление работы архиватора (архивация/деархивация)\n\
		fileIn  : входной файл\n\
		fileOut : выходной файл\n\n");
		break;
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
		Введено больше 3 параметров.\n\
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
	case STREAMINERROR:
		fprintf(stdout, "\n\
		Неправильно введен путь к входному файлу.\n\
		Повторите попытку с корректным путем.\n\n");
		break;
	case STREAMOUTERROR:
		fprintf(stdout, "\n\
		Неправильно введен путь к выходному файлу.\n\
		Повторите попытку с корректным путем.\n\n");
		break;
	case SAMEINOUTERROR:
		fprintf(stdout, "\n\
		Местоположение файлов FileIn и FileOut совпадает.\n\
		Повторите попытку с различными файлами.\n\n");
		break;
	default:
		return;
	}
	system("pause");
}

void successfulExitPrint(char* argv) {
	fprintf(stdout, "\n\
		Программа успешно завершила работу.\n\
		Результат находится в файле %s\n", argv);
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
