#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void printWColor(char* str,char colorcode);
void VideoPrintChar(char character);
void VideoBackSpace();
void VideoNewLine();
void ncPrintChar(char character);
void ncNewline();
void printTime();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

#endif