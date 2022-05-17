#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

void fillStreams(sStream* stream, char* fileIn, char* fileOut);
void closeStream(sStream* stream);
void errorPrint(char caseError);
bool checkInput(int argc, char* argv[], int* i);
void successfulExitPrint(char* argv);