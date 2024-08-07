#pragma once
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "bn.h"

FILE* fileOpen(char* fname, char* mode);
void str2hex(const unsigned char* str, uint32_t* hex, size_t hexlen);