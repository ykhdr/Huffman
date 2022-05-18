#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

char fillStreams(sStream* stream, char* fileIn, char* fileOut);
void closeStream(sStream* stream);
void errorPrint(char caseError);
bool checkInput(int argc, char* argv[]);
void successfulExitPrint(char* argv);